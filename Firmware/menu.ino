 //Function for updating the menu status after pressing the rotary knob
void cursorchange(){
//Depending on the current menu item, turning has different effects
  switch (menuPos){
//Within these menu positions, the cursor position changes depending on the menu size
    case 0: case 1: case 2: case 3: case 4: case 5:{
      if (encoderPos<5-menuSize) {encoderPos=5-menuSize; encoder.setCount((5-menuSize)*2);}
      if (encoderPos>4) {encoderPos=4; encoder.setCount(8);}
      switch (encoderPos){
        // for selection of I_set
        case 1:{
          tft.drawRect(1,1,238,21,0xE700);
          tft.drawRect(2,2,236,19,0xE700);
          tft.drawRect(1,216,68,24,ILI9341_BLACK);
          tft.drawRect(2,217,66,22,ILI9341_BLACK);
        }
        break;
        case 2:{
          tft.drawRect(1,1,238,21,ILI9341_BLACK);
          tft.drawRect(2,2,236,19,ILI9341_BLACK);
          tft.drawRect(1,216,68,24,0xE700);
          tft.drawRect(2,217,66,22,0xE700);
          tft.drawRect(124,216,28,24,ILI9341_BLACK);
          tft.drawRect(125,217,26,22,ILI9341_BLACK);

        }
        break;
        //for selection of cc mode
        case 3:{
          tft.drawRect(1,216,68,24,ILI9341_BLACK);
          tft.drawRect(2,217,66,22,ILI9341_BLACK);
          tft.drawRect(124,216,28,24,0xE700);
          tft.drawRect(125,217,26,22,0xE700);
          tft.drawRect(154,216,28,24,ILI9341_BLACK);
          tft.drawRect(155,217,26,22,ILI9341_BLACK);
          tft.drawRect(187,216,52,24,ILI9341_BLACK);
          tft.drawRect(188,217,50,22,ILI9341_BLACK);
        }
        break;
        //for menu selection
        case 4:{
          tft.drawRect(124,216,28,24,ILI9341_BLACK);
          tft.drawRect(125,217,26,22,ILI9341_BLACK);
          tft.drawRect(154,216,28,24,ILI9341_BLACK);
          tft.drawRect(155,217,26,22,ILI9341_BLACK);
          tft.drawRect(187,216,52,24,0xE700);
          tft.drawRect(188,217,50,22,0xE700);
        }
        break;
        //selection for cv mode
        case 5:{
          tft.drawRect(1,216,68,24,ILI9341_BLACK);
          tft.drawRect(2,217,66,22,ILI9341_BLACK);
          tft.drawRect(154,216,28,24,0xE700);
          tft.drawRect(155,217,26,22,0xE700);
          tft.drawRect(124,216,28,24,ILI9341_BLACK);
          tft.drawRect(125,217,26,22,ILI9341_BLACK);
          tft.drawRect(187,216,52,24,ILI9341_BLACK);
          tft.drawRect(188,217,50,22,ILI9341_BLACK);
        }
        break;
      }
    }
    break;
    case 6: {
      if (encoderPos<1) {encoderPos=1; encoder.setCount(2);}
      if (encoderPos>menuSize) {encoderPos=menuSize; encoder.setCount(menuSize*2);}
      unsigned int cursorposition=24*encoderPos;
      tft.drawRect(1,oldcursorposition-1,318,24,ILI9341_BLACK);
      tft.drawRect(2,oldcursorposition,316,22,ILI9341_BLACK); 
      tft.drawRect(1,cursorposition-1,318,24,0xE700);
      tft.drawRect(2,cursorposition,316,22,0xE700);
      oldcursorposition=cursorposition;}
      break;

    /*case 7: {
      if (encoderPos<0) {encoderPos=5; encoder.setCount(10);}
      if (encoderPos>5) {encoderPos=0; encoder.setCount(0);}
      tft.setCursor(6, 4);
      switch (encoderPos){
        case 0: tft.print("Monitor    ");break;
        case 1: tft.print("CC-Mode    ");break;
        case 2: tft.print("Charge-Mode");break;
        case 3: tft.print("Discharge-M");break;
        case 4: tft.print("Cycle-Mode ");break;
        case 5: tft.print("Pump-Only-M");break;
      }
      break;*/

// A setting value is changed with the rotary encoder within the following menu items
case 61: { // Charge for Purge
    if (encoderPos < 1) { encoderPos = 1; encoder.setCount(2); }
    else if (encoderPos > 200) { encoderPos = 200; encoder.setCount(400); }
    chargeForPurge = encoderPos / 1.0; // Range: 1-200 AS
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    if (chargeForPurge < 10) { tft.setCursor(268, 52); }
    else { tft.setCursor(256, 52); }
    tft.print(chargeForPurge, 1);
    tft.print("AS");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    break;
}
case 62: { // Purge Time
    if (encoderPos < 100) { encoderPos = 100; encoder.setCount(200); }
    else if (encoderPos > 500) { encoderPos = 500; encoder.setCount(1000); }
    purgeTime = encoderPos; // Range: 100-500 ms
    tft.setCursor(256, 76);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.print(purgeTime, 0);
    tft.print("ms");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    break;
}
case 63: { // Temperature for FC
    if (encoderPos < 30) { encoderPos = 30; encoder.setCount(60); }
    else if (encoderPos > 120) { encoderPos = 120; encoder.setCount(240); }
    temperatureFC = encoderPos / 1.0; // Range: 30-120 °C
    tft.setCursor(256, 100);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.print(temperatureFC, 1);
    tft.print("C");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    break;
}
case 64: { // Blower Duty Cycle
    if (encoderPos < 50) { encoderPos = 50; encoder.setCount(100); }
    else if (encoderPos > 100) { encoderPos = 100; encoder.setCount(200); }
    blowerDutyCycle = encoderPos; // Range: 50-100%
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(268, 124);
    if (blowerDutyCycle < 10) { tft.print("  "); }
    else if (blowerDutyCycle < 100) { tft.print(" "); }
    tft.print(blowerDutyCycle);
    tft.print("%");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    break;
}
case 65: { // Blower Current
    if (encoderPos < 0) { encoderPos = 0; encoder.setCount(0); }
    else if (encoderPos > 100) { encoderPos = 100; encoder.setCount(200); }
    blowerCurrent = encoderPos / 10.0; // Range: 0.0-10.0 A
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(268, 148);
    if (blowerCurrent < 10) { tft.print("  "); }
    else if (blowerCurrent < 100) { tft.print(" "); }
    tft.print(blowerCurrent, 1);
    tft.print("A");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    break;
}
    case 66:{ // No of cycles
      if(encoderPos<1){encoderPos=1; encoder.setCount(2);}
      else if (encoderPos>50){encoderPos=50; encoder.setCount(100);}
      numberofcycles=encoderPos;
      tft.setTextColor(ILI9341_BLACK, 0xE700);
      tft.setCursor(292, 172);
      if (numberofcycles<10){tft.print(" ");}
      tft.print(numberofcycles);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
      break;
    case 67:{ // datalog interval
      if(encoderPos<1){encoderPos=1; encoder.setCount(2);}
      else if (encoderPos>300){encoderPos=300; encoder.setCount(600);}
      dataint=encoderPos*1000;
      tft.setTextColor(ILI9341_BLACK, 0xE700);
      tft.setCursor(268, 196);
      if (dataint<10000){tft.print("  ");}
      else if (dataint<100000){tft.print(" ");}
      tft.print(dataint/1000);
      tft.print("s");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
      break;
    case 68:{ // No of cells
      if(encoderPos<1){encoderPos=1; encoder.setCount(2);}
      else if (encoderPos>12){encoderPos=12; encoder.setCount(24);}
      numberofcells=encoderPos;
      tft.setTextColor(ILI9341_BLACK, 0xE700);
      tft.setCursor(292, 220);
      if (numberofcells<10){tft.print(" ");}
      tft.print(numberofcells);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
      break;

//The setpoint current is set in CC mode on the overview display
    case 11:{
      if(encoderPos<-2000){encoderPos=-2000; encoder.setCount(-4000);}
      else if (encoderPos>2000){encoderPos=2000; encoder.setCount(4000);}
      Isoll=encoderPos/100.00;
      tft.setTextColor(ILI9341_BLACK, 0xE700);
      tft.setCursor(6,4);
      if(Isoll>0){tft.print("I_Set:");}
      else{tft.print("I_Set: ");}
      tft.setTextColor(0x0418, 0xE700);
      tft.setCursor(153,4);
      if (Isoll<10 || Isoll>-10){tft.print(" ");}
      if(Isoll<0){tft.print(Isoll*-1, 2);}
      else {tft.print(Isoll, 2);}
      tft.setCursor(215,4);
      tft.print("A");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
      break;

//The pump output is set in PumpMode on the overview display
    case 15:{
      if(encoderPos<0){encoderPos=0; encoder.setCount(0);}
      else if (encoderPos>100){encoderPos=100; encoder.setCount(200);}
      pumppercent=encoderPos;
      tft.setTextColor(ILI9341_BLACK, 0xE700);
      tft.setCursor(6, 124);
      tft.print("P_Pump:");
      tft.setTextColor(0x0418, 0xE700);
      tft.setCursor(177, 124);
      if (pumppercent<10){tft.print("  ");}
      else if(pumppercent<100){tft.print(" ");}
      tft.print(pumppercent);
      tft.setCursor(215, 124);
      tft.print("%");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
      break;
    }
  }



//Function for changing the menu position when the push-button is pressed
void menuchange(){
//Switch to constant current mode
  if(menuPos<6 && encoderPos==3){
    stopall();
    statenum=0;
    encoderPos=menuPos;
    menuPos=7;
    encoder.setCount(encoderPos*2);
    tft.fillRect(124,216,28,24,0xE700);
    tft.setTextColor(ILI9341_BLACK,0xE700);
    tft.setCursor(127, 221);
    tft.println("CC");
    tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);
  }
  else if (menuPos==7){
    Mode=encoderPos;
    menuPos=Mode;
    encoderPos=3;
    encoder.setCount(6);
    menuSize=menuSizes[Mode];
    tft.fillRect(124,216,28,24,ILI9341_BLACK);
    tft.setCursor(127, 221);
    tft.println("CC");
    displayMenu();
  }
//Switch to the Settings submenu
  else if (menuPos<6 && encoderPos==4){
    stopall();
    statenum=0;
    menuPos=6;
    menuSize=9;
    encoderPos=1;
    encoder.setCount(2);
    operation=false;
    tft.fillScreen(ILI9341_BLACK);
    displayMenu();
  }
//Return from the Settings submenu
  else if (menuPos==6 && encoderPos==1){
    menuPos=Mode;
    menuSize=menuSizes[Mode];
    encoderPos=4;
    encoder.setCount(8);
    tft.fillScreen(ILI9341_BLACK);
    displayMenu();
  }
//Activate/deactivate the respective operating mode
  else if (menuPos<6 && encoderPos==2){
    operation=!operation;
    Regelwert=0;
    if (!operation){tft.setCursor(6, 221);tft.println("Start");
      stopall();
      statenum=0;
      digitalWrite(2,HIGH);
      delay(400);
      digitalWrite(2,LOW);
    }
    if (operation){
      //digitalWrite(33, LOW); (always high because connected to MISO pull up)
      //digitalWrite(11, HIGH);
      tft.fillRect(3,218,64,20,ILI9341_BLACK);
      tft.setCursor(12, 221);tft.println("Stop");
      Cmess=0;
      Emess=0;
      nlog=0;
      statenum=1;
      digitalWrite(2,HIGH);
      delay(100);
      digitalWrite(2,LOW);
      setupNewFile();
      if (Mode==2 || Mode==4){
        cyclenum=0;
        charging=true;
        statenum=2;
      }
      if (Mode==3){
        discharging=true;
        statenum=3;
      }
    }
  }

// Select to change the respective setting value
else if (menuPos == 6 && encoderPos == 2) { // Charge for Purge
    menuPos = 61;
    encoderPos = chargeForPurge * 10; // Scale for encoder
    encoder.setCount(chargeForPurge * 20);
    tft.fillRect(1, 47, 318, 24, 0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 52);
    tft.print("Charge for Purge");
    if (chargeForPurge < 10) { tft.setCursor(268, 52); }
    else { tft.setCursor(256, 52); }
    tft.print(chargeForPurge, 1);
    tft.print("AS");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}
else if (menuPos == 6 && encoderPos == 3) { // Purge Time
    menuPos = 62;
    encoderPos = purgeTime; // No scaling needed (100-500 ms)
    encoder.setCount(purgeTime * 2);
    tft.fillRect(1, 71, 318, 24, 0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 76);
    tft.print("Purge Time");
    tft.setCursor(256, 76);
    tft.print(purgeTime, 0);
    tft.print("ms");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}
else if (menuPos == 6 && encoderPos == 4) { // Temperature for FC
    menuPos = 63;
    encoderPos = temperatureFC * 10; // Scale for encoder
    encoder.setCount(temperatureFC * 20);
    tft.fillRect(1, 95, 318, 24, 0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 100);
    tft.print("Temp for FC");
    tft.setCursor(256, 100);
    tft.print(temperatureFC, 1);
    tft.print("C");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}
else if (menuPos == 6 && encoderPos == 5) { // Blower Duty Cycle
    menuPos = 64;
    encoderPos = blowerDutyCycle; // No scaling needed (50-100%)
    encoder.setCount(blowerDutyCycle * 2);
    tft.fillRect(1, 119, 318, 24, 0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 124);
    tft.print("Blower Duty Cycle");
    tft.setCursor(268, 124);
    if (blowerDutyCycle < 10) { tft.print("  "); }
    else if (blowerDutyCycle < 100) { tft.print(" "); }
    tft.print(blowerDutyCycle);
    tft.print("%");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}
else if (menuPos == 6 && encoderPos == 6) { // Blower Current
    menuPos = 65;
    encoderPos = blowerCurrent * 10; // Scale for encoder
    encoder.setCount(blowerCurrent * 20);
    tft.fillRect(1, 143, 318, 24, 0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 148);
    tft.print("Blower Current");
    tft.setCursor(268, 148);
    if (blowerCurrent < 10) { tft.print("  "); }
    else if (blowerCurrent < 100) { tft.print(" "); }
    tft.print(blowerCurrent, 1);
    tft.print("A");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}
else if (menuPos == 6 && encoderPos == 7) { // Number of Cycles
    menuPos = 66;
    encoderPos = numberofcycles; // No scaling needed
    encoder.setCount(numberofcycles * 2);
    tft.fillRect(1, 167, 318, 24, 0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 172);
    tft.print("Number of Cycles");
    tft.setCursor(292, 172);
    if (numberofcycles < 10) { tft.print(" "); }
    tft.print(numberofcycles);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}
else if (menuPos == 6 && encoderPos == 8) { // Datalog Interval
    menuPos = 67;
    encoderPos = dataint / 1000; // Convert to seconds
    encoder.setCount(dataint / 500);
    tft.fillRect(1, 191, 318, 24, 0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 196);
    tft.print("Datalog Interval");
    tft.setCursor(268, 196);
    if (dataint < 10000) { tft.print("  "); }
    else if (dataint < 100000) { tft.print(" "); }
    tft.print(dataint / 1000);
    tft.print("s");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}
else if (menuPos == 6 && encoderPos == 9) { // Number of Cells
    menuPos = 68;
    encoderPos = numberofcells; // No scaling needed
    encoder.setCount(numberofcells * 2);
    tft.fillRect(1, 215, 318, 24, 0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 220);
    tft.print("Number of Cells");
    tft.setCursor(292, 220);
    if (numberofcells < 10) { tft.print(" "); }
    tft.print(numberofcells);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

// Save settings when exiting the submenu
else if (menuPos == 61) { // Charge for Purge
    menuPos = 6;
    preferences.begin("savedsettings", false);
    preferences.putFloat("chargeForPurge", chargeForPurge);
    preferences.end();
    encoderPos = 2;
    encoder.setCount(4);
    tft.fillRect(3, 49, 314, 20, ILI9341_BLACK);
    tft.setCursor(6, 52);
    tft.print("Charge for Purge");
    if (chargeForPurge < 10) { tft.setCursor(268, 52); }
    else { tft.setCursor(256, 52); }
    tft.print(chargeForPurge, 1);
    tft.print("AS");
}
else if (menuPos == 62) { // Purge Time
    menuPos = 6;
    preferences.begin("savedsettings", false);
    preferences.putInt("purgeTime", purgeTime);
    preferences.end();
    encoderPos = 3;
    encoder.setCount(6);
    tft.fillRect(3, 73, 314, 20, ILI9341_BLACK);
    tft.setCursor(6, 76);
    tft.print("Purge Time");
    tft.setCursor(256, 76);
    tft.print(purgeTime, 0);
    tft.print("ms");
}
else if (menuPos == 63) { // Temperature for FC
    menuPos = 6;
    preferences.begin("savedsettings", false);
    preferences.putFloat("temperatureFC", temperatureFC);
    preferences.end();
    encoderPos = 4;
    encoder.setCount(8);
    tft.fillRect(3, 97, 314, 20, ILI9341_BLACK);
    tft.setCursor(6, 100);
    tft.print("Temp for FC");
    tft.setCursor(256, 100);
    tft.print(temperatureFC, 0);
    tft.print("C");
}
else if (menuPos == 64) { // Blower Duty Cycle
    menuPos = 6;
    preferences.begin("savedsettings", false);
    preferences.putInt("blowerDutyCycle", blowerDutyCycle);
    preferences.end();
    encoderPos = 5;
    encoder.setCount(10);
    tft.fillRect(3, 121, 314, 20, ILI9341_BLACK);
    tft.setCursor(6, 124);
    tft.print("Blower Duty Cycle");
    tft.setCursor(268, 124);
    if (blowerDutyCycle < 10) { tft.print("  "); }
    else if (blowerDutyCycle < 100) { tft.print(" "); }
    tft.print(blowerDutyCycle);
    tft.print("%");
}
else if (menuPos == 65) { // Blower Current
    menuPos = 6;
    preferences.begin("savedsettings", false);
    preferences.putFloat("blowerCurrent", blowerCurrent);
    preferences.end();
    encoderPos = 6;
    encoder.setCount(12);
    tft.fillRect(3, 145, 314, 20, ILI9341_BLACK);
    tft.setCursor(6, 148);
    tft.print("Blower Current");
    tft.setCursor(268, 148);
    if (blowerCurrent < 10) { tft.print("  "); }
    else if (blowerCurrent < 100) { tft.print(" "); }
    tft.print(blowerCurrent, 1);
    tft.print("A");
}
  else if (menuPos==66){
    menuPos=6;
    preferences.begin("savedsettings", false);
    preferences.putUInt("numberofcycles", numberofcycles);
    preferences.end();
    encoderPos=7;
    encoder.setCount(14);
    tft.fillRect(3,169,314,20,ILI9341_BLACK);
    tft.setCursor(6, 172);
    tft.print("Number of Cycles"); 
    tft.setCursor(292, 172);
    if (numberofcycles<10){tft.print(" ");}
    tft.print(numberofcycles);
  }
  else if (menuPos==67){
    menuPos=6;
    preferences.begin("savedsettings", false);
    preferences.putUInt("dataint", dataint);
    preferences.end();
    encoderPos=8;
    encoder.setCount(16);
    tft.fillRect(3,193,314,20,ILI9341_BLACK);
    tft.setCursor(6, 196);
    tft.print("Datalog Interval"); 
    tft.setCursor(268, 196);
    if (dataint<10000){tft.print("  ");}
    else if (dataint<100000){tft.print(" ");}
    tft.print(dataint/1000);
    tft.print("s");
  }
  else if (menuPos==68){
    menuPos=6;
    preferences.begin("savedsettings", false);
    preferences.putUInt("numberofcells", numberofcells);
    preferences.end();
    encoderPos=9;
    encoder.setCount(18);
    tft.fillRect(3,217,314,20,ILI9341_BLACK);
    tft.setCursor(6, 220);
    tft.print("Number of Cells"); 
    tft.setCursor(292, 220);
    if (numberofcells<10){tft.print(" ");}
    tft.print(numberofcells);
  }

  else if (menuPos==1 && encoderPos==1){
    menuPos=11;
    encoderPos=Isoll*100;
    encoder.setCount(Isoll*200);
    tft.fillRect(1,1,238,21,0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6,4);
    if(Isoll>0){tft.print("I_Set:");}
    else{tft.print("I_Set: ");}
    tft.setTextColor(0x0418, 0xE700);
    tft.setCursor(153, 4);
    if (Isoll<10 || Isoll>-10){tft.print(" ");}
    if(Isoll<0){tft.print(Isoll*-1, 2);}
    else {tft.print(Isoll, 2);}
    tft.setCursor(215, 4);
    tft.print("A");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  }
  else if (menuPos==11){
    menuPos=1;
    encoderPos=1;
    encoder.setCount(2);
    tft.fillRect(3,4,234,19,ILI9341_BLACK);
    tft.setCursor(6,4);
    if(Isoll>0){tft.print("I_Set:");}
    else{tft.print("I_Set: ");}
    tft.setTextColor(0x0418, ILI9341_BLACK);
    tft.setCursor(153, 4);
    if (Isoll<10 || Isoll>-10){tft.print(" ");}
    if(Isoll<0){tft.print(Isoll*-1, 2);}
    else {tft.print(Isoll, 2);}
    tft.setCursor(215, 4);
    tft.print("A");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  }
  else if (menuPos==5 && encoderPos==1){
    menuPos=15;
    encoderPos=pumppercent;
    encoder.setCount(2*pumppercent);
    tft.fillRect(1,120,238,23,0xE700);
    tft.setTextColor(ILI9341_BLACK, 0xE700);
    tft.setCursor(6, 124);
    tft.print("P_Pump:");
    tft.setTextColor(0x0418, 0xE700);
    tft.setCursor(177, 124);
    if (pumppercent<10){tft.print("  ");}
    else if(pumppercent<100){tft.print(" ");}
    tft.print(pumppercent);
    tft.setCursor(215, 124);
    tft.print("%");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  }
  else if (menuPos==15){
    menuPos=5;
    encoderPos=1;
    encoder.setCount(2);
    tft.fillRect(3,122,234,19,ILI9341_BLACK);
    tft.setCursor(6, 124);
    tft.print("P_Pump:");
    tft.setTextColor(0x0418, ILI9341_BLACK);
    tft.setCursor(177, 124);
    if (pumppercent<10){tft.print("  ");}
    else if(pumppercent<100){tft.print(" ");}
    tft.print(pumppercent);
    tft.setCursor(215, 124);
    tft.print("%");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  }
}

void refreshMenu(){
  if (menuPos<6||menuPos==11||menuPos==15){
//Updated regularly
    /*tft.setTextSize(1);
    tft.setCursor(172, 12);
    tft.print(state[statenum]);
    tft.setCursor(154, 1);
    tft.print(SDstate[SDstatenum]);*/
    tft.setTextSize(2);
    if (!operation){tft.setCursor(6, 221);tft.println("Start");}
    if (operation){tft.fillRect(3,218,64,20,ILI9341_BLACK);tft.setCursor(12, 221);tft.println("Stop");}

  //Current, voltage, temperature, energy display
      tft.setTextColor(0x0700, ILI9341_BLACK);
      tft.setCursor(153, 26); 
      if (Umess>=10){tft.print(Umess, 3);}
      else{tft.print(" ");
      tft.print(Umess, 3);}
      tft.setCursor(227, 26);
      tft.print("V");

      tft.setTextColor(0xE000, ILI9341_BLACK);
      tft.setCursor(153, 44);
      if (Imess<10 && Imess>-10){tft.print(" ");}
      if(Imess<0){tft.print(Imess*-1, 3);}
      else {tft.print(Imess, 3);}
      tft.setCursor(227, 44);
      tft.print("A");

      tft.setTextColor(0x0618, ILI9341_BLACK);
      tft.setCursor(165, 62);
      if (Pmess<100 && Pmess>-100){tft.print(" ");}
      if (Pmess<10 && Pmess>-10){tft.print(" ");}
      if(Pmess<0){tft.print(Pmess*-1, 1);}
      else {tft.print(Pmess, 1);}
      tft.setCursor(227, 62);
      tft.print("W");

      tft.setTextColor(0xE600, ILI9341_BLACK);
      //Charge As range
      tft.setCursor(141, 83);
      if (Cmess<10){
        tft.print(" ");
        tft.print(Cmess, 3);      
        tft.setCursor(215, 83);
        tft.print("As");
      }
      else if (Cmess && Cmess>=10){
        tft.print(" ");
        tft.print(Cmess, 2);      
        tft.setCursor(215, 83);
        tft.print("As");
      }
      else if (Cmess<1000 && Cmess>=100){
        tft.print(" ");
        tft.print(Cmess, 1);      
        tft.setCursor(215, 83);
        tft.print("As");
      }
      /*
      //Charged energy kWh range
      else if (Cmess<10 && Cmess>=1){
        tft.print(Cmess, 3);      
        tft.setCursor(203, 83);
        tft.print("As");
      }
      else if (Cmess<100 && Cmess>=10){
        tft.print(Cmess, 2);      
        tft.setCursor(203, 83);
        tft.print("As");
      }
      else if (Cmess<1000 && Cmess>=100){
        tft.print(Cmess, 1);      
        tft.setCursor(203, 83);
        tft.print("As");
      }
*/
      // energy Wh range
      tft.setCursor(141, 101);
      if (Emess<10){
        tft.print(" ");
        tft.print(Emess, 3);      
        tft.setCursor(215, 101);
        tft.print("Wh");
      }
      else if (Emess<100 && Emess>=10){
        tft.print(Emess, 3);      
        tft.setCursor(215, 101);
        tft.print("Wh");
      }
      else if (Emess<1000 && Emess>=100){
        tft.print(Emess, 2);      
        tft.setCursor(215, 101);
        tft.print("Wh");
      }
      //energy kWh range
      else if (Emess<10 && Emess>=1){
        tft.print(Emess, 3);      
        tft.setCursor(203, 101);
        tft.print("kWh");
      }
      else if (Emess<100 && Emess>=10){
        tft.print(Emess, 2);      
        tft.setCursor(203, 101);
        tft.print("kWh");
      }
      else if (Emess<1000 && Emess>=100){
        tft.print(Emess, 1);      
        tft.setCursor(203, 101);
        tft.print("kWh");
      }

      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setCursor(68, 148);
      if (T_CS>=100){tft.print("--");}
      else{if (T_CS<10){tft.print(" ");}
      tft.print(T_CS, 0);
      }

      tft.setCursor(6, 44);
      tft.print("I_FC: ");
      tft.setCursor(6, 62);
      tft.print("P_FC: ");

  //Display of cell voltages
      for(int q=0; q<numberofcells; q++){
        tft.setCursor(246, q*20 + 2);
        if (q==CVData[0] && CVData[2]==1){
          tft.setTextColor(0x0700, ILI9341_BLACK);
        }
        else if (q==CVData[1] && CVData[3]==1){
          tft.setTextColor(0xE000, ILI9341_BLACK);
        }
        tft.print(Ucell[q], 3);
        tft.setCursor(308, q*20 + 2);
        tft.print("V");                  
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
  //Display of the minimum and maximum cell voltage per charge/discharge process
    if(menuPos==2 || menuPos==3 || menuPos==4){
      tft.setCursor(6, 171);
      if(Imess>0){tft.print("U_CVmin:");
      tft.setCursor(177, 171);
      tft.print(mincellvoltage, 2);}
      else{tft.print("U_CVmax:");
      tft.setCursor(177, 171);
      tft.print(maxcellvoltage, 2);}
      tft.setCursor(227, 171);
      tft.print("V");
    }
    else {tft.fillRect(6,171,233,16,ILI9341_BLACK);}
  //Display of the time
    if (operation){
      tft.setTextColor(0xE01F, ILI9341_BLACK);
      tft.setCursor(6, 194);
      if (hourss<10){tft.print("00");}
      else if (hourss<100){tft.print("0");}
      tft.print(hourss);
      tft.print(":");
      if (minutess<10){tft.print("0");}
      tft.print(minutess);
      tft.print(":");
      if (secondss<10){tft.print("0");}
      tft.print(secondss);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    }
    else {
      tft.setTextColor(0xE01F, ILI9341_BLACK);
      tft.setCursor(6, 194);
      tft.print("00:00:00");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    }
  //Display of the pump output
      tft.setCursor(189, 121); 
      if (round(pumppowerlast*0.390625)<10){tft.print("  ");}
      else if (round(pumppowerlast*0.390625)<100){tft.print(" ");}
      tft.print(round(pumppowerlast*0.390625), 0);
    if (menuPos==1){
      tft.setCursor(6,4);
      if(Isoll>0){tft.print("I_Set:");}
      else{tft.print("I_Set: ");}
      tft.setTextColor(0x0418, ILI9341_BLACK);
      tft.setCursor(153,4);
      if (Isoll<10 || Isoll>-10){tft.print(" ");}
      if(Isoll<0){tft.print(Isoll*-1, 2);}
      else {tft.print(Isoll, 2);}
      tft.setCursor(215,4);
      tft.println("A");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
    else if (menuPos==5) {
      tft.setCursor(6, 124);
      tft.print("P_Pump:");
      tft.setTextColor(0x0418, ILI9341_BLACK);
      tft.setCursor(177, 124);
      if (pumppercent<10){tft.print("  ");}
      else if(pumppercent<100){tft.print(" ");}
      tft.print(pumppercent);
      tft.setCursor(215, 124);
      tft.print("%");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
    else if (menuPos==11 || menuPos==15){}
    else if (Mode==2 || Mode==3 || Mode==4){
      tft.setCursor(6, 124);
      if (discharging){tft.print("I_DC:   ");}
      else if (charging){tft.print("I_C:    ");}
      else {tft.print("I_CDC:  ");}
      if (Mode==4){
        tft.setCursor(215, 148); 
        if (cyclenum-1<10){tft.print(" ");}
        if (cyclenum<1){tft.print("0");}
        else {tft.print(cyclenum-1);}
      }
    }
    //else {tft.fillRect(6,124,233,16,ILI9341_BLACK);} (problem with displaying T_FC)
  }
}

//Function for updating the elements shown on the display
void displayMenu(){
  refreshMenu();
  cursorchange();
//Elements on the display overview
    if(menuPos<6){
//Run once main page
      if (!operation){tft.setCursor(6, 221);tft.println("Start");}
      if (operation){tft.fillRect(3,218,64,20,ILI9341_BLACK);tft.setCursor(12, 221);;tft.println("Stop");}

      tft.setCursor(6, 26);
      tft.print("V_FC:");
      tft.setCursor(6, 83);
      tft.print("C_FC: ");
      tft.setCursor(6, 101);
      tft.print("E_FC: ");
      
      tft.setCursor(6, 121);
      tft.print("T_FC:");
      tft.drawCircle(98, 124, 3, ILI9341_WHITE);
      tft.setCursor(104, 121);
      tft.print("C");

      tft.setCursor(6, 148);
      tft.print("T_CS:");
      tft.drawCircle(98, 151, 3, ILI9341_WHITE);
      tft.setCursor(104, 148);
      tft.print("C");

      tft.setCursor(126, 121);
      tft.print("Pump:");
      //tft.setCursor(210, 121);
      //tft.print("0");
      //tft.setCursor(227, 121);
      //print("%");

      tft.setCursor(126, 148);
      tft.print("Air:");
      tft.setCursor(210, 148);
      tft.print("0");
      tft.setCursor(227, 148);
      tft.print("%");
       

      //tft.setCursor(126, 194);
      //tft.print("I_set:");
      //tft.setCursor(227, 194);
      //tft.print("A");
      tft.setCursor(75, 221);
      tft.println("Mode");
      tft.setCursor(127, 221);
      tft.println("CC");
      tft.setCursor(157, 221);
      tft.println("CV");
      tft.setCursor(190, 221);
      tft.println("Menu");
  //Grid
      // Draw horizontal grid lines
  tft.fillRect(0, 22, 240, 1, ILI9341_WHITE);
  tft.fillRect(0, 80, 240, 1, ILI9341_WHITE);
  tft.fillRect(0, 118, 240, 1, ILI9341_WHITE);
  //tft.fillRect(0, 144, 240, 1, ILI9341_WHITE);
  tft.fillRect(0, 166, 240, 1, ILI9341_WHITE);
  tft.fillRect(0, 189, 240, 1, ILI9341_WHITE);

  // Draw vertical grid lines
  tft.fillRect(120, 118, 1, 48, ILI9341_WHITE); // Between sections
  tft.fillRect(120, 189, 1, 25, ILI9341_WHITE); // Bottom section

  tft.fillRect(69, 215, 2, 25, ILI9341_WHITE);  // Separator for "Mode"
  tft.fillRect(184, 215, 2, 25, ILI9341_WHITE); // Separator for "Settings"

  // Bottom separator
  tft.fillRect(0, 213, 240, 2, ILI9341_WHITE);

  // Right boundary
  tft.fillRect(240, 0, 2, 240, ILI9341_WHITE);
    
//Elements specific to the CCMode
      if (Mode==1){
        tft.setCursor(6, 4);  
        tft.print("CC-Mode    ");
      }
/*//Elements specific to the CycleMode
      else if (Mode==4){
        tft.setCursor(6, 4);  
        tft.print("Cycle-Mode ");
        tft.setCursor(126, 148);
        tft.print("Cycles:");
      }
//Elements specific to the ChargeMode
      else if (Mode==2){
        tft.setCursor(6, 4);  
        tft.print("Charge-Mode");
      }
//Elements specific to the DischargeMode
      else if (Mode==3){
        tft.setCursor(6, 4);  
        tft.print("Discharge-M");
      }
//Elements specific to the PumpMode
      else if (Mode==5){
        tft.setCursor(6, 4);  
        tft.print("Pump-Only-M");
        tft.setCursor(6, 124);
        tft.print("P_Pump:");
        tft.setTextColor(0x0418, ILI9341_BLACK);
        tft.setCursor(153, 124);
        if (pumppercent<10){tft.print("  ");}
        else if(pumppercent<100){tft.print(" ");}
        tft.print(pumppercent);
        tft.setCursor(215, 124);
        tft.print("%");
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setCursor(179, 121);
        if (pumppercent<10){tft.print("  ");}
        else if(pumppercent<100){tft.print(" ");}
        tft.print(pumppercent);
        tft.println("%");
      }*/
//Display overview when no operating mode is active
      else {
        tft.setCursor(6, 4);
        tft.println("Monitor    ");
      }
      /*if (Mode==2 || Mode==3 || Mode==4){
        tft.setTextColor(0x0418, ILI9341_BLACK);
        tft.setCursor(153, 124);
        if (cdccurrent<10){tft.print(" ");}
        tft.print(cdccurrent, 2);
        tft.setCursor(215, 124);
        tft.print("A");
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }*/
     // if (Mode<6 && Mode!=4){
      //  tft.fillRect(126, 148,114,16,ILI9341_BLACK); (problem with displaying blower)
     // }
    }
// Display of the Settings submenu
else if (menuPos == 6) {
    tft.fillRect(0, 19, 320, 2, ILI9341_WHITE); // Clear the menu bar
    tft.setCursor(0, 0);  
    tft.println("Menu"); // Display the menu title

    // Back option
    tft.setCursor(6, 28);
    tft.print("Back");

    // Charge for Purge
    tft.setCursor(6, 52);
    tft.print("Charge for Purge");
    if (chargeForPurge < 10) { tft.setCursor(274, 52); } // Adjust cursor for alignment
    else { tft.setCursor(270, 52); }
    tft.print(chargeForPurge, 0); // Display the value
    tft.print("As"); // Unit (Coulombs or another unit as per your requirement)

    // Purge Time
    tft.setCursor(6, 76);
    tft.print("Purge Time");
    tft.setCursor(256, 76);
    tft.print(purgeTime, 0); // Display the value
    tft.print("ms"); // Unit (seconds)

    // Temperature for FC
    tft.setCursor(6, 100);
    tft.print("Temp for FC");
    tft.setCursor(266, 100);
    tft.print(temperatureFC, 0); // Display the value
    //tft.drawCircle(263, 100, 3, ILI9341_WHITE);
    tft.print((char)247);
    tft.print("C"); // Unit (Celsius)

    // Blower Duty Cycle
    tft.setCursor(6, 124);
    tft.print("Blower Duty Cycle");
    tft.setCursor(268, 124);
    if (blowerDutyCycle < 10) { tft.print("  "); } // Adjust alignment for single-digit values
    else if (blowerDutyCycle < 100) { tft.print(" "); }
    tft.print(blowerDutyCycle); // Display the value
    tft.print("%"); // Unit (percentage)

    // Blower Current
    tft.setCursor(6, 148);
    tft.print("Blower Current");
    tft.setCursor(268, 148);
    if (blowerCurrent < 10) { tft.print("  "); } // Adjust alignment for single-digit values
    else if (blowerCurrent < 100) { tft.print(" "); }
    tft.print(blowerCurrent); // Display the value
    tft.print("A"); // Unit (Amps)

    // Number of Cycles
    tft.setCursor(6, 172);
    tft.print("Number of Cycles");
    tft.setCursor(292, 172);
    if (numberofcycles < 10) { tft.print(" "); } // Adjust alignment for single-digit values
    tft.print(numberofcycles);

    // Datalog Interval
    tft.setCursor(6, 196);
    tft.print("Datalog Interval");
    tft.setCursor(268, 196);
    if (dataint < 10000) { tft.print("  "); } // Adjust alignment for smaller values
    else if (dataint < 100000) { tft.print(" "); }
    tft.print(dataint / 1000); // Display the value in seconds
    tft.print("s"); // Unit (seconds)

    // Number of Cells
    tft.setCursor(6, 220);
    tft.print("Number of Cells");
    tft.setCursor(292, 220);
    if (numberofcells < 10) { tft.print(" "); } // Adjust alignment for single-digit values
    tft.print(numberofcells);
}
}
