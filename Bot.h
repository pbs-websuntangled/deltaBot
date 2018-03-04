#ifndef Bot_h
#define Bot_h


/*
  The bot is instantiated with a number to identify which it is.
  We then create the forward reference (I think) to allow the creation of leg1, 2 and 3  
*/

#include "Leg.h"


class Bot
  {
    public:
      Bot(int botNumber, int leg1StepPin, int leg1DirectionPin1, int leg1PotPin1, int leg2StepPin, int leg2DirectionPin, int leg2PotPin, int leg3StepPin, int leg3DirectionPin, int leg3PotPin);
      void initiate();
      boolean moveBotRun();
      void botDisable();
      Bot();

      Leg leg0;
      Leg leg1;
      Leg leg2;    
    
      String x; // ???? what is this?????
      
    private:
    
      boolean leg0Finished;
      boolean leg1Finished;
      boolean leg2Finished;
      int _botNumber;

  };

#endif
