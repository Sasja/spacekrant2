# spacekrant2
Firmware for driving the 6 x 24 led matrix display of [whitespace](https://0x21.be) based on https://gitlab.com/jvanloov/arduino/.
More info on the [project page](https://0x21.be/index.php/spacekrant)

# how to build/upload/test
0. 'sudo apt-get install pip picocom arduino'
1. 'sudo pip install configobj glob2'
2. install ino (http://inotool.org)
3. run 'ino build -v --cflags="-std=c99"'
4. run 'ino upload' to upload
5. run 'ino serial' to connect over picocom (hint: CTRL-A CTRL-X to exit)

now change the routine by sending 'ESC + a', 'ESC + b',...

send each routine some data...

caveat: if you need to send a routine an 0x1B (ESC), then you have to escape ESC with ESC. So double ESC.

# how to talk to the spacekrant
'''
picocom /dev/ttyACM0
'''

or

'''
minicom -D /dev/ttyACM0 -b 9600
'''

# how to write your own spacekrant routine 

TODO

# how to run it on startup, quick and dirty

'''
@reboot sleep 30 && /home/whitespace/spacekrant2/tcp/startSpaceService.sh >/dev/null 2>&1 &
'''
