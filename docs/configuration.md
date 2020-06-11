Configuration
=============

The configuration is controlled by the files in the config directory.
During the build these text files get converted to binary format by the python
scripts in the tools directory, and then they get merged together into one big
binary blob which is then written to the FPGA's flash chip.

There can be up to 64 different configurations and the HDMI Light can switch
between them automatically based on the incoming video resolution and aspect ratio.

Each of the configurations is controlled by the files in the `config/config_x`
directory, where _x_ is a number starting from 0. Each directory contains the
following config files:
* area.conf: Defines rectangular areas of the screen that should be averaged
             together to produce a colour.
* `output_x.conf`: One file for each LED output port. Defines which area and
                   colour correction data should be used to set the colour
                   of each LED connected to that port
* `colour_x.conf`: Defines each of the 8 colour correction matrices
* `gamma_r_x.conf`: Defines each of the 8 gamma curves for the red channel
* `gamma_g_x.conf`: Defines each of the 8 gamma curves for the green channel
* `gamma_b_x.conf`: Defines each of the 8 gamma curves for the blue channel
* delay.conf: Controls how much delay there is between the incoming video changing 
              and the LED colours changing


In addition to the files above, there are two global configuration files in the 
top level config directory:
* format.conf: Controls which configuration is loaded for a particular resolution
               and aspect ratio
* keys.conf: Defines the key mapping for CEC remote control actions.


The following sections describe each configuration file in detail. 


format.conf
-----------

The format configuration file contains one line for each known resolution and
aspect ratio. Each line has four columns:
* X Resolution
* Y Resolution
* Aspect ratio, which must be one of 1.33, 1.77, 2.44
* Index of the configuration to use for this resolution and aspect ratio

For example, this file defines three known formats for the three different aspect
ratios for 1920x1080
```
# width  height  ratio  config
  1920   1080    1.77   0
  1920   1080    2.40   1
  1920   1080    1.33   2
```

With this format config file the HDMI Light will use `config_0` for full screen
content, `config_1` for wide-screen content, and `config_2` for old narrow content.

When the incoming video format changes the HDMI Light will look for a matching
entry in the format config file and if it finds one then it will switch to the
specified configuration. If no matching entry is found then it will continue
to use the current configuration.



area.conf
---------

The area configuration file defines rectangular areas of the screen. The colour of
each pixel within the rectangles is averaged together to generate the colour to be
used for the LEDs.

Each line in the file contains five columns:
* xmin: The X coordinate of the left edge of the rectangle
* xmax: The X coordinate of the right edge of the rectangle
* ymin: The Y coordinate of the bottom of the rectangle
* ymax: The Y coordinate of the top of the rectangle
* divshift: How to divide the result after summing the pixels

The first four columns should be fairly self explanatory, they just define the 
coordinates of the rectangle. These coordinates use a reduced resolution compared
to the incoming video. The incoming video is scaled by 32, so for 1920x1080 the
coordinates in this are 0-59 for X, and 0-32 for Y.
The origin is the top left of the screen.

The last parameter is harder to explain. Especially if you're not familiar with how 
binary numbers operate. The averaging of the pixel colours in the rectangle is done
by summing all of the R, G and B values together and then dividing the result by the
number of pixels in the rectangle. However, a true division is a _very_ expensive
operation to perform in an FPGA, so instead we take advantage of the fact that each 
time we shift the digits of a binary number to the right, we divide the value by 2.
This last parameter defines how many places to shift the result to the right to 
perform the required division.

The following values are valid:
* 0: don't divide
* 1: divide by 2
* 2: divide by 4
* 3: divide by 8
* 4: divide by 16
* 5: divide by 32
* 6: divide by 64
* 7: divide by 128
* 8: divide by 256
* 9: divide by 512
* 10: divide by 1024
* 11: divide by 2048
* 12: divide by 4096
* 13: divide by 8192
* 14: divide by 16382
* 15: divide by 32768

As can be seen in this list, we can only divide by powers of two, so we are limited
to rectangles whose area is also a power of two, otherwise we can't get the correct 
result.


output.conf
-----------

There is one output configuration file for each of the eight LED strips that can be
connected. Each file contains one line for each LED that is connected to that port.
Each line contains five columns:
* index: The LED number. It's not actually used by anything, it's just there to make 
         life easier when manually editing the file
* area:  The index of the area to use for this LED
* colour: The index of the colour matrix that should be used for this LED
* gamma: The index of the gamma curve that should be used for this LED
* enable: Whether the LED is enabled (0=disabled, 1=enabled)

If colour correction is not being used then there are really only two important
columns: area and enable. For each LED, simply select which area from area.conf should
be used to select the colour for the LED. Then set enable to 1. 
The colour and gamma columns can be set to 0 (assuming the related config files have
been left on their default do-nothing values).

The first line in the file is the LED nearest the HDMI Light.

If colour correction is desired then the colour and gamma indices can be set to the 
relevant colour matrix and gamma curve that should be used for each LED.

One important thing to note is that the enable column cannot be used to disable individual
LEDs in the middle of a run, the column should be all 1's. If there are excess LEDs in the 
file then they can be set to 0, but there must be a continuous run of 1's followed by a 
continuous run of 0's. It is not valid to switch back and forth between 1 and 0.

If you want to switch off LEDs in the middle of a run, then a good way to do it is to
change one of the colour configurations to have brightness set to 0 for all channels.
Then simply select that colour matrix for the LEDs that you want disabled and they will
always be set to black and will not light up.


colour.conf
-----------

The colour configuration files define a colour matrix that can be used to colour correct
individual LEDs. A default do-nothing configuration file looks like this:

```
matrixR = 1.000 0.000 0.000
matrixG = 0.000 1.000 0.000
matrixB = 0.000 0.000 1.000
matrixC = 0.000 0.000 0.000

brightR = 1.0
brightG = 1.0
brightB = 1.0

contrast = 1.0

saturation = 1.0
```


The first section defines a raw matrix, in the example above it is the identity matrix
where the red output comes 100% from the red input, green 100% from green, and blue
100% from blue. The last line `matrixC` is for adding constant values to the channels
and in this case it is adding 0 to each channel.

The second section controls relative brightness of each channel. In the example above
it does nothing, each channel is 100%. This value can be increased as well as decreased.
For example setting a channel to 2.0 will multiply the brightness by 2, and setting a 
channel to 0.5 would cut the brightness in half.

Next is a contrast adjustment, and then a saturation adjustment.

The brightness, contrast, and saturation values are all converted to matrices. Then all
of these matrices are multiplied together and with the raw matrix at the top of the file
to produce a final colour correction matrix that is used to adjust the LED colours.



gamma.conf
----------

There can be 8 sets of gamma correction curves, and each set has a separate curves for 
each channel. The configuration file simply contains a single value which defines the
gamma curve. A default do-nothing file would contain the value 1.0 which defines a
straight line so that each input value results in the same value being output.



delay.conf
----------

The delay configuration file allows the output of the HDMI Light to be precisely
synchronised with the TV. By default the HDMI Light will update the LEDs immediately
when the incoming video frame ends, but modern TVs generally lag a few frames as they
do a lot of internal processing on the video.

The configuration file contains three values:
* frames: how many whole frames to delay
* ticks: how many extra microseconds to delay after the frame count
* smooth: temporal smoothing factor. 

The frames parameter should be self explanatory, it simply delays the output by the
specified number of frames. Valid values are from 0 to 8.

The ticks parameter should be ignored. There is no point in trying to set the delay
that accurately as it takes so long to clock the data out to the LED strips that the
accuracy is lost.

The final parameter is the temporal smoothing factor. It defines how much of the colour
for an LED comes from the new colour and how much comes from the previous colour.
Setting this to 0.0 will disable temporal smoothing and the colour of an LED will come
entirely from the new colour from the area on the screen.
Setting this to 0.5 will use 50% of the previous colour and 50% of the new colour.

My TV delays its output by 2 frames. I find that the following settings all give 
perfect synchronisation:
* frames = 2, smooth = 0.0
* frames = 1, smooth = 0.6
* frames = 0, smooth = 0.75

Personally I prefer the last option which makes it fade between sudden colour changes.

