void initWebServer() {
  DUMPSLN("Server On");
  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", domobot_html);
  });

   server.on( DOMOBOT_ENDPOINT, HTTP_GET, [] (AsyncWebServerRequest *request) {  
    if (request->hasParam( DATA_REQUEST_INPUT )){
      String jsonData = request->getParam(DATA_REQUEST_INPUT)->value();
      DUMP(" JSON Data ", jsonData );
      bot.setStatus(jsonData);
      request->send(200, "text/plain", "OK");
    }
  });

   // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      //#ifdef SERIALDEBUG
        DUMPF("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      //#endif
    }
    DUMPSLN("On Connect Event ");
  });
  
  server.addHandler( &events );
  server.addHandler( &botmap_events );
  server.begin();
}
