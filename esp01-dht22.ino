#include <SimpleDHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// for DHT22, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT22 = 2;
const char* ssid = "WIFI-SSID";
const char* password = "WIFI-PASSWORD";
const char* mqtt_server = "mqtt-IPADDRESS";
const char* clientID = "test";

SimpleDHT22 dht22(pinDHT22);
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.println("WIFI not connect");
      delay(1000);
  }
  Serial.println("WiFi connected");  
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived ["); Serial.print(topic);   Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }  

}

void reconnect() {
  // 循环，直到我们重新连接
  while (!client.connected()) {
    
    if (client.connect(clientID)) {               //connected
      Serial.println("MQTT连接成功");                 // 连接mqtt成功
      client.publish("ESP_01tem", "hello world");  // 一旦连接，发布公告…
      client.subscribe("ESP_01tem");               // 一旦连接，发布主题
    } else {
      Serial.println("MQTT连接失败");
      delay(5000);                                 // 等待5秒再重试
    }
  }
}



void loop() {
  // read without samples.
  // @remark We use read2 to get a float data, such as 10.1*C
  //    if user doesn't care about the accurate data, use read to get a byte data, such as 10*C.
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(2000);
    return;
  }
   String tmp = "temperature:";            //字符串拼接
   tmp += String(temperature);
   tmp += "*C";
   tmp += String(" humidity:");
   tmp += String(humidity);
   tmp += "RH%";
  Serial.print("Data OK: ");
  Serial.print((float)temperature); Serial.print(" *C, ");
  Serial.print((float)humidity); Serial.println(" RH%");
  client.publish("ESP_01tem", tmp.c_str());
  // DHT22 sampling rate is 0.5HZ.
  delay(2500);
}
