#include <SPI.h>
#include <WiFi101.h>
#include <mcp_can.h> // Include CAN bus library, make sure to install this library first

// Set CAN bus parameters 
#define CAN_CS_PIN 10 // CAN bus chip select pin
MCP_CAN CAN(CAN_CS_PIN); // Initialize CAN bus object

// WiFi credentials 
char ssid[] = "BAH-2G";
char pass[] = "edwin-carl-james";

// Destination IP and port
IPAddress serverIP(192, 168, 1, 34);
int serverPort = 8888;

// Set the MAC address for the board if needed
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Define the CAN message structure 
struct CAN_Message {
    unsigned int id; // Arbitration ID (CAN ID)
    unsigned char data[8]; // Data bytes
    unsigned char length; // Data length (in bytes)
};

// Timer variables 
unsigned long previousMillis = 0;
const long interval = 40; // Interval in milliseconds (25 frames per second)

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

    // Initialize CAN bus
    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN bus initialized");
    } else {
        Serial.println("Error initializing CAN bus");
        while (1); // Halt if initialization fails
    }
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        // Send CAN message with brute-forced values for 005#
        for (unsigned long long i = 0; i <= 9999999999999999; i++) {
            char dataString[17]; // 16 digits + null terminator
            sprintf(dataString, "%016llu", i); // Format the value as a string with leading zeros
            char message[24]; // "005#" + 16 digits + null terminator
            sprintf(message, "005#%s", dataString);
            CAN.sendMsgBuf(0x005, 0, strlen(message), (byte*)message); // Convert message to byte array
            Serial.println("Sent message: ");
            Serial.println(message);

            // Send dominant bits message for 026#
            char dominantMessage[] = "026#FFFFFFFFFFFFFFFF"; // Dominant bits message
            CAN.sendMsgBuf(0x026, 0, strlen(dominantMessage), (byte*)dominantMessage); // Convert message to byte array
            Serial.println("Sent dominant message: ");
            Serial.println(dominantMessage);

            delay(40); // Maintain 25 frames per second
        }
        previousMillis = currentMillis;
    }
}
