/*
  Author: PBS
  Date: 30/01/2015

  Main features:
  
    Creates a structure to hold all of the key variables and a way of initialising them that is not positional
    
    Has the classes of Bot, Leg and Pot
    
    It works. It is a key staging point
    
*/
#include "InitBotArray.h"
#include "Bot.h"

/*
  This is a library that drives a stepper motor
  Get it from here: http://www.airspayce.com/mikem/arduino/AccelStepper/ 
  Motor wired to little red easydriver
  From left to right (so you can read th ewriting on driver board)
    Was
      Red Black Green Blue (from motor to junction block)
      brown yellow orange red (From JB to Driver)
      yellow orange red brown (Into Driver)
    
    Now
      Black Green Blue Red (from motor to junction block)
      Yellow Orange Red Brown (from junction block to driver)
      Yellow Orange Red Brown (into the driver (so you can read the writing on the driver))
*/
#include <AccelStepper.h> // Don't really understand why this library has to be included here. I thought I would only need it in Bot.h but it doesn't compile if I put it there only

int nema17MaxStepsPerSecond = 2500; // Experimentation tells the maximum the program can drive the steppers
boolean firstTimeThrough = true;

BotArray botArray[1]; // Create a 1 element array (there will be more) to hold all the structure.
InitBotArray botArray1; // Get it to initialise the array before setting up the bot as a global variable

/*
  Set up the array of pins for Bot 0
  Neds to be different from the structure as that doesn't get set up until after instantiation
  These arrays contain the stuff that is used to instantiate the classes within bot
*/
int botLegStepPinArray[1][3] = {10,4,7};
int botLegDirectionPinArray[1][3] = {11,5,8};
int botLegEnablePinArray[1][3] = {12,6,9};
int botLegPotPinArray[1][3] = {A0,A1,A2};

Bot bot0(0,   botLegStepPinArray[0][0], botLegDirectionPinArray[0][0], botLegPotPinArray[0][0],   4,5,6,   7,8,9); // Problem. When I uncomment this, it doesn't run. It really should!!

void setup() 
  {

    Serial.begin(115200);   
    Serial.println("Hey There. I'm running classPlayGround09");
    Serial.println("Hey There. I'm running classPlayGround09"); 
    
    Serial.print("botArray[0].legArray[0].motor.pinStep = ");
    Serial.println(botArray[0].legArray[0].motor.pinStep);
    
    bot0.initiate(); // Do all the stuff that cant be done in instantiate
   
    Serial.println("In setup, finished all the initiation"); 
  }
  
void loop() 
  {

    if ( firstTimeThrough == true)
      {
        firstTimeThrough = false;
        Serial.print("in loop, Pin A3 is 16 on Uno 56 on Mega = bot0.leg2.torSenPot.getPotPin() = ");
        Serial.println(bot0.leg2.torSenPot.getPotPin());
        bot0.botDisable();
      }

  } // End of loop

