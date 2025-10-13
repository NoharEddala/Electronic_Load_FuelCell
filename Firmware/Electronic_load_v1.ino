/*  
 *  Software for the Electronic Load at ThaiGer H2 Racing (Stralsund University of Applied Sciences)
 * As part of a Master thesis entitled:
 * “Development and optimization of an electronic load 
    with built in controls for fuel cell characterization”
 *   
 *  Microcontroller: ESP32-Piko-D4
 *  Creator: Nohar Chandrakanth Eddala
 *  Version: 2.3.4
 *  Datum: 03.02.2025
 *  Arduino version: 2.3.4 
    Note: Please use ESP32 board version 2.0.17 only!!
 */

//Einbindung der verwendeten Software-Bibliotheken
#include <ESP32Encoder.h>
#include "Wire.h"
#include "Adafruit_MCP4725.h"
#include "FS.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Arduino.h"
#include <Preferences.h>
#include <SD.h>

//Pinbelegung des ILI9341-Displays
#define _dc 10
#define _cs 5
#define _miso 19         
#define _mosi 23           
#define _sclk 18
#define _rst 27

#define NTC_PIN 39          // NTC pin
#define FAN_PIN 4           // Fan control pin
#define SERIES_RES 10000.0  // 10k series resistor
#define VCC 3.3             // Supply voltage

//Pinbelegung des Encoders
#define Button 35


//Festlegung Aufrufstrukturen
ESP32Encoder encoder;
Adafruit_MCP4725 dacS;
Adafruit_MCP4725 dacL;
Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _rst);
Preferences preferences;
SPIClass * hspi = NULL;

//Pinbelegung Pumpen für PWM
const int pump = 0;
//Variablen für die Pumpenfunktion
const int freq = 500;
const int PWMChannel = 0;
const int resolution = 8;

int pumppower=0;
int pumppercent=0;
int pumppowerlast=0;

// Steinhart-Hart coefficients for 10k NTC
const float A = 0.001129148;
const float B = 0.000234125;
const float C = 0.0000000876741;

// Fan control parameters
const int MIN_SPEED = 128;   // 50% duty cycle (128/255)
const int MAX_SPEED = 255;   // 100% duty cycle
const float MIN_TEMP = 30.0; // Start increasing fan at 30°C
const float MAX_TEMP = 60.0; // Full speed at 60°C

//Variablen für die Energiemessung und Regelung
float Imess=0;
float ImessRow[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int ImessCount=0;
int loadTemperature=0;
float T_CS=0;
float T_FC=0;
float Umess=0;
float ImessOld=0;
float UmessOld;
float Pmess=0;
float Cmess=0;
float Emess=0;
long Energy=0;
float rShunt=0.002;
float Isoll = 0;
float Regelwert = 0;
//Array mit den Zellspannungen der Einzelspannungsmessung
float Ucell[12];
//Array mit Informationen über min und max Einzelsapnnungen
int CVData[4];
float Uav = 0;

//Zustandsvariablen der möglichen Betriebsmodi
int Mode=0;
int menuSizes[6]={2,4,3,3,3,4};
//Zustandsvariablen für den Ablauf der Betriebsmodi
bool charging=true;
bool discharging=false;
bool done=false;
//char* state[]={"       idle","    running","   charging","discharging","       done"," aborted OT"," aborted OL"};
int statenum=0;
//char* SDstate[]={"  SD no status","    SD running","     SD failed"};
int SDstatenum=0;
bool operation=false;
int cyclenum=0;

//Variable zum Auslösen einer Displayaktualisierung
bool updatemenu=true;
//Variablen zur Definition der Menüposition
int encoderPos=3;
int menuPos=0;
int menuSize=2;
int oldcursorposition=26;

//Variablen zur Überwachung der Zeit
unsigned long TimeOld=0;
unsigned long currentTime=0;
unsigned long totalTime=0;
unsigned long totalTimeLast=0;
unsigned long timerP=0;
int secondss=0;
int minutess=0;
int hourss=0;
int nlog=0;

//Variablen der Einstellwerte zur Speicherung im Flash Speicher
unsigned int maxpumppercent=0;
unsigned int minpumppercent=0;
unsigned int numberofcells=0;
float mincellvoltage=0;
float maxcellvoltage=0;
float cdccurrent=0;
unsigned int numberofcycles=0;
unsigned int dataint = 4000;

// Declare new variables for the updated parameters
int chargeForPurge = 0;      // Charge for Purge (1-200 AS)
int purgeTime = 100;             // Purge Time (100-500 ms)
int temperatureFC = 30;      // Temperature for FC (30-120 °C)
int blowerDutyCycle = 50;        // Blower Duty Cycle (50-100%)
int blowerCurrent = 0;       // Blower Current (0.0-10.0 A)

//Variablen zur Datenspeicherung auf der SD-Karte
unsigned long initime=0;
String messdaten = "";
int datacounter = 1;
String dataname = "";
unsigned long lastmeasurement=0;

//Vorgezogene Deklaration der Funktionen aus menu.ino Datei
void displayMenu(void);
void refreshMenu(void);
void cursorchange(void);
void menuchange(void);

//Variablen zur Kommunikation mit dem LTC6803-3 der Einzelspannungsmessung
byte LTC6803_DATA[18];
uint16_t cv[12];
byte PEC;
const int LTC6803_SS = 15;



/* Variablen zur Einstellung des periodischen Purgens */
#define PURGE_STEPSIZE_MS (1000) /* Purge State-Machine Sampling Time in ms */
#define PURGE_STEPS_ON    (1)    /* Number of ON-States (1 ON-State = Sampling Time) */
#define PURGE_STEPS_OFF   (5)    /* Number of OFF-States (1 OFF-State = Sampling Time) */
uint8_t purgeActive = false;
uint8_t purgeStepCounter = 0;
uint64_t purgeTimeStamp = 0;

//Funktion zur Befehlsübertragung an den INA233
static void writeRegister(byte reg, word value) {
  Wire.beginTransmission(0x41);//Adresse
  Wire.write(reg);
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

 void setADCRange() {
  Wire.beginTransmission(0x41); // INA228 Address
  Wire.write(0x00); // CONFIG Register Address

  // Read current CONFIG register value (optional, if you need to preserve other settings)
  Wire.endTransmission(false);
  Wire.requestFrom(0x41, 2);
  uint16_t configValue = (Wire.read() << 8) | Wire.read(); // Read existing value

  // Set ADCRANGE bit (Bit 4)
  configValue |= (1 << 4); // Set bit 4 to 1

  // Write back the modified CONFIG register value
  Wire.beginTransmission(0x41);
  Wire.write(0x00); // CONFIG Register Address
  Wire.write((configValue >> 8) & 0xFF); // Write MSB
  Wire.write(configValue & 0xFF); // Write LSB
  Wire.endTransmission();
}

void setShuntCalibration() {
  // Calculate CURRENT_LSB
  float MAX_EXPECTED_CURRENT = 159.844; // Max measurable current in Amps
  float CURRENT_LSB = MAX_EXPECTED_CURRENT / 524288.0; // 2^19 = 524288
  
  // Calculate SHUNT_CAL
  uint16_t SHUNT_CAL = (uint16_t)(13107.2e6 * CURRENT_LSB * 0.00025); // 0.25 mΩ shunt

  Wire.beginTransmission(0x41);
  Wire.write(0x02);  // SHUNT_CAL Register
  Wire.write((SHUNT_CAL >> 8) & 0xFF);  // High byte
  Wire.write(SHUNT_CAL & 0xFF);         // Low byte
  Wire.endTransmission();
}

void getShuntCurrent_A() {
  int32_t res = 0; // 32-bit signed integer for current register value

  Wire.beginTransmission(0x41); // INA228 Address
  Wire.write(0x07); // Current Register Address
  if (Wire.endTransmission() == 0) {
    if (Wire.requestFrom(0x41, 3) == 3) { // Read exactly 3 bytes
      res |= (Wire.read() << 16); // Read MSB
      res |= (Wire.read() << 8); // Read Mid Byte
      res |= Wire.read();        // Read LSB
    }
  }
  res >>= 2;
  // Check if the value is negative (bit 19 is set)
  if (res & 0x80000) {
    res |= 0xFFF00000;
  } 
    // Calculate CURRENT_LSB based on the maximum expected current
    float MAX_EXPECTED_CURRENT = 159.844; // 163.84 Adjust this value based on your setup
    float CURRENT_LSB = MAX_EXPECTED_CURRENT / 524288.0; // 2^19 = 524288

    // Convert register value to Amperes
    Imess = ((float)res) * CURRENT_LSB;
    if (Imess < 0) Imess = 0.0;
}

void getEnergy_Wh() {
  uint64_t res = 0; // 40-bit Energy register value

  Wire.beginTransmission(0x41); // INA228 Address
  Wire.write(0x09); // Energy Register Address
  if (Wire.endTransmission() == 0) {
    if (Wire.requestFrom(0x41, 5) == 5) { // Ensure we receive exactly 5 bytes
      res |= ((uint64_t)Wire.read() << 32); // Read MSB
      res |= ((uint64_t)Wire.read() << 24);
      res |= ((uint64_t)Wire.read() << 16);
      res |= ((uint64_t)Wire.read() << 8);
      res |= Wire.read(); // Read LSB
    }
  }

  // Calculate CURRENT_LSB based on the maximum expected current
  float MAX_EXPECTED_CURRENT = 163.84; // Max measurable current in Amps
  float CURRENT_LSB = MAX_EXPECTED_CURRENT / 524288.0; // 2^19 = 524288

  // Convert raw register value to Joules
  float energy_J = 16 * 3.2 * CURRENT_LSB * res;
  
  // Convert Joules to Watt-hours
  Emess = energy_J / 3600.0;
}

void getCharge_As() {
  int64_t res = 0; // 40-bit Charge register value

  Wire.beginTransmission(0x41); // INA228 Address
  Wire.write(0x0A); // Charge Register Address
  if (Wire.endTransmission() == 0) {
    if (Wire.requestFrom(0x41, 5) == 5) { // Ensure we receive exactly 5 bytes
      res |= ((int64_t)Wire.read() << 32); // Read MSB
      res |= ((int64_t)Wire.read() << 24);
      res |= ((int64_t)Wire.read() << 16);
      res |= ((int64_t)Wire.read() << 8);
      res |= Wire.read(); // Read LSB
    }
  }

  // Handle Two's Complement (Sign Extension for 40-bit values)
  if (res & ((int64_t)1 << 39)) { // If bit 39 is set (negative value)
    res |= ((int64_t)0xFFFFFF << 40); // Sign extend to 64-bit integer
  }

  // CURRENT_LSB Calculation based on Maximum Expected Current
  float MAX_EXPECTED_CURRENT = 163.84; // Adjust according to your application
  float CURRENT_LSB = MAX_EXPECTED_CURRENT / 524288.0; // 2^19 = 524288

  // Convert raw charge register value to Ampere-seconds (As)
  Cmess = CURRENT_LSB * res;
}

//Function for reading the total voltage of the RFB from the INA228
void getBusVoltage_V() {
  uint64_t res = 0;
  Wire.beginTransmission(0x41);//Adresse
  Wire.write(0x05);
  if (Wire.endTransmission() == 0) {
    if (Wire.requestFrom(0x41, 3) >= 2) {
      res |= Wire.read() << 16;
      res |= Wire.read() << 8;
      res |= Wire.read();
    }
  }
  res >>= 4;
  Umess=((float)res) / 5120.0;
}

// temperature measurement for cooling fans
float readTemp() {
  int adc = analogRead(NTC_PIN);
  float voltage = (adc / 4095.0) * VCC;
  float resistance = SERIES_RES * (voltage / (VCC - voltage));
  
  float logR = log(resistance);
  float tempK = 1.0 / (A + B*logR + C*logR*logR*logR);
  return tempK - 273.15;  // Convert to Celsius
}

void controlFan(float T_CS) {
  int fanSpeed;
  
  if (T_CS <= MIN_TEMP) {
    fanSpeed = MIN_SPEED; // Minimum speed below 30°C
  } 
  else if (T_CS >= MAX_TEMP) {
    fanSpeed = MAX_SPEED; // Maximum speed above 60°C
  } 
  else {
    // Linear speed increase between 30-60°C
    fanSpeed = map(T_CS * 100, MIN_TEMP * 100, MAX_TEMP * 100, MIN_SPEED, MAX_SPEED);
  }
  
  ledcWrite(0, fanSpeed);
  
  Serial.print("T_CS: ");
  Serial.print(T_CS);
  Serial.print("°C | Fan: ");
  Serial.print((fanSpeed * 100) / 255); // Percentage
  Serial.println("%");
}

//Funktion zur Auslese der Einzelspannungen aus dem LTC6803-3
void LTC6803_READCVS(){
//Konfiguration der SPI Kommunikationsregeln
  hspi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(LTC6803_SS, LOW);
  hspi->transfer(0x10);
  hspi->transfer(0xB0);
  digitalWrite(LTC6803_SS, HIGH);
  hspi->endTransaction();
//Zeitverzögerung nötig für die Auslese der Einzelspannungen mit dem ADC des LTC6803-3
  delay(20);
//Abrufen der Messdaten
  hspi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(LTC6803_SS, LOW);
  hspi->transfer(0x04);
  hspi->transfer(0xDC);
  for (int i=0; i<18; i++){
    LTC6803_DATA[i]=hspi->transfer(0x00);
  }
  PEC=hspi->transfer(0x00);
  digitalWrite(LTC6803_SS, HIGH);
  hspi->endTransaction();
//Überprüfen, ob Daten korrekt empfangen wurden
  if(LTC6803_DATA[0] == 0xFF & LTC6803_DATA[1] == 0xFF){
    //Serial.println("no data recieved");
    LTC6803_INIT();
  }
//Wenn keine Daten empfangen: Neue Initialisierung des LTC6803-3
  else if(PEC == 0xFF){
    //Serial.println("no PEC recieved");
    LTC6803_INIT();
  }
//Verrechnung der empfangenen Bits in Einzelspannungen
  else {
    int data_counter = 0;
    uint16_t part1;
    uint16_t part2;
    for(int k=0; k<12; k=k+2){
      part1 = LTC6803_DATA[data_counter++];
      part2 = (uint16_t)(LTC6803_DATA[data_counter]&0x0F)<<8;
      cv[k] = part1 + part2 - 512;
      Ucell[k]=cv[k]*0.0015;
      if(Ucell[k]>50){Ucell[k]=0;}
      part2 = (LTC6803_DATA[data_counter++])>>4;
      part1 = (LTC6803_DATA[data_counter++])<<4;
      cv[k+1] = part1 + part2 - 512;
      Ucell[k+1]=cv[k+1]*0.0015;
      if(Ucell[k+1]>50){Ucell[k+1]=0;}
    }
    CVData[0]=0;
    CVData[1]=0;
    CVData[2]=0;
    CVData[3]=0;
    for (int ii=0; ii<numberofcells; ii++){
      if (Ucell[CVData[0]]<Ucell[ii]){
        CVData[0]=ii;
      }
      if (Ucell[CVData[1]]>Ucell[ii]){
        CVData[1]=ii;
      }
    }
    for (int jj=0; jj<numberofcells; jj++){
      if (Ucell[CVData[0]]==Ucell[jj]){
        CVData[2]+=1;
      }
      else if(Ucell[CVData[1]]==Ucell[jj]){
        CVData[3]+=1;
      }
    }
    //Berechnung der durchschnittlichen Zellspannung
    Uav = 0;
    for (int p=0; p<numberofcells; p++){
      Uav+=Ucell[p];}
    Uav = Uav/numberofcells;
  } 
}

//Funktion zur Initialisierung und Konfiguration (Messmodus) des LTC6803-3
void LTC6803_INIT(){
  hspi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(LTC6803_SS, LOW);
  hspi->transfer(0x01);
  hspi->transfer(0xC7);
  hspi->transfer(0xF1);
  hspi->transfer(0x00);
  hspi->transfer(0x00);
  hspi->transfer(0x00);
  hspi->transfer(0x00);
  hspi->transfer(0x00);
  hspi->transfer(0x49);
  digitalWrite(LTC6803_SS, HIGH);
  hspi->endTransaction();
}

//Einmalig auszuführende Befehle beim Start des ESP32
void setup() {
  pinMode(NTC_PIN, INPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  digitalWrite(26, LOW);
  digitalWrite(25, HIGH);
//Setup der SPI-Schnittstelle zur Einzelspannungsmessung LTC6803-3
  hspi = new SPIClass(HSPI);
  //pinMode(2, OUTPUT);
  pinMode(LTC6803_SS, OUTPUT);
  digitalWrite(LTC6803_SS, HIGH);
  hspi->begin();
//Interner Pullup als Ersatz zur Problemlösung des Externen Pullups
  pinMode(12, INPUT_PULLUP);
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);
//Initialisierung und Auslesen der Einzelsapnnungen
  LTC6803_INIT();
  LTC6803_READCVS();

//Konfiguration der Pins des Bedienknopfes 
  encoder.attachHalfQuad(38, 34);
  encoder.setCount(3);
  pinMode(Button, INPUT);

//Konfiguration der Kommunikation mit dem Anzeigedisplay auf 60MHz
  tft.begin(60000000);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setRotation(3);
  tft.setTextSize(2);

//Einrichten der I2C Kommunikation mit den DACs MCP4725
  Wire.begin();
  dacS.begin(0x60);
  dacL.begin(0x61);
  dacS.setVoltage(0, false);
  dacL.setVoltage(0, false);
  //writeRegister(0xD0, 0x4527);

//Setup der zwei PWM Ausgänge (these are old api, have to replace with new api until then commenting 0r use esp32 older version 2.x.x)
 
  ledcSetup(PWMChannel, freq, resolution);
  ledcAttachPin(pump, PWMChannel);
  ledcWrite(PWMChannel, 255);

  // Set up PWM for fan control
  ledcSetup(0, 25000, 8);  // Channel 0, 25kHz, 8-bit resolution
  ledcAttachPin(FAN_PIN, 0);
  ledcWrite(0, MIN_SPEED); // Start with minimum fan speed

//Laden der gespeicherten Settings aus dem Flash Speicher in die vorges. Variablen
  preferences.begin("savedsettings", false);
  maxpumppercent = preferences.getUInt("maxpumpper", 100);
  minpumppercent = preferences.getUInt("minpumpper", 40);
  numberofcells = preferences.getUInt("numberofcells", 5);
  numberofcycles=preferences.getUInt("numberofcycles", 10);
  cdccurrent=preferences.getFloat("cdccurrent", 2.2);
  maxcellvoltage = preferences.getFloat("maxcellvoltage", 1.55);
  mincellvoltage = preferences.getFloat("mincellvoltage", 0.55);
  dataint=preferences.getUInt("dataint", 60000);
  preferences.end();
 
Serial.begin(115200);

//Anzeige des Startbidschirms
  displayMenu();
}

//Regelmäßig zu widerholende Aktionen
void loop() {
  int encoderPosN=encoder.getCount()/2;
  if (encoderPosN != encoderPos){
    encoderPos=encoderPosN;
    cursorchange();
    Serial.println(encoderPosN);
  }
  
//Abfrage der Strom und Spannungswerte
    getBusVoltage_V();
    delay(1);
    getShuntCurrent_A();
    delay(1);
    Pmess=Umess*Imess;

//Regelung des Stroms im CCMode
   if(Mode==1 && operation){
      if(Isoll > 0){
        Regelwert=(Isoll-Imess)*0.5+Regelwert;
        if (Regelwert<=0)Regelwert=0;                                   
        if (Regelwert>=4096)Regelwert=4096;+
        dacS.setVoltage(0, false);
        delay(1);
        dacL.setVoltage(Regelwert, false);
        delay(1);
      }
      else if(Isoll < 0){
        Regelwert=(Isoll-Imess)*0.5+Regelwert;
        if (Regelwert>=0)Regelwert=0;
        if (Regelwert<=-4096)Regelwert=-4096;
        dacL.setVoltage(0, false);
        delay(1);
        dacS.setVoltage(-Regelwert, false);
        delay(1);
      }
      else {
        Regelwert=0;
        dacS.setVoltage(0, false);
        delay(1);
        dacL.setVoltage(0, false);
        delay(1);
      }
    }

/*//Betriebsabläufe
  if (operation){
//Ladebetrieb    
    if(charging){
      for(int w=0; w<numberofcells; w++){
        if(Ucell[w]>=maxcellvoltage){
          charging=false;
          pumppower=0;
          if(Mode==4){
            discharging=true;
            statenum=3;
            digitalWrite(2,HIGH);
            delay(100);
            digitalWrite(2,LOW);
            delay(100);
            digitalWrite(2,HIGH);
            delay(100);
            digitalWrite(2,LOW);
            if (cyclenum==0){Cmess=0;}
            cyclenum+=1;
            if(cyclenum>numberofcycles){
              discharging=false;
              statenum=4;
            }
          }
          if (Mode==2){statenum=4;}
        }
      }
    }
    else if(discharging){
      for(int o=0; o<numberofcells; o++){
        if(Ucell[o]<=mincellvoltage){
          discharging=false;
          pumppower=0;
          if(Mode==4){
            charging=true;
            statenum=2;
            digitalWrite(2,HIGH);
            delay(100);
            digitalWrite(2,LOW);
            delay(100);
            digitalWrite(2,HIGH);
            delay(100);
            digitalWrite(2,LOW);
          }
          if (Mode==3){statenum=4;}
        }
      }
    }
  }
  */
//Aktualisierung der Anzeigewerte und der Einzelspannungen alle 0,5 Sekunden
  if(millis() - totalTime >= 500){
    totalTimeLast=totalTime;
    totalTime=millis();
    //T_CS=0.16*analogRead(36);
    //T_CS = getTemperature();
      T_CS = readTemp();
  if (T_CS > -50) { // Basic error check (ignore impossible values)
    controlFan(T_CS);
  }


    if (operation){
      currentTime=totalTime-initime;
      hourss=currentTime/(1000*60*60);
      minutess=(currentTime-(hourss*(1000*60*60)))/(1000*60);
      secondss=((currentTime-(hourss*(1000*60*60)))-(minutess*(1000*60)))/1000;
      float Energy=((((ImessOld*UmessOld)+((Imess)*Umess))/2)*(totalTime-totalTimeLast))/1000;
      if(Energy>0){
        Emess+=Energy;
      }
      if(Energy<0){
        Cmess+=(Energy*(-1));
      }
      Energy=0;
      ImessOld=Imess;
      UmessOld=Umess;
    }    
    LTC6803_READCVS();
    delay(2);
    if (menuPos<6 || menuPos==11 || menuPos==15){
      refreshMenu();
      }

    if((Mode==1 && !operation) || ((Mode==2 || Mode==3 || Mode==4) && operation && !(discharging || charging))){
      Regelwert=0;
      dacS.setVoltage(0, false);
      delay(1);
      dacL.setVoltage(0, false);
      delay(1);
    }
  }
  
/*//Loggen der Daten im aktiven Betrieb sobald Datenintervall erreicht ist
  if(operation){
    int processtime=(nlog-1)*dataint+initime;
    if (millis()-processtime>dataint){
      nlog++;
      if (Mode==1 || Mode==5){logData();}
      else if (Mode==2 || Mode==3 || Mode==4){
        if(charging || discharging){logData();}
      }
    }
  }  
//Regeln der Pumpen und des Stroms
//während des Ladevorgangs
    if(charging && operation){
      int Uaverage = Uav*100.00;
      int mincv = mincellvoltage*100.00;
      int maxcv = maxcellvoltage*100.00;
      int m = map(Uaverage, mincv, maxcv, minpumppercent, maxpumppercent);
      pumppower = map(m, 0 ,100, 0, 255);

      Regelwert=(-cdccurrent-Imess)*0.5+Regelwert;
      if (Regelwert>=0)Regelwert=0;
      if (Regelwert<=-4096)Regelwert=-4096;
      dacL.setVoltage(0, false);
      delay(1);
      dacS.setVoltage(-Regelwert, false);
      delay(1);
    }
//während des Entladevorgangs
    else if(discharging && operation){
      int Uaverage = Uav*100.00;
      int mincv = mincellvoltage*100.00;
      int maxcv = maxcellvoltage*100.00;
      int m = map(Uaverage, mincv, maxcv, maxpumppercent, minpumppercent);
      pumppower = map(m, 0 ,100, 0, 255);

      Regelwert=(cdccurrent-Imess)*0.5+Regelwert;
      if (Regelwert<=0)Regelwert=0;                                   
      if (Regelwert>=4096)Regelwert=4096;
      dacS.setVoltage(0, false);
      delay(1);
      dacL.setVoltage(Regelwert, false);
      delay(1);
    }
    
//Betrieb des PumpModes
  if((Mode==5 || Mode==1) && operation){
    if(Mode==1){pumppercent=maxpumppercent;}
    float pumpvalue = (pumppercent/100.00)*(pow(2, resolution));
    pumppower = pumpvalue;
  }

  if (pumppowerlast != pumppower){
    if(millis()-timerP>20){
      timerP=millis();
      if(pumppowerlast<pumppower){
        pumppowerlast+=1;
      }
      else if(pumppowerlast>pumppower){
        pumppowerlast-=1;
      }
      ledcWrite(PWMChannel, pumppowerlast);
    }
  }*/

//Aktualisierung des Menüs bei Betätigung des Druckknopfes 
  if(digitalRead(Button)==LOW){
    delay (100);
    if (digitalRead(Button)==LOW){
      menuchange();
    }
  }
//Sicherheitsabschaltungen
//Übertemperaturabschaltung
  loadTemperature=T_CS;
  if(loadTemperature>80){
    stopall();
    digitalWrite(2,HIGH);
    delay(400);
    digitalWrite(2,LOW);
    delay(100);
    digitalWrite(2,HIGH);
    delay(100);
    digitalWrite(2,LOW);
    delay(100);
    digitalWrite(2,HIGH);
    delay(100);
    digitalWrite(2,LOW);
    statenum=5;
  }
  
//Überstromabschaltung
  /*if(Imess>8 || Imess<-8){
    stopall();
    digitalWrite(2,HIGH);
    delay(400);
    digitalWrite(2,LOW);
    delay(100);
    digitalWrite(2,HIGH);
    delay(100);
    digitalWrite(2,LOW);
    statenum=6;
  }*/

/* Periodisches Purge Intervall */
if(millis()-purgeTimeStamp >= PURGE_STEPSIZE_MS)
{
  purgeTimeStamp = millis();
  purgeStepCounter ++; /* increase purgeStepCounter */
  
  /* Open Valve after dedicated Number of OFF-States */
  if((purgeActive == false) && (purgeStepCounter >= PURGE_STEPS_OFF))
  {
    digitalWrite(26,HIGH); /* Activate Purge Valve */
    purgeActive = true;
    purgeStepCounter = 0;
  }

  /* Close Valve after dedicated Number of ON-States */
  if((purgeActive == true) && (purgeStepCounter >= PURGE_STEPS_ON))
  {
    digitalWrite(26,LOW); /* Deactivate Purge Valve */
    purgeActive = false;
    purgeStepCounter = 0;
  }
}

}


//Funktion zum Anhalten aller aktiven Prozesse
void stopall(){
  /*operation=false; (commenting just to not stop data logging)
  charging=false;
  discharging=false;
  Regelwert=0;  
  dacS.setVoltage(0, false);
  delay(1);
  dacL.setVoltage(0, false);
  delay(1);
  pumppower=0;*/
  //digitalWrite(26, LOW);
  digitalWrite(25, HIGH);
}

//ledcWrite(fans, 8);

//Funktion zum Hinzufügen neuer Messdaten zur aktuellen Datei
void appendFile(fs::FS &fs, const char * path, const char * message) {
  //Serial.printf("Appending to file: %s\n", path);
  //SD.begin(9);
  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    SDstatenum=2;//Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    SDstatenum=1;//Serial.println("Message appended");
  } else {
    SDstatenum=2;//Serial.println("Append failed");
  }
  file.close();
}

//Funktion zum Erstellen einer neuen Datei zum Begin einer neuen Messreihe
void writeFile(fs::FS &fs, const char * path, const char * message) {
  //Serial.printf("Writing file: %s\n", path);
  //SD.begin(9);
  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    SDstatenum=2;//Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    SDstatenum=1;//Serial.println("File written");
  } else {
    SDstatenum=2;//Serial.println("Write failed");
  }
  file.close();
}

//Funktion zum Festlegen der Parameter einer neuen Datei beim Start eines neuen Betriebsmodus
void setupNewFile(){
//Vorhandensein der sd karte prüfen
  SD.begin(9);
  bool fileexist=false;
  if(!SD.begin(9)) {
    SDstatenum=2;//Serial.println("Card Mount Failed");
    return;}
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    SDstatenum=2;//Serial.println("NO SD card!");
    return;}
  if (!SD.begin(9)) {
    SDstatenum=2;//Serial.println("SD init failed!");
    return;}
//Feststellen des nächsten verfügbaren Dateinamen auf der SD-Karte
  dataname = "/data"+String(datacounter)+".txt";
  File file = SD.open(dataname.c_str());
  if (file){fileexist=true;}
  while (fileexist) {
    file.close();
    datacounter ++;
    dataname = "/data"+String(datacounter)+".txt";
    File file = SD.open(dataname.c_str());
    if (!file) {
      fileexist=false;
      file.close();
      }
    }

//Abspeichern der festgelegten Einstellwerte
  switch (Mode){
    case 1:{
      messdaten = "Mode;Constant Current\r\nLog interval in ms;"+String(dataint)+"\r\nNumber of cells;"+String(numberofcells)+"\r\nMaximum pump power in %;" + String(maxpumppercent) +"\r\n\r\n"; break;}
    case 2:{
      messdaten = "Mode;Charge\r\nLog interval in ms;" + String(dataint) + "\r\nNumber of cells;" + String(numberofcells) + "\r\nMaximum pump power in %;" + String(maxpumppercent) + "\r\nMinimum pump power in %;" + String(minpumppercent) + "\r\nCharging Current in A;" + String(cdccurrent,1);
      messdaten += "\r\nMaximum cellvoltage in V;" + String(maxcellvoltage,2) + "\r\n\r\n"; break;}
    case 3:{
      messdaten = "Mode;Discharge\r\nLog interval in ms;" + String(dataint) + "\r\nNumber of cells;" + String(numberofcells) + "\r\nMaximum pump power in %;" + String(maxpumppercent) + "\r\nMinimum pump power in %;" + String(minpumppercent) + "\r\nDischarging Current in A;" + String(cdccurrent,1);
      messdaten += "\r\nMinimum cellvoltage in V;" + String(mincellvoltage,2) + "\r\n\r\n"; break;}
    case 4:{
      messdaten = "Mode;Charge-Discharge-Circles\r\nLog interval in ms;" + String(dataint) + "\r\nNumber of cells;" + String(numberofcells) + "\r\nMaximum pump power in %;" + String(maxpumppercent) + "\r\nMinimum pump power in %;" + String(minpumppercent) + "\r\nCharge-Discharge-Current in A;" + String(cdccurrent,1);
      messdaten += "\r\nNumber of cycles;" + String(numberofcycles) + "\r\nMaximum cellvoltage in V;" + String(maxcellvoltage,2) + "\r\nMinimum cellvoltage in V;" + String(mincellvoltage,2) + "\r\n\r\n"; break;}
    case 5:{
      messdaten = "Mode;Pumps-Only\r\nLog interval in ms;" + String(dataint) + "\r\nNumber of cells;" + String(numberofcells)+ "\r\n\r\n"; break;}
  }
  writeFile(SD, dataname.c_str(), messdaten.c_str());
//Erstellen eines Tabellenkopfes bestehend aus Bezeichnung und Einheit der Spalte
  if (Mode<5){messdaten ="time in s;current in A;voltage in V;T_RFB in degC;Power in W;E_IN in J;E_OUT in J;P_Pump in %; ";}
  else if (Mode==5){messdaten ="time in s;voltage in V;T_RFB in degC;P_Pump in %; ";}
  for (int c=1; c<=numberofcells; c++){
    messdaten += "U_C" + String(c)+" in V;";
  }
  messdaten += "\r\n";
  appendFile(SD, dataname.c_str(), messdaten.c_str());
//Abspeichern der Initialisierungszeit für die Erstellung der Zeitachse
  initime=millis();
}

//Funktion zur Abspeicherung der aktuellen Messdaten auf der SD-Karte
void logData(){
  int latestpump=(pumppowerlast*0.390625);
  String RFBT = "NC";
  if (T_CS<100 && T_CS>=0){RFBT=String(T_CS,0);}
  
  if (Mode<5){messdaten = String(round((millis()-initime)/1000)) + ";" + String(Imess, 2) + ";"+String(Umess, 2) + ";" + RFBT + ";" + String(Pmess, 1) + ";" + String(Cmess, 0) + ";" + String(Emess, 0) + ";" + String(latestpump) + ";";}
  else if (Mode==5){messdaten = String(round((millis()-initime)/1000)) + ";" + String(Umess, 2) + ";" + RFBT + ";" + String(latestpump) + ";";}
  for (int c=0; c<numberofcells; c++){
    messdaten +=  String(Ucell[c],4) + ";";
  }
  messdaten += "\r\n";
  appendFile(SD, dataname.c_str(), messdaten.c_str());
}

