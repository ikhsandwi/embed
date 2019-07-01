/*
Creator: Ikhsan
Created: 190619
Version: 1
CodeReviewer: Brian
Goals: Mengambil data sensor angin
*/ 
 
//Global Variable
const int m_time = 3;      //Meassuretime in Seconds
int wind_ct = 0;
float wind = 0.0,
      meterPerSecond = 0.0;
unsigned long time = 0;
//---------------//

void setup(){
  Serial.begin(9600);
  time = millis();
}


void countWind() {
  wind_ct ++;
}

void meassure() {
  //meassure wind speed with interupt digital pin 
  wind_ct = 0;
  time = millis();
  attachInterrupt(1, countWind, RISING);        // RISING (mode)= the interrupt will be executed if there is a change in the reading value from LOW to HIGH (executed 1 time after a change occurs)
  delay(1000 * m_time);
  detachInterrupt(1);                           //pin interrupt 1
  wind = (float)wind_ct / (float)m_time * 2.4;  //calculation
  
  //--display--//
  Serial.print("Wind Speed: ");
  Serial.print(wind);                           //Speed in Km/h
  Serial.print(" km/h - ");
  meterPerSecond = wind / 3.6;                  //convert Km/h to m/s
  Serial.print(meterPerSecond);                 //Speed in m/s
  Serial.println(" m/s");
}


void loop(){
  meassure();
}
