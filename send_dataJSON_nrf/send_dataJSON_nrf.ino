#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino_JSON.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#define ALTITUDE 8 // Altitude in Jakarta, indonesia

Adafruit_BME280 bme; // I2C
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
JSONVar myJSON; //insiasi JSON
float temperature;
float humidity;
float pressure;
float altitude;

void setup() {
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  bool status;
  status = bme.begin(0x76);  //The I2C address of the sensor I use is 0x76
  if (!status) {
      Serial.print("Error. Check");
      Serial.println("I2C connections");
      while (1);
  }
}

float getTemperature()
{
  temperature = bme.readTemperature();
}

float getHumidity()
{
  humidity = bme.readHumidity();
}

float getPressure()
{
  pressure = bme.readPressure();
  pressure = bme.seaLevelForAltitude(ALTITUDE,pressure);
  pressure = pressure/100.0F;
}

float getAltitude()
{
  altitude=bme.readAltitude(SEALEVELPRESSURE_HPA);
}


void dataJSON(){
  getPressure();
  getHumidity();
  getTemperature();
  getAltitude();

//  int kadar_amonia = int(random(0,10));
//  int intensitas_cahaya = int(random(50,300));
//  int kecepatan_angin = int(random(0,20));
  
  myJSON["id_perangkat"] = "testing_node";
//  myJSON["kadar_amonia"] = kadar_amonia;
  myJSON["kelembapan"] = humidity;
//  myJSON["intensitas_cahaya"] = intensitas_cahaya;
//  myJSON["kecepatan_angin"] = kecepatan_angin;
  myJSON["temperatur"] = temperature;
  myJSON["tekanan"] = pressure;
//  Serial.println(myJSON);
//  delay(10000);
}

void loop() {
  getPressure();
  getHumidity();
  getTemperature();
  char id[]="ID-01 ";
  int data= (int)temperature;
//  radio.write(&data, sizeof(data));
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
//  Serial.println(data);
  delay(3000);
}
