#include <WiFi.h>
#include <HTTPClient.h>
#include <FirebaseESP32.h>

// --- WI-FI CONFIGURATION ---
const char* ssid = "1st FLOOR 2";
const char* password = "osamakhan@5";

// --- FIREBASE CREDENTIALS ---
#define FIREBASE_HOST "ahmad-ab501-default-rtdb.firebaseio.com" // Replace with your DB URL
#define FIREBASE_AUTH "YourDatabaseSecretOrLeaveBlankIfTestMode"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// --- HARDWARE PIN ---
const int pinLED = 8; // Onboard or external LED pin D8

void setup() {
    Serial.begin(115200);
    pinMode(pinLED, OUTPUT);
    digitalWrite(pinLED, LOW);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi Connected!");

    // Configure Firebase
    config.database_url = FIREBASE_HOST;
    config.signer.test_mode = true; // For test mode rules

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void loop() {
    // Check if Wi-Fi dropped and reconnect automatically
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        }
    }

    // Read state from Firebase database path "/control/led"
    if (Firebase.ready()) {
        if (Firebase.getInt(fbdo, "/control/led")) {
            int ledState = fbdo.intData();
            digitalWrite(pinLED, ledState);
            Serial.printf("LED State Updated from Cloud: %d\n", ledState);
        } else {
            Serial.println("Failed to read Firebase path: " + fbdo.errorReason());
        }
    }

    delay(1000); // Poll every 1 second
}