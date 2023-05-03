
#define BLYNK_TEMPLATE_ID "TMPL6dpFQe7Y6"
#define BLYNK_TEMPLATE_NAME "posttest2"
#define BLYNK_AUTH_TOKEN "OVCBCdvdy23TkIlcfoLB-TJvP9sNue-4"

#define BLYNK_PRINT Serial

// #include <WiFi.h>
#include <PubSubClient.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;

// Update these with values suitable for your network.
const char* ssid = "Universitas Mulawarman";
const char* password = ""; 
const char* mqtt_server = "broker.hivemq.com"; // broker gratisan

//LED pin

#define LEDPIN1 D4
#define LEDPIN2 D2
#define LEDPIN3 D0 
// #define LED_PIN 27
BlynkTimer timer;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_DHT	(0)
char msg[MSG_DHT];



//Blynk
BLYNK_WRITE(V1) { // V2 adalah pin LED yang sudah kita atur pada Blynk
  digitalWrite(LEDPIN1, param.asInt()); // Mengubah nilai LED dari Blynk
}

BLYNK_WRITE(V2) { // V2 adalah pin LED yang sudah kita atur pada Blynk
  digitalWrite(LEDPIN2, param.asInt()); // Mengubah nilai LED dari Blynk
}
BLYNK_WRITE(V3) { // V2 adalah pin LED yang sudah kita atur pada Blynk
  digitalWrite(LEDPIN3, param.asInt()); // Mengubah nilai LED dari Blynk
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

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

// Fungsi untuk menerima data
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("] ");
  String data = ""; // variabel untuk menyimpan data yang berbentuk array char
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i]; // menyimpan kumpulan char kedalam string
  }
  Serial.println(" Derajat Celcius");
  int temperatur = data.toInt(); // konvert string ke int
  Blynk.virtualWrite(V0, temperatur);
  // digitalWrite(LED_PIN, HIGH);
  // if (temperatur > 20) { // pengkondisian
  //   delay(3000);
  //   digitalWrite(LED_PIN, HIGH);  // Turn the LED off by making the voltage HIGH
  // } else {
  //   delay(3000);
  //   digitalWrite(LED_PIN, LOW);  // Turn the LED off by making the voltage HIGH
  // }
  if(temperatur < 20){
    Blynk.logEvent("notifikasi","Suhu dibawah 20 Derajat celcius");
    digitalWrite(LEDPIN1, HIGH);
    digitalWrite(LEDPIN2, LOW);
    digitalWrite(LEDPIN3, LOW);
    
  }
  if( 20 < temperatur < 27){
  
    Blynk.logEvent("notifikasi","Suhu diantara 21 sampai 26 Derajat celcius");
    digitalWrite(LEDPIN2, HIGH);
    digitalWrite(LEDPIN1, LOW);
    digitalWrite(LEDPIN3, LOW);

  }
  if( temperatur > 27){
    Blynk.logEvent("notifikasi","Suhu diatas 27 Derajat celcius");
    digitalWrite(LEDPIN3, HIGH);
    digitalWrite(LEDPIN1, LOW);
    digitalWrite(LEDPIN2, LOW);
    
  }
}


// fungsi untuk mengubungkan ke broker
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
      // Once connected, publish an announcement...
      client.subscribe("Suhu");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, password);
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  // pinMode(LED_PIN, OUTPUT);     // Inisialisasi pin LED

  setup_wifi();
  client.setServer(mqtt_server, 1883); // setup awal ke server mqtt
  client.setCallback(callback); 
}

void loop() {
  Blynk.run();
  timer.run();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

