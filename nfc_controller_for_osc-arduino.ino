//built largely upon this code here: https://www.hackster.io/condorapps1/nfc-controller-for-osc-a8aef1 

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include <ArduinoOSC.h> // libary version 1.6


WiFiUDP udp;
ArduinoOSC<WiFiUDP> osc; 

////////////////set here the IP and the port of the receiving computer////////////
///////////////The port is usually indicated in the software you want to controll////
//////////////The ip can be left as it is if you use AP mode, but if not make sure you input the computer IP////////////////

const int recv_port = 57120;
const int send_port = 57120;
const char* host = "192.168.0.2";



///////////////////Set the ID for the NFC Tags Here/////////////////
/*const char* tag1 = "04 54 36 DA 57 5D 81";
const char* tag2 = "04 4C 36 DA 57 5D 81";
const char* tag3 = "04 5B 3F DA 57 5D 80";
const char* tag4 = "04 63 3F DA 57 5D 80";
const char* tag5 = "04 3C 69 DA 57 5D 81";
const char* tag6 = "04 74 3F DA 57 5D 80";
const char* tag7 = "04 53 3F DA 57 5D 80";
const char* tag8 = "04 4B 3F DA 57 5D 80";
const char* tag9 = "04 40 3F DA 57 5D 80";
const char* tag10 = "04 3F 40 DA 57 5D 80"; */

const char* tag1 = "4 84 54 218";
const char* tag2 = "4 76 54 218";
const char* tag3 = "4 91 63 218";
const char* tag4 = "4 99 63 218";
const char* tag5 = "4 60 105 218";
const char* tag6 = "4 116 63 218";
const char* tag7 = "4 83 63 218";
const char* tag8 = "4 75 63 218";
const char* tag9 = "4 64 63 218";
const char* tag10 = "4 63 64 218";

String previousTag = "";

////////////////Set OSC Commands here//////////////////

const char* command1 =  "/oscControl/grid/1";

const char* command2 =  "/oscControl/grid/2";

const char* command3 =  "/oscControl/grid/3";

const char* command4 =  "/oscControl/grid/4";

////Failsafe - if an unregistred tag is detected will send this command///
const char* other =  "/oscControl/grid/4";



//////////////////Define your SSID and Password - if on AP mode this will be used as settings for AP if in Wifi mode this will be used to connect to an existing WiFi network//////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* ssid = "JEUSONORE";
const char* password =  "sousverre";
 


////Wifi stuff
WiFiServer server(80);
IPAddress IP(192,168,0,1);
IPAddress mask = (255, 255, 255, 0);

///nfc stuff
const int resetPin = 22; // Reset pin
const int ssPin = 21;    // Slave select pin
MFRC522 rfid = MFRC522(ssPin, resetPin); // Create instance
int code[] = {69,141,8,136}; //This is the stored UID
int codeRead = 0;
String uidString;
int selectedChannel = 0;



void setup() {

  ///serial setup
  Serial.begin(115200);
 
  SPI.begin();   
 
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();  


/*
//////////////Use this code if you want to connect the ESP32 to an existing WIFI/////////////
////////////////////////////////////////////////////////////////////////////////////////////


  /////Wifi connect

   WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
   
    delay(500);
    Serial.println("Connecting to WiFi..");
 
  }
 
  Serial.println("Connected to the WiFi network");
*/

//////////////Use this code if you want to enable ESP32 AP mode and create a WiFI network/////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

 WiFi.mode(WIFI_AP);
 WiFi.softAP(ssid, password);
 WiFi.softAPConfig(IP, IP, mask);
 server.begin();

Serial.println();
Serial.println("Server started.");
Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());



/////osc init
osc.begin(udp, recv_port);
}

void loop() {
//////Uncomment next line if you use the AP mode
runWifi();
runRfid();

}



void runWifi(){

  WiFiClient client = server.available();
  if (!client) {return;}
  
  String request = client.readStringUntil('\r');
  client.flush();

}

void runRfid(){

 // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card. PICC_HaltA() is automatically called.
   uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);

Serial.println("Detected:");
Serial.println(uidString);

///////You can use this code if all you need is to just send the next osc command when a new NFC is detected////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//if(uidString != previousTag){
//
//
//  previousTag = uidString;
//  if(selectedChannel == 6){
//
//    selectedChannel = 1;
//  }
//  else{
//
//  selectedChannel++;
//  }
//  sendOscSignal(selectedChannel);
//  
//}

///////////////////////////Use this code if you want to control the sent OSC Command When a certain NFC Tag is detected/////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if(uidString == tag1){

  if(selectedChannel != 1){
  selectedChannel =  1;
  sendOscSignal(selectedChannel);
  }
}

else if(uidString == tag2){

  if(selectedChannel != 2){
  selectedChannel =  2;
  sendOscSignal(selectedChannel);
  }
}

else if(uidString == tag3){

  if(selectedChannel != 3){
  selectedChannel =  3;
  sendOscSignal(selectedChannel);
  }
}

else if(uidString == tag4){

  if(selectedChannel != 4){
  selectedChannel =  4;
  sendOscSignal(selectedChannel);
  }
}


else if(uidString == tag5){

  if(selectedChannel != 5){
  selectedChannel =  5;
  sendOscSignal(selectedChannel);
  }
}
 
else if(uidString == tag6){

  if(selectedChannel != 6){
  selectedChannel =  6;
  sendOscSignal(selectedChannel);
  }
}

else if(uidString == tag7){

  if(selectedChannel != 7){
  selectedChannel =  7;
  sendOscSignal(selectedChannel);
  }
}


else if(uidString == tag8){

  if(selectedChannel != 8){
  selectedChannel =  8;
  sendOscSignal(selectedChannel);
  }
}
 
else if(uidString == tag9){

  if(selectedChannel != 9){
  selectedChannel =  9;
  sendOscSignal(selectedChannel);
  }
} 
 
else if(uidString == tag10){

  if(selectedChannel != 10){
  selectedChannel =  10; // this shall appear in serial monitor in arduino ide
  sendOscSignal(selectedChannel);
  }
} 
 
 delay(500); 
  
}

void sendOscSignal(int v){
Serial.println("Preparing OSC:");
 OSCMessage msg;
    msg.beginMessage(host, send_port);

if(v == 1){
Serial.println("sending v1:");
 msg.setOSCAddress(command1);
 msg.addArgInt32(1); //this value is OSC message value
  Serial.println("Sent v1:");
}
else if(v == 2){
Serial.println("sending v2:");
 msg.setOSCAddress(command1);
 msg.addArgInt32(1);
  Serial.println("Sent v2:");
}

else if(v == 3){
Serial.println("sending v3:");
 msg.setOSCAddress(command1);
 msg.addArgInt32(1);
  Serial.println("Sent v3:");
}

else if(v == 4){
Serial.println("sending v4:");
 msg.setOSCAddress(command2);
 msg.addArgInt32(1);
  Serial.println("Sent v4:");
}

else if(v == 5){
Serial.println("sending v5:");
 msg.setOSCAddress(command2);
 msg.addArgInt32(1);
  Serial.println("Sent v5:");
}

else if(v == 6){
Serial.println("sending v6:");
 msg.setOSCAddress(command2);
 msg.addArgInt32(1);
  Serial.println("Sent v6:");
}

else if(v == 7){
Serial.println("sending v7:");
 msg.setOSCAddress(command3);
 msg.addArgInt32(1);
  Serial.println("Sent v7:");
}

else if(v == 8){
Serial.println("sending v8:");
 msg.setOSCAddress(command3);
 msg.addArgInt32(1);
  Serial.println("Sent v8:");
}

else if(v == 9){
Serial.println("sending v9:");
 msg.setOSCAddress(command3);
 msg.addArgInt32(1);
  Serial.println("Sent v9:");
}

else if(v == 10){
Serial.println("sending v10:");
 msg.setOSCAddress(command4);
 msg.addArgInt32(1);
  Serial.println("Sent v10:");
}



else{

  Serial.println("sending other:");
 msg.setOSCAddress(command4);
 msg.addArgInt32(1);
  Serial.println("Sent other:");
}

osc.send(msg);
}
