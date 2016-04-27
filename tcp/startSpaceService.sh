#!/usr/bin/env bash

# listens to $PORT for a connection and forwards it to serial,
# only one connection at once,
# inactivity for n secondes terminates the connection
# - Sasja

which socat > /dev/null || { echo "Must have socat installed!"; exit 1; }
which picocom > /dev/null || { echo "Must have picocom installed!"; exit 1; }
which screen > /dev/null || { echo "Must have screen installed!"; exit 1; }

# if [[ $EUID -ne 0 ]]; then
#     echo "You must be a root user"
#     exit 1
# fi

PORT=1337
DEVICENAME=/dev/ttyACM0
#DEVICENAME=OPEN:tmp.txt,ignoreeof,append #for testing
WELCOMEMSG=welcome.txt
INACT_TIMEOUT=10

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#stty -F ${DEVICENAME} sane -hupcl || { echo "could not disable hangup signal on device ${DEVICENAME}"; exit 1; }
# the above doesnt seem to prevent arduino reset on echelon, so therefore the hack below will do the trick by keeping the serial open.
screen -dmS spacekrant picocom --nolock ${DEVICENAME}

while true; do
    echo "listening for new tcp connection"
    socat -T ${INACT_TIMEOUT} TCP4-LISTEN:${PORT},reuseaddr OPEN:${DIR}/${WELCOMEMSG},ignoreeof\!\!OPEN:${DEVICENAME},raw,echo=0 || { echo retry in 10s; sleep 10; }
done
