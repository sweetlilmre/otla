# FAQ (F.A.Q) #

FAQ


**Loadings work in an emulator but not in a real computer**

First be sure that it works at least the part of the standard routine. The hi-speed loader should be loaded (LOAD "" RUN ", or BLOAD" CAS ", R) as any normal program. If it is not, revising the cable, etc..

For the  high speed loading must be finely adjust the volume (to calibrate the best thing is to use testin screens (.hst)). Compare what you see on an emulator with what you see on the actual machine.

VERY IMPORTANT deactive in digital players any strange equalization, 3D effect, etc. ...

**Loadings work in a real computer but not in an emulator**

Some emulators only work well with the square waveform.
Others may work with a moderate speed (12600) but fails to higher speeds.

**When convert files. Sna or z80 loadings work smoothly, but some files  .cas ,.tap and .tzxdo not work even when I try on emulators**

It is not always possible to convert files .cas, .tap and .tzx automatically. The first condition for an easy conversion is that the byte blocks come with a header with information of the address home where it must be loaded. The second is that they must have a execution address once loaded the bytes (the famous USR in Zx Spectrum).
In that event that OtlaBuilder don't find these parameters can be assigned manually but  Figuring  these values out is not trival.
> An emulator can be helpful. With some practice and skill could take up multiload games at high speed.

**Do 44100 or 48000?**

The choice of the sampling frequency must not be somewhat capricious. It is clear that allows speeds exceeding 48000. But the sampling frequency should be the frequency of muesreo that uses the digital audio device. In a CD player will be 44100 Hz in an MP3 could be 48000. When in doubt: 44100 Hz


**how do I know the sampling frequency of my sound card?**

It is best to make loading tests with screens choosing  3.00 samples / bit and square waveform. If loading is correct to 48000 and not 44100 the to the sound card will probably of 48000. If the load is wrong with 48000 and 44100 goes well with the card will certainly 44100.


**What waveform using?**

Wave cubic waveform is best for load to 3.5 samples / s bit mp3 sound cards and for those unaware of its frequency of sampling. Another is that they want things done experiments with higher speeds. Equal energy wavefor can offer good results too.


**Loads with the sound card but go well with the MP3 player fail**

The biggest drawback of MP3 players fed with rechargeable batteries v 1.2 is that they  have little power output. It can be using a small amplifier.
The conversion to mp3 is done with herrameinta LAME.EXE and with the option fast

**What are the maximum speeds that can be achieved? "**

It depends on the machine (ZX, CPC, MSX) In my tests I have achieved
ZX to get 19200 bps
CPC to 16200 bps
MSX passes 21333 bps

The standard is 3.5 any case samples / bit to 44100Hz = 12600 bps.
You can still choose a slower rate (4 samples / bit) for the system to work even with an adapter magnetic audio cassette head. (Very bad condition for audio signal but it still works)


#summary preguntas frecuentes

# Preguntas Frecuentes (F.A.Q) #

FAQ

**¿Qué significan las siglas o.t.l.a?**

Viene de una canción de Willie Nelson _On The Road Again_. Pero cambiado un poco: On The Load Again. Otra vez con el LOAD"" ...

**En un emulador las cargas funcionan pero en un ordenador real no**

Primero hay que asegurarse que al menos funciona la parte de velocidad estandard. El programa cargador debería cargar (con LOAD"" , RUN" , o BLOAD"CAS:",R) como cualquier programa normal. Si no es así revisar el cable, etc.

Para la carga de alta velocidad hay que ajustar finamente el volumen (para calibrar lo mejor es usar las patallas de test). Compárese lo que se ve en un emulador con lo que se ve en la máquina real.

y MUY IMPORTATE desactivar de los reproductores cualquier tipo de ecualización extraña, efecto 3D , etc...

**En un ordenador real las cargas funcionan pero en un emulador no**

Algunos emuladores solo trabajan bien con la forma de onda cuadrada.
Otros pueden funcionar con una velocidad moderada (12,600) pero falla a velocidades más altas.

**Cuando convierto ficheros .sna o z80 las cargas funcionan sin problemas pero algunos ficheros .tap y .tzx no funcionan ni siquiera cuando lo pruebo en emuladores**

No siempre es posible convertir ficheros .tap y .tzx automáticamente. La primera condición para una conversión fácil es que los bloques de bytes vengan con una cabacera con la información de la direcion de inicio donde se cargarán los bytes. La segunda es que se debe tener una dirección de ejecución una vez cargados los bytes (el famoso USR).
En que caso de que otlabulder no encuetre estos parametros automaticamente siempre se pueden asignar manualmente pero averiguar esos valores no es trival.
> Un emulador puede ser de ayuda. Con algo de practica y habilidad hasta se podrían tener juegos multicarga a alta velocidad.


**¿44100 ó 48000?**

La elección de la frecuencia de muestreo no debe ser algo caprichoso. Es evidente que 48000 permite velocidades superiores. Pero la frecuencia de muestreo debe ser la frecuencia de muesreo que utiliza el dispositivo de sonido digital. En un reproductor de CD será 44100 Hz en un MP3 podría ser 48000. Ante la duda 44100 Hz


**¿cómo sé la frequencia de sonido de mi tarjeta de sonido?**

Lo mejor es hacer una pruebas de carga con las pantallas de test eligiendo 3.00 samples / bit y forma de onda cuadrada. La carga es correcta a 48000 y no a 44100 la tarjata será problamente de 48000 . Si la carga falla con 48000 y va bien con 44100 la tarjeta será seguramente de 44100.


**¿Que forma de onda utlizar?**

La  onda cúbica es mejor forma de onda para cagar a 3,5 samples/bit con mp3 y para tarjetas de sonido de las que desconocemos su frequencia de muestreo. Otra cosas es que se quieran hacer experimentos con velocidades más altas.


**Las cargas con la tarjeta de sonido van bien pero con el reproductor de MP3 fallan**

El mayor inconveniente de los reproductores MP3 alimentados con pilas recargables de 1,2v es que tiene poca potencia de salida. Se puede utlizar un pequeño amplificador.
La conversión a mp3 se hace con la herrameinta LAME.EXE y con la opción fast

**¿Cuales son la velocidades máximas que se pueden alcanzar?"**

Pues depende de la Maquina (ZX, CPC , MSX ) En mis pruebas he conseguido
para ZX se consiguen 19200 bps
para CPC 16200 bps
pasa MSX 21333 bps

El standard en cuaquier caso es 3,5 samples/ bit a 44100Hz = 12600 bps.
Se puede elegir todavía una velocidad más lenta (4 samples/bit) para que el sistema funcione incluso con un adaptador magnetico de audio a cabezal de cassette.