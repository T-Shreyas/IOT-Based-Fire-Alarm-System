#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>

// Initialize Telegram BOT
#define BOTtoken "*****************************************************"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "XXXXXXXXX"

int led = 13; // Connected to D7 pin of NodeMCU
int flame_sensor = 12; //Connected to D6 pin of NodeMCU
int buzzer = 4; //Connected to D2 pin of NodeMCU
int relay = 5; //Connected to D1 pin of NodeMCU
int smoke_sensor = A0; //Connected to A0 pin of NodeMCU
int temp_smoke_sensor;
int flame_sensor_read;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "s2AKNm-gUIZ3TGhsUGhdvGBHO3NBee6x"; // Enter Auth Token

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "XXX"; //Enter the wifi name
char pass[] = "YYY";// Enter the wifi password

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(flame_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(smoke_sensor, INPUT);
  
  digitalWrite(led, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(relay, LOW);
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() 
{
  flame_sensor_read = digitalRead(flame_sensor); //reading the sensor data on A0 pin
  Blynk.virtualWrite(V0, flame_sensor_read); //sending data to Blynk
  Serial.print("Flame Status:");
  Serial.println(flame_sensor_read);

  int led_status = digitalRead(led);
  if (led_status == HIGH)
  {
   Blynk.virtualWrite(V1, 255);
  }
  else
  {
   Blynk.virtualWrite(V1, 0);
  }


if (flame_sensor_read == 0)
    {
    //Blynk.virtualWrite(V1, 255);
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(relay, HIGH);
    Blynk.notify("Alert Fire Detected");
    bot.sendMessage(CHAT_ID, "Fire Alert!!", "");
    //Serial.println("Motion Detected");  
    Blynk.email("t.shreyas2001@gmail.com","Alert Fire Detected","Attention Required"); 
    }
  else
      {
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
      digitalWrite(relay, LOW);
      }
      delay(500);
      Blynk.run();
  
  temp_smoke_sensor = analogRead(smoke_sensor);
  Blynk.virtualWrite(V2, temp_smoke_sensor);
  Serial.print("Current Gas Level:");
  Serial.println(temp_smoke_sensor);
  if (temp_smoke_sensor > 100)
  {
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(relay, HIGH);
    Blynk.notify("Alert Smoke Detected");
    bot.sendMessage(CHAT_ID, "Fire Alert!!", "");
    Blynk.email("t.shreyas2001@gmail.com","Alert Smoke Detected","Attention Required"); 
    }
  else
    {
     digitalWrite(led, LOW);
     digitalWrite(buzzer, LOW);
     digitalWrite(relay, LOW);
    }
}
