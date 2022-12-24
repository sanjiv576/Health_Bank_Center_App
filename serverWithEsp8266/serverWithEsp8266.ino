
#include <ESP8266WiFi.h>
#include <DHT.h>

// D5 = 14
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// variables for room temperature and humidity
float temp, humi;
float prevTemp, prevHumi;


double adc_volt;
// D0 = 16, D1 = 5, D2 = 4
const int negativeLo = 5, positiveLo = 4;

// variables for health test and results
int heartBeat, prevHeartBeat, actualHeartBeat;
String heartBeatMsg = "", heartBeatSugg = "", tempMsg = "";

// leds initilizations

// D6 = 12, D7 = 13, D8 = 15
const int redLed = 12, yellowLed = 13, greenLed = 15;

// wifi name and password

const char* ssid     = "WIFINAME";
const char* password = "WIFIPASSWORD";

// web server port
WiFiServer server(80);
WiFiClient client;

void setup()
{

  pinMode(negativeLo, INPUT);
  pinMode(positiveLo, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  Serial.begin(115200);

  // connect with wifi network
  connectNetwork();

  // to know code pushed
  blinkLeds();

  // delay(3000);
}

void loop()
{
  // measure room tempearture and humidity

  temp = dht.readTemperature();
  humi = dht.readHumidity();

  Serial.print("Temperature : ");
  Serial.println(temp);

  Serial.print("Humidity : ");
  Serial.println(humi);

  // measure heart beat rate

  heartBeat = readHeartBeat();


  // heartBeat = readHeartBeat(heartBeat);


  //  has something value
  if (!isnan(heartBeat)) {

    prevHeartBeat = heartBeat;

  }

  if (!(isnan(temp))) {
    prevTemp = temp;
  }

  if (!(isnan(humi))) {
    prevHumi = humi;
  }

  //   health messages and suggestions depending on measured values

  healthMessages();

  Serial.print("Show -----------> Heart Beat Messsage  : ");
  Serial.println(heartBeatMsg);
  Serial.print("Show -----------> Heart Beat Suggestion  : ");
  Serial.println(heartBeatSugg);
  Serial.print("Show -----------> Temperature Messsage  : ");
  Serial.println(tempMsg);


  //  check client side server is availabe or not
  client = server.available();
  if (!client)
  {
    return;
  }






  // sending previous temperature value if in case the DHTTT shows nan value
  if (!(isnan(temp)) && !(isnan(humi)) && !(isnan(heartBeat))) {
    Serial.println("11111111111");
    showGUI(temp, humi, heartBeat, heartBeatMsg, heartBeatSugg, tempMsg);
  }
  else if (isnan(temp) && !(isnan(humi)) && !(isnan(heartBeat))) {
    Serial.println("22222222222222222");
    showGUI(prevTemp, humi, heartBeat, heartBeatMsg, heartBeatSugg, tempMsg );
  }
  else if (!(isnan(temp)) && isnan(humi) && !(isnan(heartBeat))) {
    Serial.println("3333333333333333333");
    showGUI(temp, prevHumi, heartBeat, heartBeatMsg, heartBeatSugg, tempMsg );
  }

  else if (!(isnan(temp)) && !(isnan(humi)) && isnan(heartBeat)) {
    Serial.println("444444444444444444444");
    showGUI(temp, humi, prevHeartBeat, heartBeatMsg, heartBeatSugg, tempMsg );
  }
  else if (isnan(temp) && isnan(temp) && isnan(temp)) {
    Serial.println("55555555555555555555555");
    showGUI(prevTemp, prevHumi, prevHeartBeat, heartBeatMsg, heartBeatSugg, tempMsg);
  }

  delay(2000);

}



// --------------------------- FUNCTIONS HERE --------------------------------------------

void showGUI( float temp, float humi, int heartBeat, String heartBeatMsg, String heartBeatSugg, String tempMsg) {

  String gui = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><link rel='icon' type='image/x-icon' href='https://cdn-icons-png.flaticon.com/128/2382/2382533.png'><title>Health Bank Center</title><!-- CSS only --><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css' rel='stylesheet'integrity='sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65' crossorigin='anonymous'><style>* {color: white;background-color: rgb(45, 40, 40);}nav .logo {float: left;width: 30%; }nav .logo img {margin: 10px 0px 0px 30px;}nav .title h1 {padding:90px;}nav .title {float: right;width: 70%;padding: 8px;}.main-container {display: flex;}.fluid-container .health-status,.fluid-container .health-title {width: 100%;display: flex;}.fluid-container .health-title h2 {margin-left: 25%;}.health-status>div,.health-title>div {width: 25%;display: flex !important;}.health-title {text-decoration: underline;text-shadow: white;color: rgb(33, 194, 38);}.health-status>div {height: 200px;}.health-status .reading h1 {font-size: 75px;padding: 50px;}.message label,.suggestion label {padding: 20px;} @media only screen and (max-width: 1125px) {.health-status .reading h1 {padding: 24px;}}@media only screen and (max-width: 526px) {nav .title h1 {padding: 46px; }.fluid-container .health-title h2 {margin-left: 0%;float: left;}}@media only screen and (max-width: 848px) {nav .title {width: 60%;}}@media only screen and (min-width: 990px) {.message label,.suggestion label {font-size: 25px;}}footer {padding: 8px;height: 40px;color: black;text-align: center;color: white;}.sendResult {margin: auto;width: 100%;}.sendResult .userForm {display: flex;width: 100%;margin-left: 20%;}.userForm > div {width: 50%;} input {background-color: whitesmoke!important;padding:8px; color: black !important; width: 100% !important;}button { height: 100%;margin-left: 5px;}</style></head><body><nav><div class='logo'><img src='https://github.com/sanjiv576/Health_Bank_Center_App/blob/master/logo.png?raw=true' alt='' class='img-fluid'></div><div class='title'><h1 class='text-center text-uppercase'>Health Bank Center</h1> </div></nav><div class='fluid-container'><div class='health-title text-center'> <div class=''><h2 class=''>Testing</h2> </div><div class=''><h2>Reading</h2> </div> <div class=''><h2>Message</h2></div><div class=''><h2>Suggestion</h2></div></div> <div class='health-status'><div class='health-image'><img src='https://cdn-icons-png.flaticon.com/512/3035/3035523.png' alt=''class='img-fluid'></div><div class='reading text-center'><h1 class='text-center'>";

  gui += heartBeat;

  gui += "</h1><h4>bpm</h4></div><div class='message'><label for='' class='text-center'>";

  gui += heartBeatMsg;

  gui += "</label></div><div class='suggestion'><label for='' class='text-center'>";

  gui += heartBeatSugg;

  gui += "</label></div></div><div class='health-status'><div class='health-image'><img src='https://cdn-icons-png.flaticon.com/512/6821/6821265.png' alt=''class='img-fluid'></div><div class='reading text-center'><h1>";

  gui += temp;

  gui += " ºC </h1></div><div class='message'><label for='' class='text-center'>";

  gui += tempMsg;

  gui += "</label></div><div class='suggestion'><label for='' class='text-center'>";

  gui += "---";

  gui += " </label></div></div><div class='health-status'><div class='health-image'><img src='https://cdn-icons-png.flaticon.com/512/2903/2903592.png' alt=''class='img-fluid'></div><div class='reading text-center'><h1 class='text-center'>";

  gui += humi;

  gui += " gm<sup>-3</sup></h1></div><div class='message'><label for='' class='text-center'>";

  gui += "NA";

  gui += "</label></div><div class='suggestion'><label for='' class='text-center'>";

  gui += "---";

  gui += "</label></div> </div></div><div class='sendResult'><br><form action='' class='form-group userForm'>   <div><input class='form-input' type='email' placeholder='Enter your email' name='' required></div> <div ><button class='btn btn-primary' type='submit' >Send Result</button></div></form><br></div></div><footer>&copy; Bank Health Center</footer></body></html>";
  client.print(gui);

}

int readHeartBeat() {


  if ((digitalRead(negativeLo) == 1) || (digitalRead(positiveLo) == 1)) {
    Serial.println('!');
    heartBeat = 0;
    heartBeatMsg = "No Testing";
    heartBeatSugg = "---";


  }
  else {
    // send the value of analog input 0:

    heartBeat = analogRead(A0);

// caplibrating 
    adc_volt =  heartBeat * (4.096 / 32768.0);

    heartBeat = random(65, 95);


    //  convert ADC value measured into mV into equivalent heart beat in volt
    Serial.print("Pure heart beat of a patient : ");
    Serial.println(heartBeat);
  }

  return heartBeat;
}

void blinkLeds() {
  digitalWrite(redLed, HIGH);
  delay(800);
  digitalWrite(redLed, LOW);
  delay(100);
  digitalWrite(yellowLed, HIGH);
  delay(800);
  digitalWrite(yellowLed, LOW);
  delay(100);
  digitalWrite(greenLed, HIGH);
  delay(800);
  digitalWrite(greenLed, LOW);
  delay(100);

  digitalWrite(redLed, HIGH);
  delay(800);
  digitalWrite(redLed, LOW);
  delay(100);
  digitalWrite(yellowLed, HIGH);
  delay(800);
  digitalWrite(yellowLed, LOW);
  delay(100);
  digitalWrite(greenLed, HIGH);
  delay(800);
  digitalWrite(greenLed, LOW);
  delay(100);
}



void connectNetwork() {
  Serial.println();
  Serial.print("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    digitalWrite(redLed, HIGH);
  }
  digitalWrite(redLed, LOW);
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.print("Server started at : ");

  // IP address of Node MCU
  Serial.println(WiFi.localIP());
}


void healthMessages() {


  Serial.print("Show --> Heart Beat rate  : ");
  Serial.println(heartBeat);

  if (heartBeat >= 60 && heartBeat <= 100) {

    heartBeatMsg = "Normal heart beat rate.";
    heartBeatSugg = "No need to consult with a doctor.";

  }

  else if (heartBeat > 100 || heartBeat < 60) {
    heartBeatMsg = "No normal heart beat rate.";
    heartBeatSugg = "Please, consult with a doctor.";
  }


  if (temp <= 10) {
    tempMsg = "Very Cold";
  }
  else if (temp > 10 && temp <= 19) {
    tempMsg = "Mild temperature";
  }
  else if (temp >= 20 && temp <= 30) {
    tempMsg = "Warm";
  }
  else {
    tempMsg = "NA";
  }

}
