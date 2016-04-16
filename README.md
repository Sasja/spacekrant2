# spacekrant2
Firmware for driving the 6 x 24 led matrix display of [whitespace](https://0x21.be) based on https://gitlab.com/jvanloov/arduino/.
More info on the [project page](https://0x21.be/index.php/spacekrant)

# how to build/upload/test
0. 'sudo apt-get install pip picocom'
1. 'sudo pip install configobj glob2'
2. install ino (http://inotool.org)
3. run 'ino build -v --cflags="-std=c99"'
4. run 'ino upload' to upload
5. run 'ino serial' to connect over picocom (hint: CTRL-A CTRL-X to exit)

now change the routine by sending 'ESC + a', 'ESC + b',...

send each routine some data...

caveat: if you need to send a routine an 0x1B (ESC), then you have to escape ESC with ESC. So double ESC.

# how to write your own spacekrant routine 

TODO
