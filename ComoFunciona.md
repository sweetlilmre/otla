# Explanation of  the loading routine #

Routine has two parts.
First one detects pilot guide tone and the polarity of the wave that came from Cassette port. Depending on polarity the second part is adjusted (mutant code). Note that this routine in ROM will only work for one polarity, not the reverse one. In RAM works perfectly

Second one receives data while there are cycles in the wave that are valid to be decoded.
Note that there is not any check of total amount of received bytes, so an overflow is a possibility

![http://personal.auna.com/casariche/otla/pilot.png](http://personal.auna.com/casariche/otla/pilot.png)



![http://personal.auna.com/casariche/otla/data.png](http://personal.auna.com/casariche/otla/data.png)