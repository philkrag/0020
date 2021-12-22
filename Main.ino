

#include <WiFi.h> // good

#include "mbedtls/aes.h" // good

//#include <iostream>
//#include <string>
//#include <cstring>

// /////////////////////////////////////////////////////////////////////// USER DEFINED VARIABLES

const char* ssid     = "Toolbox";
const char* password = "TruckHill468*";
//char* key = "abcdefghijklmnop"; // <<<<<<<<<<<<<<<<<<< need to move this at the front

IPAddress staticIP(192,168,4,10);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

// /////////////////////////////////////////////////////////////////////// SYSTEM VARIABLES

int Output_1_ReSend = 500000;

int Output_1_Index = 0;

String Input_1_Current = "-";
String Input_2_Current = "-";
String Input_3_Current = "-";

String Output_1_Current = "-";
String Output_2_Current = "-";
String Output_3_Current = "-";
String Output_4_Current = "-";

String Output_1_Set = "-";
String Output_2_Set = "-";
String Output_3_Set = "-";
String Output_4_Set = "-";

//String Line = "a"; // OLD

String Message_Buffer = "";
String Message_Received = "";
String Message_Received_Memory = "";
bool Message_Received_Flag = "";

String SerialCurrentLine = "";                                                  // String to hold incoming serial IO data
bool SerialLineComplete = false;                                              // Flag for the completion of incoming serial IO data

WiFiServer server(80);


void setup()
{

// /////////////////////////////////////////////////////////////////////// ESTABLISH SERIAL BUFFER
  
Serial.begin(9600);
SerialCurrentLine.reserve(200);
pinMode(5, OUTPUT);      // set the LED pin mode
delay(10);

// /////////////////////////////////////////////////////////////////////// ESTABLISH WIFI CONNECTION

Serial.println();
Serial.println();
Serial.print("<<<WIFI>>>Connecting to ");
Serial.println(ssid);

WiFi.config(staticIP, gateway, subnet);
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("<<<END>>>");
Serial.println("<<<WIFI>>>Connected<<<END>>>");
Serial.print("<<<WIFI>>>IP Address: ");
Serial.print(WiFi.localIP());
Serial.println("<<<END>>>");

// /////////////////////////////////////////////////////////////////////// ESTABLISH WEB SERVER

    server.begin();

}


void loop(){

WiFiClient client = server.available();                                   // Open WIFI client server

if (client) {                                                             // When a client connection is avialable
  
Serial.println("<<<CON>>>New Client<<<END>>");                            // Provide notification on serial
String EthernetCurrentLine = "";                                          // Create blank incoming 
while (client.connected()) {                                              // Loop while the client's connected
if (client.available()) {                                                 // When data is avilable
char c = client.read();                                                   // Read a individual byte

if (c != '\n') {   
Message_Buffer = Message_Buffer + String(c);
}

if (c == '\n') {                                                          // When the end of a string appears

if (EthernetCurrentLine.length() == 0) {

} else if (c != '\r') {                                                   // If you got anything else but a carriage return character,
          EthernetCurrentLine += c;                                       // Add data to ethernet incoming string.          
}


Serial.print("<<<DATA>>>");                                               // Print data out on serial line
if(Message_Buffer.length() == 1){ Serial.print("<<<EOT>>>"); }
Serial.print(Message_Buffer);
Serial.println("<<<END>>>");

Transmission_Monitoring_GET(Message_Buffer); 

if(Message_Buffer.length() == 1){

Display_Webpage(client);

break;
}

Message_Buffer = "";

}

}

}

Serial.println("<<<CON>>>Client Disconnected<<<END>>>");
Serial.println("");
}

IO_Monitoring();
  
}


// /////////////////////////////////////////////////////////////////////// ...
  










// ///////////////////////////////////////////////////////////////// FUNCTION(S)

// [FUNCTION] => DISPLAY HTML PAGE
void Display_Webpage(WiFiClient client) {
        
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");            
client.println();

client.print("<html>");
client.print("<head>");
client.print("<title>OPEN SOURCE TOOLBOX REMOTE NODE</title>");
//client.print("<meta http-equiv=\"refresh\" content=\"5; url=''\"/>");
client.print("</head>");

client.print("<style>");
client.print("body { font-family: Arial, Helventica, san-serif; }");
client.print("table, th, td { border: 1px solid black; border-collapse: collapse; }");
client.print(".Description_Column { text-align: left; padding: 2px 5px 2px 5px; }");
client.print(".Value_Column { text-align: center; padding: 2px 5px 2px 5px; }");
client.print(".Input_Column { text-align: center; padding: 2px 5px 2px 5px; }");
client.print(".Disclaimer { text-align: left; font-size:10px; padding: 2px 5px 2px 5px; }");

client.print("</style>");

client.print("<body>");

client.print("</table>");

client.print("<br>");

client.print("<table style=\"width:600px\">");
client.print("<col width=\"50px\">");
client.print("<col width=\"*\">");

client.print("<tr>");
client.print("<td>");
client.print("<img style=\"width:50px;height:50px;\" src=\"\" alt=\"\">");
client.print("</td>");

client.print("<td class=\"Disclaimer\">");
client.print("<h1 style=\"font-size:20px;\">OPEN SOURCE TOOLBOX</h1>");
client.print("</td>");
client.print("</tr>");
client.print("</table>");
client.print("<br>");

client.print("<table style=\"width:600px\">");
client.print("<col width=\"200px\">");
client.print("<col width=\"*\">");

client.print("<tr>");
client.print("<th class=\"Description_Column\">System</th>");
client.print("<td class=\"Description_Column\">Basic Control Board</td>");
client.print("</tr>");

client.print("<tr>");
client.print("<th class=\"Description_Column\">Version</th>");
client.print("<td class=\"Description_Column\">1.0</td>");
client.print("</tr>");

client.print("<tr>");
client.print("<th class=\"Description_Column\">Installation Date</th>");
client.print("<td class=\"Description_Column\"></td>");
client.print("</tr>");

client.print("</table>");
client.print("<br>");

client.print("<table style=\"width:600px\">");

client.print("<col width=\"*\">");
client.print("<col width=\"100px\">");
client.print("<col width=\"100px\">");
client.print("<col width=\"50px\">");
client.print("<col width=\"50px\">");

client.print("<tr>");
client.print("<th style=\"background-color: rgb(240,240,240);\" class=\"Description_Column\">Description</td>");
client.print("<th style=\"background-color: rgb(240,240,240);\">Current<br>Value</th>");
client.print("<th style=\"background-color: rgb(240,240,240);\">Set<br>Value</th>");
client.print("<th style=\"background-color: rgb(240,240,240);\">...</th>");
client.print("<th style=\"background-color: rgb(240,240,240);\">...</th>");
client.print("</tr>");

client.print("<tr>");
client.print("<td class=\"Description_Column\">Input 1</td>");
client.print("<td class=\"Value_Column\"><INPUT_1_VALUE>"+String(Input_1_Current)+"</INPUT_1_VALUE></td>");
client.print("<td class=\"Value_Column\">N/A</td>");
client.print("<td class=\"Input_Column\" colspan=\"2\"><a href=\"/H\">REFRESH</a></td>");
client.print("</tr>");

client.print("<tr>");
client.print("<td class=\"Description_Column\">Input 2</td>");
client.print("<td class=\"Value_Column\">"+String(Input_2_Current)+"</td>");
client.print("<td class=\"Value_Column\">N/A</td>");
client.print("<td class=\"Input_Column\" colspan=\"2\"><a href=\"/H\">REFRESH</a></td>");
client.print("</tr>");

client.print("<tr>");
client.print("<td class=\"Description_Column\">Input 3</td>");
client.print("<td class=\"Value_Column\">"+String(Input_3_Current)+"</td>");
client.print("<td class=\"Value_Column\">N/A</td>");
client.print("<td class=\"Input_Column\" colspan=\"2\"><a href=\"/H\">REFRESH</a></td>");
client.print("</tr>");

client.print("<tr>");
client.print("<td class=\"Description_Column\">Output 1</td>");
client.print("<td class=\"Value_Column\">"+String(Output_1_Current)+"</td>");
client.print("<td class=\"Value_Column\">"+String(Output_1_Set)+"</td>");
client.print("<td class=\"Input_Column\"><a href=\"/OUTPUT-1-ON\">ON</a></td>");
client.print("<td class=\"Input_Column\"><a href=\"/OUTPUT-1-OFF\">OFF</a></td>");
client.print("</tr>");

client.print("<tr>");
client.print("<td class=\"Description_Column\">Output 2</td>");
client.print("<td class=\"Value_Column\">"+String(Output_2_Current)+"</td>");
client.print("<td class=\"Value_Column\">"+String(Output_2_Set)+"</td>");
client.print("<td class=\"Input_Column\"><a href=\"/OUTPUT-2-ON\">ON</a></td>");
client.print("<td class=\"Input_Column\"><a href=\"/OUTPUT-2-OFF\">OFF</a></td>");
client.print("</tr>");

client.print("<tr>");
client.print("<td class=\"Description_Column\">Output 3</td>");
client.print("<td class=\"Value_Column\">"+String(Output_3_Current)+"</td>");
client.print("<td class=\"Value_Column\">"+String(Output_3_Set)+"</td>");
client.print("<td class=\"Input_Column\"><a href=\"/OUTPUT-3-ON\">ON</a></td>");
client.print("<td class=\"Input_Column\"><a href=\"/OUTPUT-3-OFF\">OFF</a></td>");
client.print("</tr>");

client.print("<tr>");
client.print("<td class=\"Description_Column\">Output 4</td>");
client.print("<td class=\"Value_Column\">"+String(Output_4_Current)+"</td>");
client.print("<td class=\"Value_Column\">"+String(Output_4_Set)+"</td>");
client.print("<td class=\"Input_Column\"><a href=\"/OUTPUT-4-ON\">ON</a></td>");
client.print("<td class=\"Input_Column\"><a href=\"/OUTPUT-4-OFF\">OFF</a></td>");
client.print("</tr>");

client.print("</table>");
client.print("<br>");

client.print("<table style=\"width:600px\">");
client.print("<col width=\"200px\">");
client.print("<col width=\"*\">");

client.print("<tr>");
client.print("<th class=\"Description_Column\">Service Company</th>");
client.print("<td class=\"Description_Column\"></td>");
client.print("</tr>");

client.print("<tr>");
client.print("<th class=\"Description_Column\">Service Technician</th>");
client.print("<td class=\"Description_Column\"></td>");
client.print("</tr>");

client.print("<tr>");
client.print("<th class=\"Description_Column\">Phone #</th>");
client.print("<td class=\"Description_Column\"></td>");
client.print("</tr>");

client.print("</table>");
client.print("<br>");

client.print("<table style=\"width:600px\">");
client.print("<tr>");
client.print("<td>");
client.print("<img style=\"width:100px; padding: 0px 5px 0px 5px;\" src=\"data:image/png;base64,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\" alt=\"\" />");
client.print("</td>");

client.print("<td class=\"Disclaimer\">");
client.print("<b>DISCLAIMER</b>: This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by     the Free Software Foundation, either version 3 of the License, or (at your option) any later version. <br> This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. <br>You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.");
client.print("</td>");
client.print("</tr>");
client.print("</table>");
client.print("<br>");

client.print("</body>");
client.print("</html>");

}


// [FUNCTION] => MONITORING INBOUND HTML DATA
void Transmission_Monitoring_GET(String Incoming_Message) {

// OUTPUT 1
if (Incoming_Message.indexOf("OUTPUT-1-ON") > -1) {
  Serial.println("TRIGGERED - OUTPUT-1-ON");
Output_1_Set = "ON";
//digitalWrite(5, HIGH);               // GET /H turns the LED on
}

if (Incoming_Message.indexOf("OUTPUT-1-OFF") > -1) {
    Serial.println("TRIGGERED - OUTPUT-1-OFF");
Output_1_Set = "OFF";
//digitalWrite(5, LOW);                // GET /L turns the LED off
}

}


// [FUNCTION] => MONITORING INBOUND SERIAL IO DATA
void IO_Monitoring(){

//String Incoming_Serial_String = String(SerialCurrentLine).replace("\n","");
String Incoming_Serial_String = SerialCurrentLine;


 
if(Output_1_Current != Output_1_Set && Output_1_Set != "-"){
if(Output_1_Index < Output_1_ReSend){
Output_1_Index = Output_1_Index + 1;  
}else{
Serial.println("OUTPUT-1-" + String(Output_1_Set));
Output_1_Index = 0;
}  
}

if(Output_1_Current==Output_1_Set){ Output_1_Set = "-"; }
if(Output_2_Current==Output_2_Set){ Output_2_Set = "-"; }
if(Output_3_Current==Output_3_Set){ Output_3_Set = "-"; }
if(Output_4_Current==Output_4_Set){ Output_4_Set = "-"; }

// print the string when a newline arrives:
if (SerialLineComplete) {
Serial.print("<<<SERIAL>>>");
Serial.print(Incoming_Serial_String);
Serial.println("<<<END>>>");
// clear the string:

if(Incoming_Serial_String == "INPUT-1-ON"){Input_1_Current="ON";}
if(Incoming_Serial_String == "INPUT-1-OFF"){Input_1_Current="OFF";}

if(Incoming_Serial_String == "INPUT-2-ON"){Input_2_Current="ON";}
if(Incoming_Serial_String == "INPUT-2-OFF"){Input_2_Current="OFF";}

if(Incoming_Serial_String == "INPUT-3-ON"){Input_3_Current="ON";}
if(Incoming_Serial_String == "INPUT-3-OFF"){Input_3_Current="OFF";}

if(Incoming_Serial_String == "OUTPUT-1-ON"){Output_1_Current="ON";}
if(Incoming_Serial_String == "OUTPUT-1-OFF"){Output_1_Current="OFF";}

if(Incoming_Serial_String == "OUTPUT-2-ON"){Output_2_Current="ON";}
if(Incoming_Serial_String == "OUTPUT-2-OFF"){Output_2_Current="OFF";}

if(Incoming_Serial_String == "OUTPUT-3-ON"){Output_3_Current="ON";}
if(Incoming_Serial_String == "OUTPUT-3-OFF"){Output_3_Current="OFF";}

if(Incoming_Serial_String == "OUTPUT-4-ON"){Output_4_Current="ON";}
if(Incoming_Serial_String == "OUTPUT-4-OFF"){Output_4_Current="OFF";}
    
SerialCurrentLine = "";
SerialLineComplete = false;
  }

}


// [FUNCTION] => SERIAL EVENT LISTNER (GENERAL)
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if(inChar != '\n'){
    SerialCurrentLine += inChar;
    }
    if (inChar == '\n') {
      SerialLineComplete = true;
    }
  }
}
