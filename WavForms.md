# Wave Forms #

> OtlaPlayer can generate different waveforms.

Cubic waveform cube is defualt. When you want to experiment with higher speeds it is possible that another form of wave can function better

**Square**

It is the natural way to represent the transmission of data by a audio-line as home computers did in the 80's. Many tools work only with this waveform.
But in a real transmission ther is not such ideal square wave.
And especially when you want to convert the sound to an MP3 format the square wave  gives  dismal results.

(Note: some emulators only work well with the square waveform (for instance WinApe for Amstrad CPC))

**Cubic**

It was generated using a 3 order polynomial : x(x + P)(x - P) where 1/(2P) is the frequency

**Square Root Sine**

Similar to the previous one but it offers more outstanding in the falling and rising edges. This waveform is used to generate the part of the normal load.

**Saw tooth**

Better falling edge and smooth rising edge

**Equal energy pulse**

The square pulses of high frequencies are transmitted with different amplitude dependig on their frequency. So electrical energy quantum is constant for each pulse.
Theorical intuition that actually works.

**Delta**

Purely experimental


---

_Distintas formas de onda para cargas a alta velocidad_

# Formas de onda #

> ProyectoOtla puede generar distintas formas de onda.

Lo normal será utilizar la forma de onda cúbica.

Cuando se quiere experimentar con velocidades más altas es posible que otra forma de onda funcione mejor

**Cuadrada**

Es la forma natural para representar la trasmisión de datos por una linea de audio tal y como lo hacían los computadores domésticos de los años 80. Muchas herramientas trabajan únicamente con esta forma de onda.
Pero a la hora de una trasmision real, no ideal, la onda que "oye" el computador nunca será cuadrada.
Y sobre todo cuando se quiere convertir el sonido a un formato MP3 la onda cuadrada da pésimos resultados.

(Nota: algunos emuladores solo trabajan bien con la forma de onda cuadrada)

**Cubica**

Se construye con un polinomio de orden 3:  x(x+P)(x-P) donde 1/(2\*P) es la frecquncia

**Raiz cuadrada del seno**

Parecida a la anterior ofrece mayor pendiente en los flancos de subida y bajada. Esta forma de onda es la que se utiliza para generar la parte de carga normal.

**Diente de sierra**

Mayor pendiente posible del flanco de bajada y constante en el flanco de subida.

**Pulsos de igual energia**

Las ciclos de altas frecuencias se transmiten con mayor amplitud de los de baja de tal forma qua energia de los pulsos es constante.
Intuición teórica que realmente funciona. (Sobre todo en otro tipo de trasmisiones como Manchester)

**Delta**

Puramente experimental