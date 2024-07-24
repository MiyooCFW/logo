# logo

**Usage:**

`boot-logo <logo_start[tick]> <logo_ending[tick]> <logo_speed[ppf]>`

- logo_start = delay until the logo start in [1/60th of a second]  
- logo_ending = time from the moment the logo stops moving and sound is played until the logo app closes in [1/60th of a second]  
- logo_speed = speed at which the logo moves in [pixels per frame]

You can customize your boot-logo output by modyfing following files:
- `$HOME/logo.png` - scrolling logo image
- `$HOME/logo.wav` - scrolling logo sound
- `$HOME/logobg.png` - background image

For images use PNG, for sound WAV (stereo, 22050 Hz, signed 16-bit PCM)

**Building:**
- native linux build:
```
make clean
make
```
- cross-compile for miyoo:
```
export SYSROOT=/opt/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot
export CROSS_COMPILE=/opt/miyoo/usr/bin/arm-linux-
make clean
make
```
You'll need to convert your logo image into a "0x" formatted hexadecimal data string and add it to "assets.h". (The same goes for the sound file.) 
You have to also count the total number of "initialisers", the comma separated values, in your hexadecimal string, and write their total number 
into the square brackets for the header identifier that contains your file.

**Tips:**

- Make sure that your hex string in broken into newlines for ease of reading, and to avoid copy-paste complications with certain software.

**Online tools/resources:**

Hexadecimal to binary converter: http://tomeko.net/online_tools/hex_to_file.php?lang=en

Binary to hexadecimal converter: http://tomeko.net/online_tools/file_to_hex.php?lang=en

Comma counter: https://tools.knowledgewalls.com/noofcommascounter or https://www.8nog.com/counter/?hl=en

(Don't forget to add 1 to the total sum the counter gives you, for the last value in the string.)
