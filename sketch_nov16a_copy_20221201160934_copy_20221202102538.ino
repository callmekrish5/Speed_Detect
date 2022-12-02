#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define FIREBASE_HOST "nodemcuspeed-default-rtdb.firebaseio.com/" // realtime database url
#define FIREBASE_AUTH "531OMJuqrrvz0961Lzp5L7CLYP2IOSiBOcg92rnH" // database key

const char* ssid = "ranjitabinod_5"; //WIFI SSID
const char* password = "Poiuy@0987"; //WIFI Password
const char* host = "maker.ifttt.com"; //IFTTT url
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;
FirebaseJson json;

int sen1=D0;
int sen2=D1;
unsigned long t1=0;
unsigned long t2=0; 
float velocity;
float velocity_real;
float timeFirst;
float timeScnd;
float diff;
float speedConst=8;  //in cm.

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Wifi not connected");
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
   if (analogRead(sen1)<500 && analogRead(sen2)>500)
    {
      timeFirst = millis();
      delay(30);
    }
 
  if (analogRead(sen2)>500 && analogRead(sen1)<500)
  {
    timeScnd = millis();
    diff = timeScnd - timeFirst; 
    velocity = speedConst / diff;
    velocity_real = (velocity*360)/100;     //milliseconds to hours and centimetres to kilometeres.
    delay(30);
    Serial.print("\n the velocity is : ");
    Serial.println(velocity_real);
    Serial.print(" km/hr. ");
    if (Firebase.setString(firebaseData, "/Attendance/speed", 10)){
        Serial.println("PASSED");
      }else{
        Serial.println("FAILED");
    }
    WiFiClient client ;
    const int httpPort = 80;
    if (!client.connect(host, httpPort )) {
      Serial.println("connection failed");
      delay(1000);
      return;
    }
    String url = "/trigger/app/json/with/key/jy-fQKg7DwGDNLOd1puPZr6HOJxwNmXqasOaPgFy3FD";
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + host +  "\r\n" + 
                                  "Connection: close\r\n\r\n");
  }
}