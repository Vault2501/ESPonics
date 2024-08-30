#!/bin/sh

echo 'const char index_html[] PROGMEM = R"rawliteral(' > index.h
cat index.html >> index.h
echo ')rawliteral";' >> index.h
