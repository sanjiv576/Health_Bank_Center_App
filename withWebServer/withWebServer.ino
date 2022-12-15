

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "SS_wlink_2.4";
const char *password = "@74NePaL#64!";

WebServer server(80);

const int led = 13;

void handleRoot() {

  String gui = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><link rel='icon' type='image/x-icon' href='https://cdn-icons-png.flaticon.com/128/2382/2382533.png'><title>Health Bank Center</title><!-- CSS only --><link href='https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css' rel='stylesheet'integrity='sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65' crossorigin='anonymous'><style>* {color: white;background-color: rgb(45, 40, 40);}nav .logo {float: left;width: 30%; }nav .logo img {margin: 10px 0px 0px 30px;}nav .title h1 {padding:90px;}nav .title {float: right;width: 70%;padding: 8px;}.main-container {display: flex;}.fluid-container .health-status,.fluid-container .health-title {width: 100%;display: flex;}.fluid-container .health-title h2 {margin-left: 25%;}.health-status>div,.health-title>div {width: 25%;display: flex !important;}.health-title {text-decoration: underline;text-shadow: white;color: rgb(33, 194, 38);}.health-status>div {height: 200px;}.health-status .reading h1 {font-size: 75px;padding: 50px;}.message label,.suggestion label {padding: 20px;} @media only screen and (max-width: 1125px) {.health-status .reading h1 {padding: 24px;}}@media only screen and (max-width: 526px) {nav .title h1 {padding: 46px; }.fluid-container .health-title h2 {margin-left: 0%;float: left;}}@media only screen and (max-width: 848px) {nav .title {width: 60%;}}@media only screen and (min-width: 990px) {.message label,.suggestion label {font-size: 25px;}}footer {padding: 8px;height: 40px;color: black;text-align: center;color: white;}.sendResult {margin: auto;width: 100%;}.sendResult .userForm {display: flex;width: 100%;margin-left: 20%;}.userForm > div {width: 50%;} input {background-color: whitesmoke!important;padding:8px; color: black !important; width: 100% !important;}button { height: 100%;margin-left: 5px;}</style></head><body><nav><div class='logo'><img src='https://github.com/sanjiv576/Health_Bank_Center_App/blob/master/logo.png?raw=true' alt='' class='img-fluid'></div><div class='title'><h1 class='text-center text-uppercase'>Health Bank Center</h1> </div></nav><div class='fluid-container'><div class='health-title text-center'> <div class=''><h2 class=''>Testing</h2> </div><div class=''><h2>Reading</h2> </div> <div class=''><h2>Message</h2></div><div class=''><h2>Suggestion</h2></div></div> <div class='health-status'><div class='health-image'><img src='https://cdn-icons-png.flaticon.com/512/3035/3035523.png' alt=''class='img-fluid'></div><div class='reading text-center'><h1 class='text-center'>";

  gui += "64";

  gui += "</h1></div><div class='message'><label for='' class='text-center'>";

  gui += " Normal heart beat. No need to worry.";

  gui += "</label></div><div class='suggestion'><label for='' class='text-center'>";

  gui += "No need to consult with a doctor.";

  gui += "</label></div></div><div class='health-status'><div class='health-image'><img src='https://cdn-icons-png.flaticon.com/512/6821/6821265.png' alt=''class='img-fluid'></div><div class='reading text-center'><h1>";

  gui += "64";

  gui += "</h1></div><div class='message'><label for='' class='text-center'>";

  gui += "Normal heart beat. No need to worry.";

  gui += "</label></div><div class='suggestion'><label for='' class='text-center'>";

  gui += " No need to consult with a doctor.";

  gui += " </label></div></div><div class='health-status'><div class='health-image'><img src='https://cdn-icons-png.flaticon.com/512/2903/2903592.png' alt=''class='img-fluid'></div><div class='reading text-center'><h1 class='text-center'>";

  gui += "64";

  gui += "</h1></div><div class='message'><label for='' class='text-center'>";

  gui += "Normal heart beat. No need to worry.";

  gui += "</label></div><div class='suggestion'><label for='' class='text-center'>";

  gui += "No need to consult with a doctor.";

  gui += "</label></div> </div></div><div class='sendResult'><br><form action='' class='form-group userForm'>   <div><input class='form-input' type='email' placeholder='Enter your email' name='' required></div> <div ><button class='btn btn-primary' type='submit' >Send Result</button></div></form><br></div></div><footer>&copy; Bank Health Center</footer></body></html>";


  server.send(200, "text/html", gui);

}

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);

}

void setup(void) {
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
