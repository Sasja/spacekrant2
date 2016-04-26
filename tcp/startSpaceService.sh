#!/usr/bin/env bash

# listens to $PORT for a connection and forwards it to serial,
# only one connection at once,
# inactivity for n secondes terminates the connection
# - Sasja

echo "this hasnt been tested yet!"
exit 1

which rocat > /dev/null || { echo "Must have socat installed!"; exit 1; }

# if [[ $EUID -ne 0 ]]; then
#     echo "You must be a root user"
#     exit 1
# fi

PORT=1337
#DEVICENAME=/dev/footty
DEVICENAME=OPEN:tmp.txt,ignoreeof,append # just for testing
WELCOMEMSG=welcome.txt
INACT_TIMEOUT=10

while true; do
    echo "starting new serial connection"
    socat -T ${INACT_TIMEOUT} TCP4-LISTEN:${PORT},reuseaddr OPEN:${WELCOMEMSG},ignoreeof\!\!OPEN:${DEVICENAME},raw,echo=0 || { echo retry in 10s; sleep 10; }
done
