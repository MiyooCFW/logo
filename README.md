# logo
Main repository at https://github.com/TriForceX/MiyooCFW

**Instructions:**

After compiling the Miyoo CFW toolchain (https://github.com/TriForceX/MiyooCFW/wiki/Making-Games) on your system, edit the appropriate "Makefile" in your copy of this repository to point it to where you've installed the toolchain (and rename the file to just "Makefile"), and simply run the "make" command in the terminal (while inside the source directory) to build the new "boot-logo" binary.

You'll need to convert your logo image into a "0x" formatted hexadecimal data string and add it to "assets.h". (The same goes for the sound file.) You'll also need to count the total number of "initialisers", the comma separated values, in your hexadecimal string, and write their total number into the square brackets for the header identifier that contains your file.

**Tips:**

- Make sure that your hex string in broken into newlines for ease of reading, and to avoid copy-paste complications with certain software.

**Online tools/resources:**

Hexadecimal to binary converter: http://tomeko.net/online_tools/hex_to_file.php?lang=en

Binary to hexadecimal converter: http://tomeko.net/online_tools/file_to_hex.php?lang=en

Comma counter: https://tools.knowledgewalls.com/noofcommascounter or https://www.8nog.com/counter/?hl=en

(Don't forget to add 1 to the total sum the counter gives you, for the last value in the string.)
