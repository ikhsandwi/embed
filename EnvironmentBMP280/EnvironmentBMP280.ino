#include <Arduino_JSON.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#define ALTITUDE 216.0 // Altitude in Sparta, Greece

float temperature;
float humidity;
float pressure;
float altitude;

Adafruit_BME280 bme; // I2C

void setup(void) {
  Serial.begin(9600);
  Serial.print("Reading sensor");
  Serial.println();
  bool status;
  status = bme.begin(0x76);  //The I2C address of the sensor I use is 0x76
  if (!status) {
      Serial.print("Error. Check");
      Serial.println("I2C connections");
      while (1);
  }
}

void dataJSON(){
  JSONVar myJSON; //insiasi JSON
  getPressure();
  getHumidity();
  getTemperature();
  getAltitude();

  String kelembapan = String(humidity,2);
  String temperatur = String(temperature,2);
  String tekanan = String(pressure,2);
  String kadar_amonia = String(random(0,15));
  String intensitas_cahaya = String(random(20,100));
  String kecepatan_angin = String(random(50,100));
  
  myJSON["id_perangkat"] = "testing_node";
  myJSON["kadar_amonia"] = kadar_amonia;
  myJSON["kelembapan"] = kelembapan;
  myJSON["intensitas_cahaya"] = intensitas_cahaya;
  myJSON["kecepatan_angin"] = kecepatan_angin;
  myJSON["temperatur"] = temperatur;
  myJSON["tekanan"] = tekanan;
  Serial.println(myJSON);
  delay(10000);
}

void loop(){
  dataJSON();
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
