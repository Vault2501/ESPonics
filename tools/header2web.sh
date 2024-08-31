#!/bin/sh

grep -v "const char index_html" index.h | grep -v "rawliteral" > index.html
