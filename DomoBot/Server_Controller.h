#include <DNSServer.h>

#include <ESPAsyncWebServer.h>
#include "webpage.h"

AsyncWebServer server(80);
DNSServer dns;

// Create an Event Source on /events
AsyncEventSource events( SERVER_EVENTS_ENDPOINT );
AsyncEventSource botmap_events( MAP_EVENTS_ENDPOINT );
