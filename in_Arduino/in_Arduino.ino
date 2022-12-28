#include "MQ135.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>



Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int gasValue;
int bodyTempVal;
int measureTemp = 0.00;
const int gasPin = A0;
void setup () {
  Serial.begin (9600);
  mlx.begin();
  pinMode(gasPin, INPUT);
}
void loop() {
  MQ135 gasSensor = MQ135(A0); // Attach sensor to pin A0

  gasValue = analogRead(gasPin);
  bodyTempVal = 37;

  for (int i = 0; i < 21; i++) {
    measureTemp += mlx.readObjectTempC();
    // finger 10 seconds
    delay(500);
  }


  int avgTemp = measureTemp / 20;
  int actuaVal = (mlx.readObjectTempC() * 0.5) + mlx.readObjectTempC();
  //  Serial.print("Acutal temp : ");
  //  Serial.println(actuaVal);
  //
  //
  //
  //  Serial.print("Average temp value : ");
  //  Serial.println(measureTemp);

  Serial.println ((String) "Gas Value: " + gasValue + " Body Temp: " + avgTemp);
  delay(1000);

}
