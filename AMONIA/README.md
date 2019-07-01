The first step that must be done to calculate R0, how we achieve this with a Sub Program called CAlc_R0 which is attached.
At first we uploas this sketch, and we wil get a serial responce from it.
We take note of this Value, IT'S VERY IMPORTANT!!
it's NOT A FIXED VALUE! it depends of the ambient
Now... we update some values in our pmain program:

```
int gas_sensor = A0; //Sensor pin
float m = -0.263; //Slope
float b = 0.42; //Y-Intercept
float R0 = 2.76; //Sensor Resistance in fresh air from previous code
```
