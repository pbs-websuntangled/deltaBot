#ifndef InitBotArray_h
#define InitBotArray_h

/*
 
*/
/*
  Structure for holding all the bot parameters
*/

struct Point
  {
     long x; // millimetres 
     long y; // millimetres
     long z; // millimetres
  } ;
struct MotorParams
  {
     int pinStep; // tells it to step
     int pinDirection; // Sets the direction of the motor 
     int pinEnable; // This pin enabled the power output on the stepper driver
     int stepsPerRevolution; // Big Easy driver is 3200
     float winderCircumference; // Millimetres. Need to be done accurately. Wind 5 turns and measure the distance moved by the climber divide by 5 , divide by pi
  } ;
struct ClimberDims
  {
     int topStopToAxis; // Z Distance from the top of the guide tubes to the axis of the UJ
     int bottomStopToAxis; // Z Distance from the bottom of the guide tubes to the axis of the UJ
     int pointToAxis; // Straight line distance from the outside triangle intersection to the axis of the uj ignoring z plane
  } ;
struct LegStruct
  {
     int legNumber; // Start with the point facing forward or up. The bottom left is 0, then work clockwise round 
     int potPin; // The pin that reads the torque sensing potentiometer for that leg. A0 - A5. These map to int 14 to int 19. Different on Mega
                 // As you look at the Pot knob, the lh one goes to 5v, the middle one goes to Analog pin, the rh one goes to ground
     int height; // Between the stops
     ClimberDims climber;
     Point legPoint;
     MotorParams motor;
  } ;

struct BotArray
  {
    int botNumber;
    LegStruct legArray[3]; // The number of legs in a delta bot is 3
  } ;

class InitBotArray
  {
    public:
      InitBotArray();
      
    private:
    

  };

#endif
