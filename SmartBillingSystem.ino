#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* serverUrl = "http://10.224.248.61:5000/api/bills";

const int numItems = 10;
const float prices[numItems] = {15.00, 20.00, 25.00, 30.00, 35.00, 40.00, 45.00, 50.00, 55.00, 60.00};
const char* items[numItems] = {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6", "Item 7", "Item 8", "Item 9", "Item 10"};

WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nSmart Billing System - ESP8266");
  Serial.println("Initializing...");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Server: ");
    Serial.println(serverUrl);
  } else {
    Serial.println("\nWiFi connection failed");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    sendBillData();
    delay(10000);
  } else {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    WiFi.reconnect();
    delay(5000);
  }
}

void sendBillData() {
  int itemCount = random(1, 6);
  float totalAmount = 0;
  
  String jsonPayload = "{\"customerName\":\"Customer #" + String(random(1000, 9999)) + "\",\"items\":[";
  
  for (int i = 0; i < itemCount; i++) {
    int itemIndex = random(0, numItems);
    int quantity = random(1, 4);
    float subtotal = prices[itemIndex] * quantity;
    totalAmount += subtotal;
    
    jsonPayload += "{\"name\":\"" + String(items[itemIndex]) + "\",\"price\":" + String(prices[itemIndex], 2) + ",\"quantity\":" + String(quantity) + ",\"subtotal\":" + String(subtotal, 2) + "}";
    
    if (i < itemCount - 1) {
      jsonPayload += ",";
    }
  }
  
  jsonPayload += "],\"totalAmount\":" + String(totalAmount, 2) + "}";
  
  Serial.println("📤 Sending bill data...");
  Serial.print("Payload: ");
  Serial.println(jsonPayload);
  
  if (http.begin(client, serverUrl)) {
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(jsonPayload);
    String response = http.getString();
    
    Serial.print("Response Code: ");
    Serial.println(httpCode);
    Serial.print("Response: ");
    Serial.println(response);
    
    if (httpCode == 201) {
      Serial.println("✅ Bill successfully sent to server!");
    } else if (httpCode > 0) {
      Serial.print("⚠️ Server responded with code: ");
      Serial.println(httpCode);
    } else {
      Serial.print("❌ HTTP error: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
    
    http.end();
  } else {
    Serial.println("❌ Could not connect to server");
  }
}
