InitBotArray::InitBotArray()
  {           
    
    botArray[0].botNumber = 0;
    
    botArray[0].legArray[0].legNumber = 0;
    botArray[0].legArray[0].potPin = A0;
    botArray[0].legArray[0].height = 671; // was 671
    botArray[0].legArray[0].climber.topStopToAxis = 70; // measured correct
    botArray[0].legArray[0].climber.bottomStopToAxis = 1; // measured correct
    botArray[0].legArray[0].climber.pointToAxis = 100;    
    botArray[0].legArray[0].legPoint.x = 0;
    botArray[0].legArray[0].legPoint.y = 0;
    botArray[0].legArray[0].legPoint.z = 0;
    botArray[0].legArray[0].motor.pinStep = 10; // white and brown
    botArray[0].legArray[0].motor.pinDirection = 11; // green 
    botArray[0].legArray[0].motor.pinEnable = 12; // white and green
    botArray[0].legArray[0].motor.stepsPerRevolution = 3200;
    botArray[0].legArray[0].motor.winderCircumference = 3.14159 * 26.18; // 4 turns moves 329mm on climber       
    
    botArray[0].legArray[1].legNumber = 1;
    botArray[0].legArray[1].potPin = A1;
    botArray[0].legArray[1].height = 671;
    botArray[0].legArray[1].climber.topStopToAxis = 35; // measured correct
    botArray[0].legArray[1].climber.bottomStopToAxis = 38; // measured correct
    botArray[0].legArray[1].climber.pointToAxis = 100;
    botArray[0].legArray[1].legPoint.x = 0;
    botArray[0].legArray[1].legPoint.y = 0;
    botArray[0].legArray[1].legPoint.z = 0;
    botArray[0].legArray[1].motor.pinStep = 4; // white and brown
    botArray[0].legArray[1].motor.pinDirection = 5; // green
    botArray[0].legArray[1].motor.pinEnable = 6; // white and green
    botArray[0].legArray[1].motor.stepsPerRevolution = 3200;
    botArray[0].legArray[1].motor.winderCircumference = 3.14159 * 21.12; // 8 turns moves 531mm on climber
    
    botArray[0].legArray[2].legNumber = 2;
    botArray[0].legArray[2].potPin = A2;
    botArray[0].legArray[2].height = 671;
    botArray[0].legArray[2].climber.topStopToAxis = 41; // measured correct
    botArray[0].legArray[2].climber.bottomStopToAxis = 38; // measured correct
    botArray[0].legArray[2].climber.pointToAxis = 100;    
    botArray[0].legArray[2].legPoint.x = 307;
    botArray[0].legArray[2].legPoint.y = 614;
    botArray[0].legArray[2].legPoint.z = 0;
    botArray[0].legArray[2].motor.pinStep = 7; // white and brown
    botArray[0].legArray[2].motor.pinDirection = 8; // green
    botArray[0].legArray[2].motor.pinEnable = 9; // white and green
    botArray[0].legArray[2].motor.stepsPerRevolution = 3200;
    botArray[0].legArray[2].motor.winderCircumference = 3.14159 * 26.18;  // same winder as leg0  
    
    /*
      End of the array of parameters
    */    
        
  } // End of Constructor


