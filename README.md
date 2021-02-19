# About
Custom code and notes for the project https://www.airgradient.com/diy/ (https://archive.is/aJiZO)

## HW Notes
- The PM2 meter has to be taped upside down compared to the picture in the guide (so the Plantower logo is on the top), or the exhaust ports won't line up with the holes in the 3D printed enclosure
- When the board is in the enclosure, apparently the board produces enough heat to raise the temperature inside the enclosure by about 3 degrees, compared to the "outside" air.

## SW notes
- The code provided by AirGradient ran fine, except for the "<Math.h>" library, which is apparently named "<math.h>" on my machine.
- Pin D7 is soldered to a cable that is connected either to GND or 3V, to determine if the module will report data to InfluxDB. 
