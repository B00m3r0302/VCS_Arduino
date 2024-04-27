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

// Variables to store the previous message
unsigned char prev_len = 0;
unsigned char prev_buf[8];
bool has_prev_message = false;

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
  // put your main code here, to run repeatedly:
  // Connect to the server
  if (!client.connected()) {
    Serial.print("Connecting to server: ");
    Serial.print(serverIP);
    Serial.print(":");
    Serial.println(serverPort);

    if (client.connect(serverIP, serverPort)) {
      Serial.println("Connected to server");
    } else {
      Serial.println("Connection failed");
      delay(5000);
      return;
    }
  }

  // Read from CAN bus
  unsigned char len = 0;
  unsigned char buf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(NULL, &len, buf);
    Serial.print("Received message from CAN bus:");

    // Color-coded and labeled output 
    // Extracting CAN ID from the first three bytes of the message 
    unsigned long canId = 0;
    for (int i = 0; i < 3; i++) {
      canId |= ((unsigned long)buf[i] << (16 - i * 8));
    }
    Serial.print("\x1B[34mID: "); // ID label in blue
    Serial.print("#");
    Serial.println(canId);

    Serial.print("\x1B[32mData Length: "); // Data Length label in green
    Serial.println(len);

    Serial.print("\x1B[33mData: "); // Data label in yellow
    for (int i = 0; i < len; i++) {
        // Color-coded data bytes
        Serial.print("[");
        Serial.print(buf[i]);
        Serial.print("]");
        if (i < len -1) {
            Serial.print(", ");
        }
    }
    Serial.println();

    // Compare with previous message if available
    if (has_prev_message && prev_len == len) {
        Serial.println("\x1B[31mChanges from previous message:");

        for (int i = 0; i < len; i++) {
            if (buf[i] != prev_buf[i]) {
                // Highlight changes in red
                Serial.print("\x1B[31m[");
                Serial.print(buf[i]);
                Serial.print("]");
            } else {
                Serial.print("[");
                Serial.print(buf[i]);
                Serial.print("]");
            }
            if (i < len -1) {
                Serial.print(", ");
            }
        }
        Serial.println();
    }

    // Store current message as previous message
    prev_len = len;
    memcpy(prev_buf, buf, sizeof(buf));
    has_prev_message = true;
    
    // Reset text color
    Serial.print("\x1B[0m");

    // Send data over the TCP/IP connection
    if (client.connected()) {
        // Send CAN data to the server
        client.write(buf, len);
        Serial.println("Data sent to server");
    }
  }

  delay(1000); // Adjust delay as needed
}