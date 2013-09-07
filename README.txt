SimpleGSM

I bought myself a GSM Shield and wanted to use with an Arduino Leonardo and
a LCD shield.

The libraries I found where or incompatible with my GSM shield or with
Leonardo, so I write a simple program for use with it.

I slightly modified the GSM Shield for soft-power-on.

GSM Shield:
http://www.geeetech.com/gprsgsm-sim900-shield-board-arduino-compatible-p-610.html

What I did is to add a jumper wire from pin D2 to JP (the left one, close to the
antenna -- Note that I haven't short-circuit the two pads).

Now, I can soft-power-on the shield using pin D2 instead of D9 (as D9 is used
by LCD shield).

The "el'cheapo" LCD/Keypad Shield I bought from ebay is using the D4, D5,
D6, D7, D8, D9 pins for the LCD and A0 for the buttons (they are all
connected to the same analog pin via resistors -- nice idea!).

Arduino Leonardo has hardware support for TTL serial in pins D0, D1 (for
uploading sketches is using USB CDC communications). So you don't have to use 
a Software Serial library!



The SimpleGSM can display all the serial communications in LCD (it's
buffered for 50 lines/16 chars) and you can scroll with the UP/DOWN buttons.

SELECT button diaplays a menu to (voice) dial a number.
LEFT button answers a call.
RIGHT button hangups/terminates a call.
UP/DOWN buttons for scrolling the buffer.



Next things:

I will add a GPS module and some (temperature/humidity/weight) sensors, and will
try to upload the data occasionally using GPRS to my server for monitoring.



To be continued...
