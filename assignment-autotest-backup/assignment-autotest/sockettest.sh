#!/bin/bash

SERVER_HOST=localhost
SERVER_PORT=9000

MESSAGE="hello world"
EXPECTED="$MESSAGE"

RESPONSE=$(echo "$MESSAGE" | nc $SERVER_HOST $SERVER_PORT)

if [ "$RESPONSE" = "$EXPECTED" ]; then
    echo "PASS: Server echoed message correctly"
    exit 0
else
    echo "FAIL: Expected '$EXPECTED' but got '$RESPONSE'"
    exit 1
fi
