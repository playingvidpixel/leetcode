/*
 * 31 mar 2015
 * This sketch display UDP packets coming from an UDP client.
 * On a Mac the NC command can be used to send UDP. (nc -u 192.168.1.101 2390). 
 *
 * Configuration : Enter the ssid and password of your Wifi AP. Enter the port number your server is listening on.
 *
 */

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

extern "C" {  //required for read Vdd Voltage
#include "user_interface.h"
  // uint16 readvdd33(void);
}

int status = WL_IDLE_STATUS;
const char* ssid = "iot";  //  your network SSID (name)
const char* pass = "micromax";       // your network password

unsigned int localPort = 12345;      // local port to listen for UDP packets

byte packetBuffer[512]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const int motor = D1;
const int LED = D2;
void setup()
{
  // Open serial communications and wait for port to open:
  pinMode(motor, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(motor,LOW);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // setting up Station AP
  WiFi.begin(ssid, pass);
  
  // Wait for connect to AP
  Serial.print("[Connecting]");
  Serial.print(ssid);
  int tries=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries > 30){
      break;
    }
  }
  Serial.println();


printWifiStatus();

  Serial.println("Connected to wifi");
  Serial.print("Udp server started at port ");
  Serial.println(localPort);
  Udp.begin(localPort);
}

void loop()
{
  digitalWrite(LED,LOW); 
  int noBytes = Udp.parsePacket();
  String received_command = "";
  
  if ( noBytes ) {
    Serial.print(millis() / 1000);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.println();
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,noBytes); // read the packet into the buffer
    // display the packet contents in HEX
    for (int i=1;i<=noBytes;i++)
    {
    //  Serial.print(packetBuffer[i-1],HEX);
      received_command = received_command + char(packetBuffer[i - 1]);
    
    if(char(packetBuffer[i - 1] == 'a'))
    {
     Serial.print("motor on"); 
     digitalWrite(motor,HIGH);
    }
    if(char(packetBuffer[i - 1] == 'b'))
    {
     Serial.print("motor off"); 
     digitalWrite(motor,LOW);
    }
     
      if (i % 32 == 0)
      {
        Serial.println();
      }
      else Serial.print(' ');
    } // end for
//   Udp.remoteIP() = 192.168.1.37;
//    Serial.println("sending");
//  //  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//    Udp.beginPacket("192.168.1.37",12345);
//    Udp.write("data received");
//    Udp.print(system_get_chip_id());
//    Udp.write("thank you");
//    Udp.println(WiFi.localIP());
//    Udp.endPacket();
    Serial.print("received_command = ");
    Serial.println(received_command);
 //   Serial.println();
    delay(1000);
  //} // end if

  }
   digitalWrite(LED,HIGH);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
