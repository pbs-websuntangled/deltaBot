Bot::Bot(int botNumber, int leg1StepPin, int leg1DirectionPin, int leg1PotPin, int leg2StepPin, int leg2DirectionPin, int leg2PotPin, int leg3StepPin, int leg3DirectionPin, int leg3PotPin)
  :
    leg0(botNumber, 0, leg1StepPin, leg1DirectionPin, leg1PotPin),
    leg1(botNumber, 1, leg2StepPin, leg2DirectionPin, leg2PotPin),
    leg2(botNumber, 2, leg3StepPin, leg3DirectionPin, leg3PotPin)
  {           
    _botNumber = botNumber;
  } // End of Constructor 

boolean Bot::moveBotRun() 
  {
    boolean returnValue = leg0.moveClimberRun() * leg1.moveClimberRun() * leg2.moveClimberRun();
    
    if (leg0.distanceforClimberToGo() == 0)
      {
        leg0Finished = true;
      }
    if (leg1.distanceforClimberToGo() == 0)
      {
        leg1Finished = true;
      }
    if (leg2.distanceforClimberToGo() == 0)
      {
        leg2Finished = true;
      }
    return returnValue;
  }
void Bot::botDisable() 
  {
     leg0.motor.disableOutputs();
     leg1.motor.disableOutputs();
     leg2.motor.disableOutputs();
  }

 
 
 void Bot::initiate() 
  {  
    leg0.initiateMotor(); // Do the stuff that can't be done at instantiate time
    leg1.initiateMotor(); // Do the stuff that can't be done at instantiate time
    leg2.initiateMotor(); // Do the stuff that can't be done at instantiate time
    int t1 = millis();
    boolean botInitiated = false;
    boolean leg0Initiated = false;
    boolean leg1Initiated = false;
    boolean leg2Initiated = false; 
    
    while (botInitiated == false)
      {
        if (leg0Initiated == false)
          {
            leg0Initiated = leg0.initiate();
          }
        if (leg1Initiated == false)
          {
            leg1Initiated = leg1.initiate();
          }          
        if (leg2Initiated == false)
          {
            leg2Initiated = leg2.initiate();
          }          
        
        botInitiated = leg0Initiated * leg1Initiated * leg2Initiated; // And them together
          
      }
    int t2 = millis();

    Serial.print("The bot has initialised in (millis) ");
    Serial.println(t2-t1);    
    Serial.println("Just finished the instantiation of bot0 ");
    
    Serial.println("Now testing a few elements of the parameter structure:");
    Serial.print("Pin A0 is 14 on Uno 54 on Mega = bot0.leg0.torSenPot.getPotPin() = ");
    Serial.println(leg0.torSenPot.getPotPin());
    Serial.print("Pin A2 is 15 on Uno 55 on Mega = bot0.leg1.torSenPot.getPotPin() = ");
    Serial.println(leg1.torSenPot.getPotPin());
    Serial.print("Pin A3 is 16 on Uno 56 on Mega = bot0.leg2.torSenPot.getPotPin() = ");
    Serial.println(leg2.torSenPot.getPotPin());    
    
    Serial.print("botArray[0].legArray[0].motor.pinStep = ");    
    Serial.println(botArray[0].legArray[0].motor.pinStep);    
    Serial.print("botArray[0].legArray[0].motor.pinDirection = ");    
    Serial.println(botArray[0].legArray[0].motor.pinDirection);       
    
    Serial.print("leg0.getZ() = ");
    Serial.println(leg0.getZ());     
    Serial.print("leg1.getZ() = ");
    Serial.println(leg1.getZ());   
    Serial.print("leg2.getZ() = ");
    Serial.println(leg2.getZ()); 

  }


