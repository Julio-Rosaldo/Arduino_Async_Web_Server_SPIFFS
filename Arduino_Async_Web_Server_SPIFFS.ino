// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Setting network credentials
const char* ssid = "Red Rosaldo Pacheco";
const char* password = "R3dCh1ll1H34v3n";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// GPIOs constants
const char* input_parameter1 = "output";
const char* input_parameter2 = "state";

// Set GPIOs
const int Light1 = 2;
String Light1State = "apagada";
const int Light2 = 23;
String Light2State = "apagada";
const int Light3 = 22;
String Light3State = "apagada";

// SET DHT GPIO
DHT dht(13, DHT11);

// current temperature & humidity, updated in loop()
boolean readSensor = false;
float temperature = 0.0;
float humidity = 0.0;
float thermal = 0.0;

// Timming variables
unsigned long previousMillis = 0;
const long interval = 2000;



String setLight(String light, String state) {
  if (state == "1") {
    if (light == "2")
    {
      Light1State = "encendida";
      return Light1State;
    }
    else if (light == "23")
    {
      Light2State = "encendida";
      return Light2State;
    }
    else if (light == "22")
    {
      Light3State = "encendida";
      return Light3State;
    }
    else
    {
      return "Not found";
    }
  }
  else if (state == "0") {
    if (light == "2")
    {
      Light1State = "apagada";
      return Light1State;
    }
    else if (light == "23")
    {
      Light2State = "apagada";
      return Light2State;
    }
    else if (light == "22")
    {
      Light3State = "apagada";
      return Light3State;
    }
    else
    {
      return "Not found";
    }
  }
}

// Replaces placeholder with LED state value
String processor(const String& var) {
  if (var == "LIGHT1") {
    return Light1State;
  }
  if (var == "LIGHT2") {
    return Light2State;
  }
  else if (var == "LIGHT3") {
    return Light3State;
  }
  else if (var == "TEMPERATURE") {
    return String(temperature);
  }
  else if (var == "HUMIDITY") {
    return String(humidity);
  }
  else if (var == "THERMAL") {
    return String(thermal);
  }
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(02, OUTPUT);
  digitalWrite(02, LOW);
  pinMode(23, OUTPUT);
  digitalWrite(25, LOW);
  pinMode(22, OUTPUT);
  digitalWrite(27, LOW);

  //Start DHT Sensor
  dht.begin();

  readDHT();

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    //request->send(SPIFFS, "/index.html", "text/html", false); // Fuente, ruta, formato, descargable, placeholder filler
    request->redirect("/index.html");
  });

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(404, "text/plain", "Not found");
  });

  // Route to set user menu
  server.on("/menu.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/menu.js", "text/javascript");
  });

  // Route to load style.css file
  server.on("/menu-style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/menu-style.css", "text/css");
  });

  // Route to load Bootstrap css file
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });

  // Route to load JQuery js file
  server.on("/jquery-3.3.1.slim.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/jquery-3.3.1.slim.min.js", "text/javascript");
  });

  // Route to load Popper js file
  server.on("/popper.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/popper.min.js", "text/javascript");
  });

  // Route to load Bootstrap js file
  server.on("/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/bootstrap.min.js", "text/javascript");
  });


  // Click Events

  // Route to set user index click
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html", false);
  });

  // Route to set user wheater click
  server.on("/wheater.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/wheater.html", "text/html", false, processor);
  });

  // Route to load wheater.js file
  server.on("/wheater.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/wheater.js", "text/javascript");
  });

  // Route to load style.css file for wheater
  server.on("/wheater-style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/wheater-style.css", "text/css");
  });

  // Route to set user light click
  server.on("/light.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("Light");
    request->send(SPIFFS, "/light.html", "text/html", false, processor);
  });

  // Route to load light.js file
  server.on("/light.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/light.js", "text/javascript");
  });

  // Route to load style.css file for light
  server.on("/light-style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/light-style.css", "text/css");
  });

  // Route to set user about click
  server.on("/about.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/about.html", "text/html", false);
  });


  // AJAX Events

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    readSensor = true;
    request->send(200, "text/plain", String(temperature).c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(humidity).c_str());
  });

  server.on("/thermal", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(thermal).c_str());
  });

  //POST read for form urlencoded params
  server.on("/postupdate", HTTP_POST, [](AsyncWebServerRequest * request) {
    String inputMessage1;
    String inputMessage2;

    int count = 0;

    int paramsCount = request->params();
    for (int i = 0; i < paramsCount; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == input_parameter1) {
        inputMessage1 = p->value().c_str();
        count++;
        Serial.printf("GOT PARAM 1 \n");
      }
      else if (p->name() == input_parameter2) {
        inputMessage2 = p->value().c_str();
        count++;
        Serial.printf("GOT PARAM 2 \n");
      }
    }

    if (count = 2) {
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }

    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);

    request->send(200, "text/plain", setLight(inputMessage1, inputMessage2).c_str());
  });

  //POST read for form urlencoded params
  server.on("/posturl", HTTP_POST, [](AsyncWebServerRequest * request) {
    // The following print statements work + removing them makes no difference
    // This is displayed on monitor "Content type::application/x-www-form-urlencoded"
    Serial.print("Content type::");
    Serial.println(request->contentType());
    Serial.println("OFF hit.");

    int params = request->params();
    Serial.printf("%d params sent in\n", params);
    for (int i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isFile())
      {
        Serial.printf("_FILE[%s]: %s, size: %u", p->name().c_str(), p->value().c_str(), p->size());
      }
      else if (p->isPost())
      {
        Serial.printf("_POST[%s]: %s \n", p->name().c_str(), p->value().c_str());
      }
      else
      {
        Serial.printf("_GET[%s]: %s", p->name().c_str(), p->value().c_str());
      }
    }
  });

  //POST read for form json payload
  server.on("/postjson", HTTP_POST, [](AsyncWebServerRequest * request) {}, NULL,
  [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    // The following print statements work + removing them makes no difference
    // This is displayed on monitor "Content type::application/x-www-form-urlencoded"
    Serial.print("Content type::");
    Serial.println(request->contentType());
    Serial.println("OFF hit.");


    for (size_t i = 0; i < len; i++) {
      Serial.write(data[i]);
    }

    Serial.println();

    request->send(200);
  });

  // Light change click
  server.on("/setlightold", HTTP_GET, [](AsyncWebServerRequest * request) {
    String light = request->arg("Light");
    String order = request->arg("Order");
    request->send(200, "text/plain", setLight(light, order).c_str());
  });

  // Start server
  server.begin();
}

void readDHT() {
  Serial.print("Read: ");
  Serial.println(readSensor);

  float newT = dht.readTemperature();
  // For read temperature as Fahrenheit use: float newT = dht.readTemperature(true);
  float newH = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  if (!isnan(newT)) {
    temperature = newT;
  }

  if (!isnan(newH)) {
    humidity = newH;
  }


  /*
    thermal = -8.78469476 + 1.61139411*(float)temperature +
      2.338548839*(float)humidity - 0.14611605*(float)temperature*(float)humidity -
      0.012308094*powf((float)temperature,2.0) - 0.016424828*powf((float)humidity,2.0) +
      0.002211732*powf((float)temperature,2.0)*(float)humidity +
      0.00072546*(float)temperature*powf((float)humidity,2.0) -
      0.000003582*powf((float)temperature,2.0)*powf((float)humidity,2.0);

    thermal = round(thermal);
  */

  thermal = round(dht.computeHeatIndex(temperature, humidity, false));
  Serial.print("Thermal: ");
  Serial.println(thermal);
}

void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval && readSensor == true) {
    readDHT();
    previousMillis = currentMillis;
    readSensor = false;
  }

}
