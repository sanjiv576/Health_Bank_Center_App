#include <DHT.h>


#define DHTPIN 5

#define DHTTYPE DHT22


DHT dht(DHTPIN, DHTTYPE);


float temp, humi;
const int negativeLO = 11, positiveLO = 10;

void setup() {
  Serial.begin(9600);
  // Setup for leads off detection LO +
  pinMode(positiveLO, INPUT);
  // Setup for leads off detection LO -
  pinMode(negativeLO, INPUT);
  Serial.println("Code Strated");

}


void loop() {
  readTempHumi();

}

// using DHT22 sensor
void readTempHumi() {
  temp = dht.readTemperature();
  humi = dht.readHumidity();

  if (isnan(temp) || isnan(humi)) {
    Serial.println("Humidity and Temperature are not detected");
  }
  else {
    Serial.print("Temperature in C ");
    Serial.println(temp);

    Serial.print("Humidity ");
    Serial.println(humi);
    delay(2000);
  }
}

// using ECG sensor
void readHeartBeat() {
  if ((digitalRead(positiveLO) == 1) || (digitalRead(negativeLO) == 1)) {
    Serial.println('!');
  }
  else {
    // send the value of analog input 0:

    int val = analogRead(A0);
    Serial.println(val);
    delay(2000);
  }
  //Wait for a bit to keep serial data from saturating
  delay(1);
}
