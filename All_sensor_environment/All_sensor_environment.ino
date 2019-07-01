//----------------library---------------//
#include <Wire.h>                       //I2C
#include <BH1750.h>                     //sensor intensitas cahaya
#include <Arduino_JSON.h>               //JSON
#include <Adafruit_Sensor.h>    
#include <Adafruit_BME280.h>            //sensor suhu,kelembaban,tekanan

//------------initialization------------//
BH1750 lightMeter(0x23);                //light sensor BH1750 Address

Adafruit_BME280 bme;                    //sensor BME280(suhu,kelembaban,tekanan,altitude)
#define SEALEVELPRESSURE_HPA (1013.25)  //calibration value pressure hPa
#define ALTITUDE 216.0                  // Altitude in Sparta, Greece

//-----------Global Variable------------//
int light;
float   temperature ,
        humidity    ,
        pressure    ,
        altitude    ;
//wind velocity
const int m_time = 3      ;                   //Meassuretime in Seconds
int       wind_ct = 0     ;
float     wind = 0.0            ,
          meterPerSecond = 0.0  ;
unsigned long time = 0      ;
//Amonia gas
int gas_sensor = A1 ;    //Sensor pin
float   m = -0.263  ,    //Slope
        b = 0.42    ,    //Y-Intercept
        R0=2.77     ,    //R0 value from calibration
        sensor_volt ,    //Define variable for sensor voltage
        RS_gas      ,    //Define variable for sensor resistance
        ratio       ,    //Define variable for ratio
        sensorValue ,    //get value from sensor
        RS_air      ;    //Define variable for sensor resistance
long jedaSebelum = 0;
double ppm_log      ,    //Get ppm value in linear scale according to the the ratio value
       ppm          ;    //Convert ppm value to log scale
bool   status       ;

void setup() {
  Serial.begin(9600)        ;
  Wire.begin()              ;
  time = millis()           ;
  pinMode(gas_sensor, INPUT); //Set gas sensor as input
  
  status = bme.begin(0x76)  ; //The I2C address of the sensorBME280 I use is 0x76
  if (!status) {
      Serial.print("Error. Check");
      Serial.println("I2C connections BME280");
      while (1);
  }
  
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("--Start Monitoring-- \n"));
  }
  else {
    Serial.println(F("Error initialising I2C Address"));
  }
  Serial.print("Reading sensor");
  Serial.println();

  delay(1000);
}

// Mengambil data suhu
float getTemperature(){
  temperature = bme.readTemperature();
}
// Mengambil data kelembaban
float getHumidity(){
  humidity = bme.readHumidity();
}
// Mengambil data tekanan
float getPressure(){
  pressure = bme.readPressure();
  pressure = bme.seaLevelForAltitude(ALTITUDE,pressure);
  pressure = pressure/100.0F;
}
// Mengambil data Altitude
float getAltitude(){
  altitude=bme.readAltitude(SEALEVELPRESSURE_HPA);
}
// Mengabil data cahaya
int getLight(){
  light = lightMeter.readLightLevel();
}
// Increment satuan kecepatan angin
void countWind() {
  wind_ct ++;
}
// Menghitung kecepatan angin
void get_wind_speed() {
  //meassure wind speed with interupt digital pin 
  wind_ct       = 0                    ;
  time = millis()                      ;
  attachInterrupt(1, countWind, RISING);        // RISING (mode)= the interrupt will be executed if there is a change in the reading value from LOW to HIGH (executed 1 time after a change occurs)
  delay(3000)                          ;
  detachInterrupt(1)                   ;        //pin interrupt 1
  wind = (float)wind_ct / (float)m_time * 2.4;  //calculation in Km/h
  meterPerSecond = wind / 3.6                ;  //convert Km/h to m/s
}
// Menghitung data gas
void get_gas_ppm() {  
  sensorValue = analogRead(gas_sensor)              ; // Read analog values of sensor
  sensor_volt = sensorValue * (5.0 / 1023.0)        ; // Convert analog values to voltage
  RS_gas      = ((5.0 * 10.0) / sensor_volt) - 10.0 ; // Get value of RS in a gas
  ratio       = RS_gas / R0                         ; // Get ratio RS_gas/RS_air
  
  //convert to ppm value//
  ppm_log   = (log10(ratio) - b) / m; 
  ppm       = pow(10, ppm_log); 
}

void dataJSON(){
  JSONVar myJSON        ; //insiasi JSON
  getPressure()         ;
  getHumidity()         ;
  getTemperature()      ;
  getAltitude()         ;
  getLight()            ;
  get_wind_speed()      ;
  get_gas_ppm()         ;
  String kelembapan         = String(humidity,2)        ;
  String temperatur         = String(temperature,2)     ;
  String tekanan            = String(pressure,2)        ;
  String kadar_amonia       = String(ppm,2)             ;
  String intensitas_cahaya  = String(light)             ;
  String kecepatan_angin    = String(meterPerSecond,2)  ;
  
  myJSON["id_perangkat"]        = "testing_node"        ;
  myJSON["kadar_amonia"]        = kadar_amonia          ;
  myJSON["kelembapan"]          = kelembapan            ;
  myJSON["intensitas_cahaya"]   = intensitas_cahaya     ;    
  myJSON["kecepatan_angin"]     = kecepatan_angin       ;
  myJSON["temperatur"]          = temperatur            ;
  myJSON["tekanan"]             = tekanan               ;
  Serial.println(myJSON)    ;
  delay(5000);
}

void loop() {
  dataJSON();
//  calibration_Amonia_Sensor();
}
