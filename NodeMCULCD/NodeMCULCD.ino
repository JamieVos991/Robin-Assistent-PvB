#include <ESP8266WiFi.h>       // WiFi-functionaliteit
#include <Wire.h>              // I2C-communicatie
#include <LiquidCrystal_I2C.h> // LCD via I2C
#include <ESP8266WebServer.h>  // Webserver op ESP8266

const char* ssid = "Medialab";        // WiFi SSID
const char* password = "Mediacollege"; // WiFi wachtwoord

#define I2C_ADDR 0x3F                 // I2C adres LCD

const int LCD_COLUMNS = 16;           // LCD kolommen
const int LCD_ROWS = 2;               // LCD rijen

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS); // LCD init

ESP8266WebServer server(80);          // Webserver op poort 80

String currentText = "";              // Huidige tekst voor LCD
unsigned long lastScrollTime = 0;     // Tijdstip laatste scroll
int scrollIndex = 0;                  // Scroll positie

void setup() {
  Serial.begin(115200);               // Serial communicatie
  Wire.begin(D2, D1);                 // I2C start

  lcd.init();                         // LCD initialisatie
  lcd.backlight();                    // LCD achtergrondverlichting aan
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");

  WiFi.begin(ssid, password);         // Verbinden met WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);          // Root endpoint
  server.on("/submit", HTTP_POST, handleFormSubmit); // Formulier submit endpoint

  server.begin();                    // Start webserver
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();             // Verwerk client verzoeken
  if (millis() - lastScrollTime >= 250) { // Scroll delay
    scrollText();
    lastScrollTime = millis();
  }
}

void handleRoot() {
  String html = "<html><body><form action='/submit' method='post'>"
                "Text to display: <input type='text' name='text'><br>"
                "<input type='submit' value='Submit'></form></body></html>";
  server.send(200, "text/html", html);
}

void handleFormSubmit() {
  if (server.hasArg("text")) {
    currentText = server.arg("text");
    Serial.println("Received text: " + currentText);

    scrollIndex = 0;  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(currentText.substring(0, LCD_COLUMNS));  

    server.send(200, "text/html", "Text displayed: " + currentText + "<br><a href='/'>Back</a>");
  } else {
    server.send(400, "text/html", "No text received");
  }
}

void scrollText() {
  if (currentText.length() > LCD_COLUMNS) {
    String displayText = currentText.substring(scrollIndex, scrollIndex + LCD_COLUMNS);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(displayText);
    scrollIndex++;
    if (scrollIndex + LCD_COLUMNS > currentText.length()) {
      scrollIndex = 0; 
    }
  }
}