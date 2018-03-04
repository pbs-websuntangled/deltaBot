#ifndef Pot_h
#define Pot_h
/* 
  This reads 
*/
#include "Arduino.h" // Don't really understand why this include needed. Seems just the same as Bot.h and not required there. Hope nothing amiss!!
const int generations = 10; // this number cannot change dynamically and is the maximum number of values to store
class Pot
  {
    public:
      Pot(int botNumber, int legNumber, int legPotPin);
      Pot(); // Just there to create the reference
      int getLegNumber();
      int getPotPin();

      void pbs(int *num1, int *num2);
      long readGeneration(int generation, char* typeOfRead); // generation 0 is the latest and does a digitalread then stores the result and rolls the others over. type = reading or millis
      int readMax(int millisToInclude); 
      float readMaxDrByDt(int millisToInclude); 
      float readMinDrByDt(int millisToInclude);   
      int readMin(int millisToInclude);   
      float dReadingByDTime(int generation, int millisToInclude);
      void dumpIt(char* dumpMessage); // send all the results to the serial port
      void clearIt(); // Zeroise all the readings and timings 
      
    private:
      int _legNumber;
      int _botNumber;
      
      int _potPin; 
      int _generation;
      int _millisToInclude;
      char* _typeOfRead;
      
      int _reading[generations];
      long _readingMillis[generations];
      int _thisPotReading;
      int _oldestPotReading;
      int _i;
      char* _dumpMessage;
      int _generationsToInclude;
    
  };
#endif
