# The player otla #

> The otlaPlayer coverts  Sbb files (SbbFormat), which contains the information of the original program, to an audio signal (wav format) that the old computer will be able to interpret.

> Typically, the audio signal is structured as follows.

> The first part is a program that is loaded according to the standard loading routine of each machine.
> The program is nothing more than a loader with special loading routine. In addition to the routine load itself can be modified some parameters (CLEAR, SP, POKE-1, ..)

> Once the routine is loaded into memory it stars loading one or more blocks of data. It is a loading at high speed according to this specification: DefinicionAudio



## Loader Options ##

**Makeloader**
> The wave normally begins with the audio signal for the standard loader. If this option is disabled, it will only be saved in wav signal corresponding to the blocks of high speed. (This can be useful in case that the high speed loader is already in memory)

**Accelerate loader**.
> The standard loading rutine of each machine  specifies a duration of the tone guide. That length can be shortened to a certain limit and loading standards will work smoothly. (It can be saved some extra seconds of loading)

**Check loading error**.
> The loading of blocks at high-speed includes a cheksum control for each block. If there is a loading  error , loading will stop. This control can be overriden with the hope that one small loading error does not obstruct the program to work.


**Cassette motor relay .**
> The machines MSX and CPC have a relay to activate or stop the old cassette player motor. With digital devices this is not necessary but emulators require activating this option




## WAV options ##


**Sampling Rate**
> Wav files can be generated with different sampling frequencies (44100 or 48000 Hz). We must choose the same frequency that digital audio player uses. For example, a CD player uses 44100. The old sound cards use 44100. Some modern use 48000.

**Sampling / bit (speed) .**
> The lodind routine load can operate at different speeds. See audio definitión for datails (DefincionAudio)

**Waveform**
> The same can be said of the waveform (WavForms). When trying highest speed waveform is key to achieving a correct loading. Don't use square wavs to conver to MP3 (use better cubic or sqrsin)

**Reverse polarity**
> The hi-speed loading should be work for any polarity. However loading at the highest speeds and depending on the waveform , one polarity can work  better than its opposite.

**ms pause Between blocks**
> Between the high-speed  oaded blocks there can be silences. Not properly , in fact, high frequency tone guide of greater or lesser time length. In the case of loading an execuable block  (but not last block to be loaded, such as a block to be autodescomprime) time length of the tone must be greater than the decompression time needed to return to the loeding of the next block


_Typical adjustment_: cubic waveform, 44100 hz sampling  frequency and 3.5 samples / bit (12600 bps)


---

_modulo para crear la señal de audio_

# El reproductor otla #

> El módulo otlaPlayer covierte un fichero .sbb (SbbFormat) , que contiene la información del programa original, a una señal de audio ( formato wav)  que la vieja computadora será capaz de interpretar.

> Típicamente la señal de audio tiene la siguiente estructura.

> Una primera parte es un programa que se carga según la norma de la rutina de carga standard de cada maquina.
> El programa no es otra cosa que un cargador con rutina de carga especial. Además de la  propia rutina de carga se pueden modificar algunos parametros (CLEAR , SP, POKE-1 ,..)

> Una vez la rutina de carga está en memoria comienza la carga de uno o varios bloques de datos. Es una carga a alta velocidad segun  esta especificación: DefinicionAudio



## Opciones del cargador ##

**Generar cargador**
> El wav empieza con la señal de audio correspondiente al cargador. Si no se activa solo se guardarán en el wav la señal correspondiente a los bloques de alta velocidad. (esto puede resultar útil en el caso de que el cargador esté ya en memoria)

**Acelerar cargador**.
> La norma de carga standard de cada maquina especifica una duración del tono guia. Esa duración se puede acortar hasta cierto límite y la carga standar seguirá funcinando sin problemas. (se pueden ahorrar nos cuantos segundos de carga)

**Parar ante error de carga**.
> La carga de los bloques de alta velocidad incluye un control de checksum para cada bloque. En caso de error la carga se para (loading error). Este parón se puede desactivar con la esperanza de que algún pequeño error de carga no impida el programa que se pretende cargar pueda llegar a funcionar.

**Activar relé motor.**
> Las maquinas msx y cpc disponen de un relé para activar o parar el relé del motor del reproductor de cassete. Con los reproducotres digitales esto no es necesario pero los emuladores requieren activar esta opción



## Opciones del wav ##

**Frecuencia de muestreo**
> Los ficheros wavs se pueden generar con distintas frecuencias de muestreo (44100 o 48000). Se debe elegir la frecuencia que utliza el reproductor de audio digital. Por ejemplo, un reproductor de CD utiliza 44100. Las tarjetas de sonido antiguas 44100. algunas modernas 48000.

**Muestras/bit (velocidad).**
> La rutina de cargas puede funcionar a distintas velocidades.

**Forma de onda**
> Lo mismo se puede decir de la forma de onda (WavForms). Cuando se intentan velocidad más altas la forma de onda es clave para conseguir una carga correcta

**Invertir polaridad**
> La carga debería ser correcta para cualquier polaridad. Sin embargo para cargas a las velocidades más altas y dependiendo de la forma de onda una polaridad puede funcionar mejor que su inversa.

**Pausa entre bloques**
> Entre la carga de bloques de alta velocidad no hay propiamente pausas sino tono guia de alta frecuencia de mayor o menor duración. En el caso de carga de un bloque con dirección de ejecución (pero que no es ultimo bloque que se debe cargar, por ejemplo un bloque que se autodescomprime) el tiempo del tono debe ser mayor que el tiempo que llevará la descompresión para que en el retorno a la carga del siguiente bloque el tono guia siga presente




_El ajuste tipico_: forma de onda cubica, 44100 hz de frecuencia de muestreo y 3,5 samples /bit (12600 bps)