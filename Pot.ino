
Pot::Pot(int botNumber, int legNumber, int legPotPin) 
  {
    _legNumber = legNumber;
    _botNumber = botNumber;
    _potPin = legPotPin;
    
    clearIt();
    //Serial.print("instantiating Pot with legNumber = ");
    //Serial.println(_legNumber); 
  }

int Pot::getLegNumber()
  {
    return _legNumber;
  }

int Pot::getPotPin()
  {
    return _potPin;
  }

float Pot::dReadingByDTime(int generation, int millisToInclude)
  /*
  Starts at a generation and then looks at the rate of change of the readings over a requested period of change
  Not certain that I need the start at generation any more. I always call it with 1. In previous methods, I did use it but no more.
  */
  {
    _millisToInclude = millisToInclude;
    int _endReading = readGeneration(_generation,"reading");
    long _startReading;
    long _endReadingMillis = readGeneration(_generation,"millis");
    long _startReadingMillisActual;
    long _startReadingMillisTarget = _endReadingMillis - _millisToInclude;
    float millisDifference;
    int _i1;
    float _drbydt;
    
    for (_i1=_generation; _i1 < generations +1; _i1++)
    {
      if (readGeneration(_i1,"millis") >= _startReadingMillisTarget) // is the current reading of millis still later than the earliest one that I want
        {
          _startReading = readGeneration(_i1,"reading");
          _startReadingMillisActual = readGeneration(_i1,"millis");      
        }  
      else // I've now read earlier than my earliest target
        {
          if (readGeneration(_i1,"millis") == 0)//I've run out of readings before getting to the target time start
            {
              return(0);
            }
          millisDifference = _endReadingMillis - _startReadingMillisActual;        
          if (_endReadingMillis - _startReadingMillisActual < 1)// The two readings were in the same millisecind. Because millisDifference is float, compare to zero is not good idea.
            {
              millisDifference = 0.5; // the alternative seems to be storing microseconds and the overhead for this is too expensive
            }                         
          _drbydt =  float((1.00*_endReading - _startReading) / (1.00*millisDifference));     
          return(_drbydt);      
        } // end of (else) ive read earlier than my earliest target
    } // end of for each generation
    return 0; // There weren't enough readings to get me the correct time sample
  } // end of function

int Pot::readMin(int millisToInclude)
  {
    long _endReadingMillis = readGeneration(1,"millis");
    long _startReadingMillisActual;
    long _startReadingMillisTarget = _endReadingMillis - _millisToInclude;
    float millisDifference;     
    _millisToInclude = millisToInclude;
    int _readMinValue = 9999;
    int _potentialReadMinValue;
    for (_i=1; _i < generations+1; _i++)
      {
        if (readGeneration(_i,"millis") >= _startReadingMillisTarget) // is the current reading of millis still later than the earliest one that I want
          {
            _potentialReadMinValue = readGeneration(_i,"reading");
            _readMinValue = min(_readMinValue, _potentialReadMinValue); // Language reference says don't do functions in this one      
          }
        else // I've read all the values that are in the time frame requested
          {
            return _readMinValue;
          }  
      } //end of for each reading
    return _readMinValue;
  } // End of function

int Pot::readMax(int millisToInclude)
  {
    long _endReadingMillis = readGeneration(1,"millis");
    long _startReadingMillisActual;
    long _startReadingMillisTarget = _endReadingMillis - _millisToInclude;
    float millisDifference;  
    
    _millisToInclude = millisToInclude;
    int _readMaxValue = 0;
    int _potentialReadMaxValue;
    for (_i=1; _i < generations+1; _i++)
      {
        if (readGeneration(_i,"millis") >= _startReadingMillisTarget) // is the current reading of millis still later than the earliest one that I want
          {
            _potentialReadMaxValue = readGeneration(_i,"reading");
            _readMaxValue = max(_readMaxValue, _potentialReadMaxValue); // Language reference says don't do functions in this one      
          }
        else // I've read all the values that are in the time frame requested
          {
            return _readMaxValue;
          }    
      } //end of for each reading
    return _readMaxValue;
  } // End of function

float Pot::readMaxDrByDt(int millisToInclude) // Goes through the whole array but calculates using the millis to include
  {
    float maxDrByDt;
    for (_i=1; _i < generations+1; _i++)
      {
        float tempDrByDt = dReadingByDTime(_i,millisToInclude);  
        maxDrByDt = max (maxDrByDt, tempDrByDt);  // language ref says dont do functions inside
      }
    return maxDrByDt; 
  } // End of function

float Pot::readMinDrByDt(int millisToInclude)// Goes through the whole array but calculates using the millis to include
  {
    float minDrByDt;
    for (_i=1; _i < generations+1; _i++)
      {
        float tempDrByDt = dReadingByDTime(_i,millisToInclude);     
        minDrByDt = min (minDrByDt, tempDrByDt);  // language ref says dont do functions inside
      }
    return minDrByDt; 
  } // End of function

void Pot::dumpIt(char* dumpMessage)
  {
    _dumpMessage = dumpMessage;
    /*
    Serial.println("============= OO Dump of the Pot=============="); 
    Serial.println(_dumpMessage); 
    Serial.println("generation, millis, reading, drbydt(5)");     
    for (_i=1; _i < generations+1; _i++)
      {
        Serial.print(_i); 
        Serial.print(", ");  
        Serial.print(readGeneration(_i,"millis")); 
        Serial.print(", "); 
        Serial.print(readGeneration(_i,"reading")); 
        Serial.print(", "); 
        
        Serial.println(float(dReadingByDTime(_i,5)));             
   
        
      }
    Serial.println("============= End OO Dump of the Pot ==============");  
    */
  } // End of function

void Pot::clearIt()
  {
    for (_i=0; _i < generations; _i++) // Remember that generations is number of entries so is like a position rather than a displacement 
      {
        _reading[_i] = 0;
        _readingMillis[_i] = 0;    
      }
    _thisPotReading = -1; // Needs to be ready to have 1 added and then be 0
    _oldestPotReading =  generations - 1; // -1 for displacement not position another 
  } // End of function
  
  
long Pot::readGeneration(int generation, char* typeOfRead)
  /*
  Return a reading of the pot or the time it was read at.
  If generation 0 then do a fresh reading and return it. 
  If other than 0 then go to that generation (1 = last reading made etc) 
  Read type can be millis or reading
  */
  {
    _generation = generation;
    _typeOfRead = typeOfRead;
    boolean readMillis = false;
    boolean readReading = false;
    
    if ((_typeOfRead == "reading") || (_typeOfRead == "millis") ||  (_typeOfRead == "motorPosition")) // 
      {  
      }  
    else
      {
        return 9999;
      }
    if (_generation > generations)
      {  
        return 9999;
      }
    if (_generation < 0)
      {  
        return -9999;
      }  
      
    if (_generation == 0 )
      {
        _thisPotReading = _thisPotReading + 1;
        _oldestPotReading = _oldestPotReading + 1;
        if (_thisPotReading > generations - 1)
          {
            _thisPotReading = 0;         
          }
        if (_oldestPotReading > generations - 1) // generations is positional, not displacement
          {
            _oldestPotReading = 0;         
          }          
        //digitalWrite(_supplyPin, HIGH); // Power up the Pot
        _reading[_thisPotReading] = (analogRead(_potPin)); 
        _readingMillis[_thisPotReading] = millis();   
        //digitalWrite(_supplyPin, LOW); // Power down the Pot          
        _generation = 1; // now I've added a new reading, let the next bit return the 1 generation
      } // end of if generation 0
  
    int _generationDisplacement = _generation -1; // turn generation from a position to a displacement
    if (_thisPotReading - _generationDisplacement >= 0) // is it before the rollover
      {
        if (_typeOfRead == "reading")
          {    
            return _reading[_thisPotReading - _generationDisplacement]; 
          } 
        if (_typeOfRead == "millis")
          {    
            return _readingMillis[_thisPotReading - _generationDisplacement]; 
          }        
          
      } // end of is it before the rollover
    else // it's rolled over
      {
        if (_typeOfRead == "reading")
          {    
            return _reading[generations + _thisPotReading - _generationDisplacement]; 
          } 
        if (_typeOfRead == "millis")
          {    
            return _readingMillis[generations + _thisPotReading - _generationDisplacement]; 
          } 
          
      } // end of its rolled over
  } // End of function
