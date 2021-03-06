/*
  Arduino MQ135
  connect the sensor as follows :
  A H A   >>> 5V
  B       >>> A0
  H       >>> GND
  B       >>> 10K ohm >>> GND
 
  Contribution: epierre
  Based on David Gironi http://davidegironi.blogspot.fr/2014/01/cheap-co2-meter-using-mq135-sensor-with.html
  http://skylink.dl.sourceforge.net/project/davidegironi/avr-lib/avr_lib_mq135_01.zip
 
*/

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69


// #include <SPI.h>    
//#include <MySensor.h>
//#include <MySensors.h>
#include <Wire.h> 

#define CHILD_ID_AIQ 0
#define AIQ_SENSOR_ANALOG_PIN A3

#define MQ135_DEFAULTPPM 100 //default ppm of NH3 for calibration
#define MQ135_DEFAULTRO 108251 //default Ro for MQ135_DEFAULTPPM ppm of NH3
#define MQ135_SCALINGFACTOR 37.58805473 //for NH3
#define MQ135_EXPONENT -3.235365807 //for NH3
#define MQ135_MAXRSRO 1.505 //for NH3
#define MQ135_MINRSRO 0.56 //for NH3

unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in seconds)
//VARIABLES
float mq135_ro = 10000.0;    // this has to be tuned 10K Ohm
int val = 0;                 // variable to store the value coming from the sensor
float valAIQ =0.0;
float lastAIQ =0.0;

void setup()  
{
  Serial.begin(9600);
}

/*
 * get the calibrated ro based upon read resistance, and a know ppm
 */
long mq135_getro(long resvalue, double ppm) {
return (long)(resvalue * exp( log(MQ135_SCALINGFACTOR/ppm) / MQ135_EXPONENT ));
}

/*
 * get the ppm concentration
 */
double mq135_getppm(long resvalue, long ro) {
double ret = 0;
double validinterval = 0;
validinterval = resvalue/(double)ro;
if(validinterval<MQ135_MAXRSRO && validinterval>MQ135_MINRSRO) {
ret = (double)MQ135_SCALINGFACTOR * pow( ((double)resvalue/ro), MQ135_EXPONENT);
}
return ret;
}

void loop()      
{    
  uint16_t valr = analogRead(AIQ_SENSOR_ANALOG_PIN);// Get AIQ value
  Serial.println(val);
  uint16_t val =  ((float)22000*(1023-valr)/valr); 
  //during clean air calibration, read the Ro value and replace MQ135_DEFAULTRO value with it, you can even deactivate following function call.
  mq135_ro = mq135_getro(val, MQ135_DEFAULTPPM);
  //convert to ppm (using default ro)
  valAIQ = mq135_getppm(val, MQ135_DEFAULTRO);

  Serial.print ( "Val / Ro / value:");
  Serial.print ( val);
  Serial.print ( " / ");
  Serial.print ( mq135_ro);
  Serial.print ( " / ");
  Serial.print ( valAIQ);
  delay(1000);
}

 
/*****************************  MQGetPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         pcurve      - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
         of the line could be derived if y(rs_ro_ratio) is provided. As it is a 
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
         value.
************************************************************************************/ 
int  MQGetPercentage(float rs_ro_ratio, float ro, float *pcurve)
{
  return (double)(pcurve[0] * pow(((double)rs_ro_ratio/ro), pcurve[1]));
}
