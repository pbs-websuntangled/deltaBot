#ifndef Leg_h
#define Leg_h

/*

*/

#include "Pot.h"
#include <AccelStepper.h> 

class Leg
  {
    public:
      Leg(int botNumber, int legNumber, int legStepPin, int legDirectionPin, int legPotPin);
      Leg();
      Pot torSenPot;      
      AccelStepper motor;
      
      boolean initiate();
      void testIt();
      int getLegNumber();
      int getBotNumber();
      void initiateMotor();
      int getZ();
      long moveClimberTo(long z, long millisToMoveIn); // called many times (once per loop) and initiates a move when it needs to
      boolean moveClimberRun();      
      int distanceforClimberToGo();
        
    private:
      
      int _legNumber;
      int _botNumber; 
        
      int _legHeight;
      long _legHeightSteps;
      float _mmPerStep;
      
      boolean _firstTimeThrough;
      boolean _definitelyAtTop;
      boolean _topDone;
      boolean _arrayFilledWithSlipsAtTop;    
      int _slipTorSenReadingAtTop;
      
      boolean _definitelyAtBottom;
      boolean _bottomDone;
      boolean _arrayFilledWithSlipsAtBottom;     
      int _slipTorSenReadingAtBottom;  
      long _motorDistanceToGo;    
  };

#endif
