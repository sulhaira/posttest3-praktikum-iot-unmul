#define BLYNK_TEMPLATE_ID "TMPL6dpFQe7Y6"
#define BLYNK_TEMPLATE_NAME "posttest2"
#define BLYNK_AUTH_TOKEN "OVCBCdvdy23TkIlcfoLB-TJvP9sNue-4"

#define BLYNK_PRINT Serial



#include <ESP8266WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <BlynkSimpleEsp8266.h>
const char* mqtt_server = "broker.hivemq.com";


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Universitas Mulawarman"; //nama hotspot yang digunakan
char pass[] = ""; //password hotspot yang digunakan

// #define LEDPIN1 4
// #define LEDPIN2 2
// #define LEDPIN3 15 
// #define LED_PIN 27
#define DHTPIN D5          // Mention the digital pin where you connected 
#define DHTTYPE DHT22     // DHT 11  
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
#define MSG_DHT (0)
char msg[MSG_DHT];
int value = 0;



// BLYNK_WRITE(V1) { // V2 adalah pin LED yang sudah kita atur pada Blynk
// }

// BLYNK_WRITE(V2) { // V2 adalah pin LED yang sudah kita atur pada Blynk
//   digitalWrite(LEDPIN2, param.asInt()); // Mengubah nilai LED dari Blynk
// }
// BLYNK_WRITE(V3) { // V2 adalah pin LED yang sudah kita atur pada Blynk
//   digitalWrite(LEDPIN3, param.asInt()); // Mengubah nilai LED dari Blynk
// }

//setup wifi

void setup_wifi() {
  delay(10);//   digitalWrite(LEDPIN1, param.asInt()); // Mengubah nilai LED dari Blynk

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// fungsi untuk menghubungkan ke broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("LED");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// menerima data
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan Diterima [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void setup(){
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  // pinMode(LEDPIN1, OUTPUT);
  // pinMode(LEDPIN2, OUTPUT);
  // pinMode(LEDPIN3, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(DHTPIN,INPUT);
  
  dht.begin();
  // timer.setInterval(2500L, sendSensor);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(){
  Blynk.run();
  timer.run();
  if (!client.connected()) {
    reconnect();
    }

  client.loop();
  int nilaiTemperatur = dht.readTemperature();
  delay(2000);
  snprintf (msg, MSG_DHT, "%s", itoa(nilaiTemperatur,msg,10)); // itoa (konversi integer ke string)
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("Suhu", msg); // fungsi untuk publish ke broker  
}



// void sendSensor(){
 
//   float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
//   // char msg[MSG_DHT] = t;
//   if (isnan(t)) {
//     Serial.println("Failed to read from DHT sensor!");
//     return;
//   }

//   // Blynk.virtualWrite(V1, h);
//   Blynk.virtualWrite(V0, t);
//   // Menampilkan ke serial monitor
//    Blynk.virtualWrite(V0, t);
//   Serial.print("Suhu : ");
//   Serial.print(t);
//   // Serial.print("    Humidity : ");
//   // Serial.println(h);


//   if(t < 20){
//     Blynk.logEvent("notifikasi","Suhu dibawah 20 Derajat celcius");
//     digitalWrite(LEDPIN1, HIGH);
//     digitalWrite(LEDPIN2, LOW);
//     digitalWrite(LEDPIN3, LOW);
//   }
//   if( 20 < t < 27){
  
//     Blynk.logEvent("notifikasi","Suhu diantara 21 sampai 26 Derajat celcius");
//     digitalWrite(LEDPIN2, LOW);
//     digitalWrite(LEDPIN1, HIGH);
//     digitalWrite(LEDPIN3, LOW);
//   }
//   if( t > 27){
//     Blynk.logEvent("notifikasi","Suhu diatas 27 Derajat celcius");
//     digitalWrite(LEDPIN3, HIGH);
//     digitalWrite(LEDPIN1, LOW);
//     digitalWrite(LEDPIN2, LOW);
//   }
// }
