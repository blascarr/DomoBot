#include <DNSServer.h>

#include <ESPAsyncWebServer.h>
#include "webpage.h"

AsyncWebServer server(80);
DNSServer dns;

// Create an Event Source on /events
AsyncEventSource events( SERVER_EVENTS_ENDPOINT );

void initWebServer() {
  Serial.println("Server On");
  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", domobot_html);
  });

   server.on("/domobot", HTTP_GET, [] (AsyncWebServerRequest *request) {  
    if (request->hasParam( DATA_REQUEST_INPUT )){
      String jsonData = request->getParam(DATA_REQUEST_INPUT)->value();
      Serial.print(" JSON Data ");
      Serial.println( jsonData );
      bot.setStatus(jsonData);
      request->send(200, "text/plain", "OK");
    }
  });

   // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      //#ifdef SERIALDEBUG
        Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      //#endif
    }
    Serial.print("On Connect Event ");
  });
  
  server.addHandler(&events);
  server.begin();
}
