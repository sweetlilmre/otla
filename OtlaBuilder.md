# otlaBuilder #


The programs that once were stored on cassette tapes, discs 3'' or ROM cartridges can be found today in files that can be downloaded from the Internet.

To load these files into a real ZX81 , Spectrum , CPC, MSX with OtlaPlayer first it is necesary to convert them into a standard format (SbbFormat)

**Loading Name**
This is the string that appears on screen when loading starts. Its maximum length depends on machine type (10 for Spectrum, 16 for CPC, 6 for MSX). It can be used also with the load command (LOAD "name" instead of LAOD"")

**Enable interruptions between blocs .**
> Before starting the loading of a block disable interrupts. After recieving a block interrupts are enabled again.. The latter can be avoided. When loading snapshots interrupsts must be disabled.

**Relocate .**
> The hi-speed loading routine is located in different places for each machine.
> Default locations
  * ZXSpectrum $FF58 (GDU area)
  * CPC $FF10 (final part of the screen)
  * MSX $F430 (keyboard buffer)
  * ZX81 16444 (keyboard buffer)
> For CPC and MSX can be given alternative locations giving another value to more significant byte



---

_Módulo para construir ficheros sbb_

# otlaBuilder #


Los progrmas que en su día se almacenaban en cintas de cassette, discos de 3" o cartuchos ROMse puden encontrar hoy en día en ficheros que se pueden descargar de internet.

Para cargar estos ficheros en un ZX81 , Spectrum, CPC, MSX real con OtlaPlayer primero hay que convertirlos a un formato normalizado (SbbFormat)


**Nombre de carga**
Es la cadena que aparecerá en la pantalla cuando empiece la carga

**Habilitar las interrupciones entre bloques.**
> Antes de empezar la carga de un bloque se deshabilitan las interrupciones. Cuando acaba la carga del bloque se vuelven a habilitar. Esto ultimo se puede evitar.


**Reubicar.**
> La rutina de carga se ubica en lugares diferentes para cada maquina.
> ubicaciones por defecto
  * ZXSpectrum  $FF58  (zona de GDU)
  * CPC $FF10  (parte final de pantalla)
  * MSX $F430  (buffer del teclado)
  * ZX81  16444 (buffer de impresora)
> Para CPC y MSX se pueden dar ubicaciones alternativas dando otro valor al byte más significativo