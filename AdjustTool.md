# Adjust audio (volume and sampling frequency) #


The purpose of the adjust tool is to determine the adjustment of beter sampling frequency and volume suited to load faster.

For loads at high speed audio system must handle frequencies up to 11kHz and 24kHz for higher speeds.


These high frequencies are admitted smoothly for the 8-bit computers (Amstrad models seem to have more difficulties)

To make the adjustment, load the adjustment program normally (MSX to use LOAD "CAS", R (no bload)).

Next  playe a desired  test signal (11kHz, 12kHz, 22KHz or 24 kHz) with the volume to  minimum. Start with the signal of 11kHz

As you increase the volume it will be observed swaths of color on the BORDER. The swaths must be of a uniform thickness and with a continuous movement

Six number will appear on screen

The first number indicates the width of synchronism pulse

Must have a value between 17 and 23 for a direct polarity

Must have a value between 28 and 40 for a reverse polarity.

(The reference for forward and reverse in the real machine can be contrary to the parameter "inverse wav polarity." Even in emulators such as Spin and Spectaculator have contrary references for polarity )

The exact value depends on the machine and the sampling frequency.

The second number indicates the level of Vcc . Adjusting the volume can control this value. The ideal would be worth between -9 and 9. A value between -20 and 20 is acceptable.


The third and fifth numbers measure semicycles positive and negative. ideally should be similar values.

The fourth and sixth numbers measure the dispersion.

After setting the volume for optimal level of Vcc with the lower frequency test signals (11kHz and 12kHz) the test with the higher frequencies will help choose the appropriate sampling frequency.

If sampling is adequate, numbers third and fifth will be between 0 and 1 . And they will be rather like those observed in an emulator.
If the sampling frequency is not adequate, numbers will have no meaning.

(Not all emulators support the frequencies of 22KHz. Spectaculator Use, Klive .. for Spectrum, WINAP for CPC, OpenMSX for MSX, EightyOne for ZX81


---

_manual de usuario para el programa de ajuste_

# Ajustar audio (volumen and frecuencia de muestreo) #

El objetivo de la herramienta de ajuste es determinar la frequiencia de muestro y volumenes idóneos para una mejor carga a alta velocidad.

Para las cargas a alta velocidad el sistema de audio debe soportar frecuencias de 11kHz y hasta 24kHz para las velocidades más elevadas.

Estas elevadas frecuencias son adimitidas sin problemas por los computadores de 8 bit (los modelos Amstrad parecen tener más dificultades)

Para hacer el ajuste se carga el programa de ajuste normalmente (para MSX utilizar LOAD"CAS:",R (no bload)).

A continación se reproduce una señal de test deseada (11kHz, 12kHz, 22kHz, 24 kHz) con el volumen al mínimo. Empiécese con la señal de 11kHz

A medida que se aumenta el volumen se observarán franjas de color en el BORDER.  Las franjas será de un grosor uniforme y con un movimiento continuo. Con un poco de experiencia se puede determinar viendo las franjas si la frecuencia de muestro es la correcta

Aparecerán seis numeros en pantala

el primer número indica el ancho  del pulo de sincronismo

Deberá tener un valos entre 17 y 23 para una polaridad directa

Deberá tener un valos entre 28 y 40 para una polaridad inversa.

(La referencia  para "directa" e "inversa" en la maquina real puede ser al contraria del parametro "inver wav polarity". Incluso en emuladores como Spin y Spectaculator tienen referencias de polaridad contrarias)

El valor exacto depende la máquina y la frecuencia de muestreo.

El segudo número indica el nivel de continua. Ajustando el volumen se puede controlar este valor. El valor ideal sería entre -9 y +9. Un valor entre -20 y +20 es aceptable.


Los tercer y quinto números miden los semiciclos positivos y negativos y varían seguún la frecuencia de test. idealmente deberían ser valores parecidos.

Los cuarto y sextos números miden la dispersión.

Una vez ajustado el volmen para un óptimo nivel de continua con las señales de test de frecuencia más bajas (11kHz y 12kHz) las pruebas con las frecuencias más altas ayudarán a elegir la frecuencia de muestreo adecuada.

Si frecuencia de muestreo es adecuada los números tercero y quinto estarán comprendidos entre 0 y 1 se parecerán bastante a los que se observan en un emulador.
Si la frecuencia de muestreo no es la adecuada los números no tendrán sentido.

(No todos los emuladores soportan las frecuencias de 22kHz. Utilizar Spectaculator, Klive .. para Spectrum, WinApe para CPC, OpenMSX para MSX, EightyOne para zx81