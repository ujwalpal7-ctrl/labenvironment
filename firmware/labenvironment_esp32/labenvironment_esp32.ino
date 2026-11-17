/*
  LabEnvironment - Firmware ESP32 + DHT22 (ETAPA PLANIFICADA, no probado en hardware)
  Responsable: Vicente Vergara (Hardware/Instrumentación) · Software: Dimitry Quezada

  Conexiones:
    DHT22 pin 1 (VCC)  -> 3V3
    DHT22 pin 2 (DATA) -> GPIO 4  (con resistencia pull-up de 10 kΩ a 3V3 si el sensor no es módulo)
    DHT22 pin 4 (GND)  -> GND

  Librerías (Gestor de librerías del Arduino IDE):
    - "DHT sensor library" (Adafruit)
    - "Adafruit Unified Sensor"
  Placa: "ESP32 Dev Module".

  Uso: conectar el smartphone a la red Wi-Fi "LabEnvironment-ESP32" y abrir http://192.168.4.1/data
*/
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

const char* AP_SSID = "LabEnvironment-ESP32";
const char* AP_PASS = "labenv1234";          // mínimo 8 caracteres
const unsigned long INTERVALO_MS = 2000;     // DHT22: máx. 0,5 Hz (1 lectura cada 2 s)

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);
float temperatura = NAN, humedad = NAN;
unsigned long ultimaLectura = 0;

void handleData() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  if (isnan(temperatura) || isnan(humedad)) {
    server.send(503, "application/json", "{\"error\":\"sensor sin lectura\"}");
    return;
  }
  char buf[96];
  snprintf(buf, sizeof(buf), "{\"t\":%.1f,\"h\":%.1f,\"ms\":%lu}", temperatura, humedad, millis());
  server.send(200, "application/json", buf);   // t en °C, h en % HR
}

void handleRoot() {
  server.send(200, "text/plain", "LabEnvironment ESP32 activo. Datos en /data");
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("Punto de acceso listo. IP: ");
  Serial.println(WiFi.softAPIP());            // normalmente 192.168.4.1
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
  if (millis() - ultimaLectura >= INTERVALO_MS) {
    ultimaLectura = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();          // °C
    if (!isnan(h) && !isnan(t)) { humedad = h; temperatura = t; }
    else Serial.println("Error de lectura del DHT22");
  }
}
