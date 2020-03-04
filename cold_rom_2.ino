#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>// blynk library
#include <ESP8266WiFi.h>    // esp8266 library
#include <FirebaseArduino.h>// firebase library
#include <DHT.h>  // dht11 temperature and humidity sensor library

#define FIREBASE_HOST "cold-room-9b2fd.firebaseio.com"// host name 
#define FIREBASE_AUTH "r22bcsV8qv3VVtiI3SOMti7Mmd88pR07HJiJka3V" // secreat key of firebase
char auth[] = "6z-rFKQFJAnmJJyaxG2bD5kMEGu7JmHO"; // key of blynk
#define WIFI_SSID "Rugema"
#define WIFI_PASSWORD "ru123456" //password of wifi ssid
BlynkTimer timer;
#define DHTPIN D2            // what digital pin we're connected to
#define DHTTYPE DHT11      // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE);// construct with datapin and dhtype as parameters
int greenled=D3;  // assign  red led to digital pin 3 
int redled=D4; // assign green led to digital 4
float t,h=0; // declare variable to hold sensor as input                                                   
// create method to send data to blynk
void sendSensor()
{
  Blynk.virtualWrite(V1, t); // write the temperature from the DHT to virtual pin
  delay (200);  // pause program 200 ms
  Blynk.virtualWrite(V2, h);// write the humidity from the DHT sensor 
  delay (200);   
 }
void setup() 
{
  pinMode (D3,OUTPUT); // set D3 as output pin
  pinMode (D4, OUTPUT);// set D4 as output pin
  Serial.begin(9600);  // set the baud rate of serial monitor
  delay(1000);          // pause program 1 second              
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //start the wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);   // print on serial monitor ssid name
  // scann the network status
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); // print dot while network is not available
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  // connect to firebase
  Blynk.begin(auth, WIFI_SSID, WIFI_PASSWORD);
  timer.setInterval(1000L, sendSensor);
  dht.begin();                             //Start reading dht sensor
}

void loop() { 
    Blynk.run();
  timer.run();
   h = dht.readHumidity();    // Reading temperature or humidity takes about 250 milliseconds!
   t = dht.readTemperature();  // Read temperature as Celsius (the default)
    
  if (isnan(h) || isnan(t)) {            
    // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
    delay(2000);
     }  

  Serial.print("Humidity: ");  Serial.print(h);  // print on serial monitor temp, and humidity
  String fireHumid = String(h) + String("%");     //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");
  String fireTemp = String(t) + String("°C");      //convert integer temperature to string temperature
  delay(4000);
  
  Firebase.pushString("/DHT11/Humidity", fireHumid);  //setup path and send readings
  Firebase.pushString("/DHT11/Temperature", fireTemp);//setup path and send readings
 if(t>25)
 {
  digitalWrite(redled,HIGH);  // blink green led if temperature goes up 25
  }  
  else
  {
    digitalWrite(greenled,1);  // blink red led if temperature goes bellow 25
    }
}
