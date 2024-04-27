#include <WiFi101.h>
#include <SPI.h>
#include <mcp_can.h> // Include CAN bus library, make sure to install this library first

// WiFi credentals 
char ssid[] = "BAH-2G";
char pass[] = "edwin-carl-james";

// Destination IP and port
IPAddress serverIP(192, 168, 1, 34);
int serverPort = 8888;

// Set the MAC address for the board if needed
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Set the CAN bus parameters 
#define CAN_CS_PIN 10 // CAN bus chip select pin
MCP_CAN CAN(CAN_CS_PIN); // Initialize CAN bus object

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status () != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize CAN bus
  if(CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN bus initialized");
  } else {
    Serial.println("Error initializing CAN bus");
    while(1); // Halt if initialization fails
  }
}

void loop() {
    // Wait for CAN messages from the server
    if (client.available()) {
        String message = client.readStringUntil('\n'); //Read message until newline
        Serial.println("Received message from server: " + message);

        // Extract CAN ID and Data from the message
        int separatorIndex = message.indexOf('#');
        if (separatorIndex != -1) {
            String idString = message.substring(0, separatorIndex);
            String dataString = message.substring(separatorIndex + 1);

            unsigned int id = idString.toInt(); // Convert ID string to integer
            unsigned char data[8]; // Buffer to store data bytes 
            int dataLength = dataString.length();
            for (int i = 0; i < dataLength && i < 8; i++) {
                data[i] = dataString.charAt(i); // Store data bytes in buffer
            }

            // Send CAN message 5 times
            for (int i = 0; i < 5; i++) {
                CAN.sendMsgBuf(id, 0, dataLength, data);
                delay(100); // Adjust the delay as needed
            }
        }
    }

    // Check if the connection is still alive
    if (!client.connected()) {
        Serial.println("Connection to server lost");
        while(1); // Halt if the connection is lost
    }
}