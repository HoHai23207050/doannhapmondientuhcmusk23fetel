#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
int ID[4]={0,0,0,0};
int PS[4]={35,254,247,167};
int p = 0;
bool L=0,L2=0;    
constexpr uint8_t RST_PIN = D3;    
constexpr uint8_t SS_PIN = D4;     
MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;
WiFiClient client;
WiFiServer server(80);
const char* ssid = "NGGYU";  
const char* password = "321123321";    
String  data =""; 
int Relay4 = D1;     
void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

void setup()
{
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(D1,LOW);
  digitalWrite(D2,HIGH);
  SPI.begin(); 
  rfid.PCD_Init(); 
  Serial.begin(115200);
  connectWiFi();
  server.begin();
}

void loop()
{
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  client = server.available();
  if (client){
    data = checkClient ();
    if(data=="RELAY4SUCCESS")L=1;
  }
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( rfid.PICC_ReadCardSerial()){ 
    for(byte i = 0; i < 4 ; i++)
      p+=rfid.uid.uidByte[i];
  }
  if((p==350) || (p==550) || (p==632) || (p==573)){
    L2=1;p=0;
    }else {
    for(byte i =0; i<10;i++){
      tone(D8,3000);
      digitalWrite(D2,LOW);
      delay(250);
      tone(D8,0);
      digitalWrite(D2,HIGH);
      delay(250);
      }
      p=0;
    }
  if(L&&L2){
    tone(D8,3000);
    delay(500);
    tone(D8,0);
    digitalWrite(D1,HIGH);
    digitalWrite(D0,HIGH);
    digitalWrite(D2, LOW);
    delay(3000);
    digitalWrite(D1,LOW);
    digitalWrite(D0,LOW);
    digitalWrite(D2, HIGH);
    L=0;L2=0;
  }
}