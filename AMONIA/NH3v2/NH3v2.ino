int gas_sensor = A1;                      //Sensor pin
float m = -0.263;                         //Slope
float b = 0.42;                           //Y-Intercept
float R0 = 2.77;                          //Sensor Resistance in fresh air from previous code
float sensor_volt;                        //Define variable for sensor voltage
float RS_gas;                             //Define variable for sensor resistance
float ratio;                              //Define variable for ratio
float sensorValue;                        //get value from sensor
double ppm_log;                           //Get ppm value in linear scale according to the the ratio value
double ppm;                               //Convert ppm value to log scale

void setup() {
  Serial.begin(9600); //Baud rate
  pinMode(gas_sensor, INPUT); //Set gas sensor as input
  delay(1000);
}

void calculate_ppm() {  
  sensorValue = analogRead(gas_sensor); //Read analog values of sensor
  sensor_volt = sensorValue * (5.0 / 1023.0); //Convert analog values to voltage
  RS_gas = ((5.0 * 10.0) / sensor_volt) - 10.0; //Get value of RS in a gas
  ratio = RS_gas / R0; // Get ratio RS_gas/RS_air
  
  //convert to ppm value//
  ppm_log = (log10(ratio) - b) / m; 
  ppm = pow(10, ppm_log); 
  
  //display value//
  Serial.print("sensor voltage: "); 
  Serial.print(sensor_volt);
  Serial.print("\t Ammonia: ");
  Serial.println(ppm);
  delay(2000);
}

void loop(){
  calculate_ppm();
}
