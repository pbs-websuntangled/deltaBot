
Leg::Leg(int botNumber, int legNumber, int legStepPin, int legDirectionPin, int legPotPin)
  : 
    torSenPot(botNumber, legNumber, legPotPin),
    motor(AccelStepper::DRIVER,legStepPin,legDirectionPin)
  {
    _legNumber = legNumber;
    _botNumber = botNumber;
    _firstTimeThrough = true;
    _definitelyAtTop = false;
    _definitelyAtBottom = false;
    _arrayFilledWithSlipsAtTop = false;  
    _arrayFilledWithSlipsAtBottom = false;
    _topDone = false;
    _bottomDone = false;   

  }
// ---------------------------------------------------------------------------------------------------------------------------

int Leg::getZ()
  {
    return motor.currentPosition() * _mmPerStep;
  }

// ---------------------------------------------------------------------------------------------------------------------------

int Leg::getLegNumber()
  {
    return _legNumber;
  }
// ---------------------------------------------------------------------------------------------------------------------------  
void Leg::initiateMotor()
  {
   
    motor.setEnablePin(botArray[_botNumber].legArray[_legNumber].motor.pinEnable); // Tell it that the enable pin is set to number  for this stepper
    motor.setPinsInverted(true, false, true); // DIR, STEP, ENABLE   
    motor.setMaxSpeed(nema17MaxStepsPerSecond);
    motor.enableOutputs();
    
    _mmPerStep = botArray[_botNumber].legArray[_legNumber].motor.winderCircumference / botArray[_botNumber].legArray[_legNumber].motor.stepsPerRevolution;
    _legHeight = botArray[_botNumber].legArray[_legNumber].height - botArray[_botNumber].legArray[_legNumber].climber.topStopToAxis - botArray[_botNumber].legArray[_legNumber].climber.bottomStopToAxis;
    _legHeightSteps = _legHeight / _mmPerStep;
    
    _motorDistanceToGo = 99999999;
    
    testIt(); 
  }
// ---------------------------------------------------------------------------------------------------------------------------  
int Leg::getBotNumber()
  {
    return _botNumber;
  }
// ---------------------------------------------------------------------------------------------------------------------------
boolean Leg::initiate()
  {
      
    /*
      Each time called, first job is to get to the bottom so do that a bit at a time until done
      Then once we know it's done, then fill the array with readings after a move bit by bit on each call
      Then set the pot reading it slips at for the bottom
      
      Then go to the top quickly without doing torSen readings
      Once definitely at top, fill array with slips, set position to zero and store the pot reading it slips at
    */
    int millisToMoveIn;

    if (_bottomDone == false)
      {
        if (_firstTimeThrough == true)
          {
            _firstTimeThrough = false;               
            long millisToMoveIn = (1000 * 1.05 * _legHeightSteps) / nema17MaxStepsPerSecond;
            moveClimberTo(long (-1.05 * _legHeight), millisToMoveIn);
            //_definitelyAtBottom = false;            
          }

        if (_definitelyAtBottom == false)
          {
            if (moveClimberRun() == true)
              {
                if (motor.distanceToGo() == 0)
                  {
                    _definitelyAtBottom = true;   
                    //Serial.print("I'm def at the bottom for legNumber = ");
                    //Serial.println(_legNumber);
                    
                    long millisToMoveIn = (50 /_mmPerStep) * 1000/ nema17MaxStepsPerSecond; // so now get ready to fill the array
                    moveClimberTo(getZ() - 50, millisToMoveIn); // tell it to move down another 50mm

                  } // end of did it arrive
              } // end of did it move         
          } // end of am i def at bottom      
        else // I'm not def at bottom. This must be an else, not the inverse of the if or it will do 2 moves when the condiotion is met.
          {
            if (_arrayFilledWithSlipsAtBottom == false)
              {
                //Serial.print("Filling array with slips for legNumber = ");
                //Serial.print(_legNumber);
                //Serial.print(", motor.distanceToGo() =");
                //Serial.println(motor.distanceToGo());
                
                if (moveClimberRun() == true) //Funtion moves the motor if required and returns true if it moved
                  {
                    torSenPot.readGeneration(0, "reading");
                    if (motor.distanceToGo() == 0)
                      {
                        //Serial.print("Filled torSenArray with slips at bottom for legNumber = ");
                        //Serial.println(_legNumber);                          
                        _arrayFilledWithSlipsAtBottom = true;
                        _bottomDone = true;
                        //_firstTimeThrough = true;
                        _slipTorSenReadingAtBottom = torSenPot.readMin( millisToMoveIn);
                        
                        //Serial.print("    finished initialising bottom and getZ = ");
                        //Serial.println(getZ());
                        
                        long millisToMoveIn = (1000 * 1.05 * _legHeightSteps) / nema17MaxStepsPerSecond; // Prepare for initialising the bottom
                        moveClimberTo(long (getZ() + 1.05 * _legHeight), millisToMoveIn); // Prepare for initialising the bottom
            
                        //Serial.print("    _slipTorSenReadingAtBottom for legNumber = ");
                        //Serial.println(_slipTorSenReadingAtBottom); 

                        
                        return false;
                      }
                  }                        
              }                     
          } // end of if definitelyAtBottom                    
      } // end of if bottom done    
      
      if (_bottomDone == true)
      {
        if (_topDone == false)
        {
          if (_definitelyAtTop == false)
            {
              if (moveClimberRun() == true)
                {
                  if (motor.distanceToGo() == 0)
                    {
                      _definitelyAtTop = true;
                      //Serial.print("I'm def at the top for legNumber = ");
                      //Serial.println(_legNumber);                      
                      
                      motor.setCurrentPosition(500); // make sure that 0 is just away from the top position
    
                      long millisToMoveIn = (generations) * 1000/ nema17MaxStepsPerSecond; 
                      moveClimberTo(-generations, millisToMoveIn);
                    }
                }          
            }        
          else // This must be an else, not the inverse of the if or it will do 2 moves when the condiotion is met.
            {
              if (_arrayFilledWithSlipsAtTop == false)
                {
                  if (moveClimberRun() == true) //Funtion moves the motor if required and returns true if it moved
                    {
                      torSenPot.readGeneration(0, "reading");
                      if (motor.distanceToGo() == 0)
                        {
                          _arrayFilledWithSlipsAtTop = true;
                          _topDone = true;
                          return true;
                        }
                    }                        
                }                     
            } // end of if definitelyAtTop        
        } // end of if topDone        
      } // end of if bottomDone


     return false;                
  }

// --------------------------------------------------------------------------------------------------------------------------- 

long Leg::moveClimberTo(long z, long millisToMoveIn)
  {
    /*
      Sets the position that the motor should move towards each time the moveClimberRun() method is called
      Also calculate the speed that is needed to achieve the move in the desired number of milliseconds
    */
    long _z = z;
    long _zStepTarget = _z /_mmPerStep;
    long _deltaZStep = _zStepTarget - motor.currentPosition();
    long _millisToMoveInTarget = millisToMoveIn;
    motor.moveTo(_zStepTarget); 
    float _speedTemp = (1000.0*_deltaZStep)/_millisToMoveInTarget ;
    motor.setSpeed(float(_speedTemp));  // Warning!!!! put the signing on speed and position carefully right   
    long _millisToMoveInActual = 1000 * _deltaZStep / motor.speed();   
   
    Serial.print("moveClimberTo(): For legNumber = ");
    Serial.println (_legNumber); 
    
    if (_millisToMoveInActual == _millisToMoveInTarget)
      {
        Serial.print(" good: _millisToMoveInActual = ");
        Serial.print(_millisToMoveInActual);
      }
     else
      {
        Serial.print(" bad: _millisToMoveInActual = ");
        Serial.print(_millisToMoveInActual);     
      }     
    
    Serial.print(", _z = ");
    Serial.print(_z);     
  
    Serial.print(", _millisToMoveInTarget = ");
    Serial.print(_millisToMoveInTarget);  
    
    Serial.print(", motor.currentPosition() = ");
    Serial.print(motor.currentPosition());  
    
    Serial.print(", _zStepTarget = ");
    Serial.print(_zStepTarget);     
    
    Serial.print(", speed of motor (motor.speed()) has been set to (steps / second) = ");
    Serial.println(motor.speed());   
    
    return _millisToMoveInActual;
  }


// --------------------------------------------------------------------------------------------------------------------------- 

boolean Leg::moveClimberRun()
  {
    boolean motorDidStep = motor.runSpeed();
    
    //Serial.print("moveClimberRun() for legNumber = ");
    //Serial.print(_legNumber);    
    //Serial.print(", motor.distanceToGo() = ");
    //Serial.println(motor.distanceToGo());     
    
    if (motorDidStep == true)
      {
        if (_motorDistanceToGo - motor.distanceToGo() > 100)
          {
            _motorDistanceToGo = motor.distanceToGo();
            /*   
            Serial.print("moveClimberRun() for legNumber = ");
            Serial.print(_legNumber); 
            Serial.print(" and _motorDistanceToGo = ");
            Serial.print(_motorDistanceToGo);  
            Serial.print(" and motor.speed() = ");
            Serial.println(motor.speed());             
            */
        } // end of have i moved more than 100                      
      } // end of did I move
    //
    return motorDidStep;
  } 

// ---------------------------------------------------------------------------------------------------------------------------  

void Leg::testIt()
  {
    
    Serial.print("instantiating Leg with botNumber = ");
    Serial.print(_botNumber);    
    
    Serial.print(" and legNumber = ");
    Serial.println(_legNumber);      

    Serial.print("botArray[botNumber].legArray[legNumber].motor.stepsPerRevolution = ");
    Serial.println(botArray[_botNumber].legArray[_legNumber].motor.stepsPerRevolution);   

    Serial.print("botArray[botNumber].legArray[legNumber].motor.winderCircumference = ");
    Serial.println(botArray[_botNumber].legArray[_legNumber].motor.winderCircumference); 
 
    Serial.print("For precision, 100,000 * _mmPerStep = ");
    Serial.println(100000 * _mmPerStep);   
 
    Serial.print("_legHeight = ");
    Serial.println(_legHeight);  
 
    Serial.print("_legHeightSteps = ");
    Serial.println(_legHeightSteps);     
    
    int x = torSenPot.readGeneration(0,"reading"); // Force a read
    x = torSenPot.readGeneration(0,"reading"); // Force a read
    x = torSenPot.readGeneration(0,"reading"); // Force a read
    x = torSenPot.readGeneration(0,"reading"); // Force a read
    torSenPot.dumpIt("In Leg Constructor: There are 4 readings");      
    
    
  }
  
// ---------------------------------------------------------------------------------------------------------------------------  

int Leg::distanceforClimberToGo()
  {   
    return _mmPerStep / motor.distanceToGo();  
  }  
  
