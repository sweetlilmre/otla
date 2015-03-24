# La cuestión de la compresión #


¿Cual sería el ahorro de tiempo cargado un fichero comprimido?


## Teoría ##


> El tiempo de carga a gran velocidad es la suma de los siguientes tiempos.

> Tiempo de carga del cargador rápido a la velocidad de ROM . Es un programa BASIC con un linea REM con el código del cargador.Tiene un tamaño del orden de 200 bytes (el tamaño de cada método de carga  varía)y su tiempo de carga es aprox. 6 segundos según las temporizaciones defininas en la rutina de carga de la ROM. Con la opción de acortar esas temporizaciones (Accelerate BASIC loader) se acortan el tono guia y la duración de ceros y unos. Así el tiempo se reduce a aprox. 4 segundos

> Tiempo de carga de los datos a velocidad rápida . Si pensamos en Snapshot de 48k  tenemos  49152\*8 = 393216 bits para trasmitir a una velocidad de 19.600 resultaría: 20 segundos.En la realidad ese tiempo siempre suele ser menor ya que los snapshot no son secuencias puras aleatorias de ceros y unos. Predominan los ceros sobre los unos. Como trasmitir un cero lleva menos tiempo podemos decir que hay una compresión implícita (Ci). (Esto no se aplica métodos de carga que utiliazan el mismo tiempo para trasmitir un cero que para trasmitir un uno ( Manchester, Fi ))

> En el caso de trasmisión del mismo snapshot pero comprimido el tiempo carga del cargador será el mismo. El numero de bits para trasmitir será en este caso menor, según la relación de compresión conseguida. Pero en un fichero comprimido el numero de unos y ceros tiende a ser igual por lo que la compresión implícita en la trasmisión no se aprovecha. Además de los datos hay que trasmitir la rutina descompresora. Tiempo perfectamente despreciable ya que se va a cargar a gran velocidad o si se quiere se puede incluir corrigiendo la relación de compresión final sumando al tamaño del fihero comrpimido el tamaño de la rutina.Y por último queda el tiempo de descompresión. Que se puede despreciar si el objetivo es el tamaño final del fichero wav. Pero se debe tener en cuenta para medir el tiempo total o si se se van a cargar varios bloques comprimidos (para snapshots de 128k) no se podrá empezar a cargar el segundo bloque hasta que no se haya descomprimido el primero.

## Haciendo números. ##

Ejemplo: snapshot del Manic Miner comprimido con un algoritmo RLE queda en 26627.
Lo que significa una relación de compresion 26627/49152=54% considerable

Tc = Tiempo del cargador
Ci = compresion implicita en la tramision de un fichteo sin comprimir
Rc = ratio de comresion
Td = tiempo de descompresion

Veamos los timempos de carga del snapshot del Manic Miner en los diferentes casos (comrpimido y sin comprimir) y a diferentes velocidades. Se supone un tiempo de descompresión de 1 segundo.

### 19600 bps  (2459 bytes/seguundo) ###

> carga normal: Tc + Ci\*49152/2459 = 4.2 + 15.8 = 20 segundos

> carga comprimido : Tc + Rc\*49152/2459 + Td = 4.2 + 10.3 +1 = 15.5 segundos

> el tiempo final queda en el : 15,5/20 = 77.5%


### 25200 bps  (3150 bytes/segundo) ###

> carga normal: Tc + Ci\*49152/3150 =  4.2 + 11.4 = 15.2 segundos

> carga comprimido : Tc + Rc\*49152/3150 + Td = 4.2 + 7.8 +1= 13 segundos

> el tiempo final queda en el :  13/15.2 = 85.5%

Conclusión primera: Todo lo que se gane comprimimiendo no se va a ganar proporcianalmente en el tiempo final de carga y la mejora en la compresión se notará menos cuanto mayor se la velocidad de carga

De la reducción de 46% del fichero comprimido se cosigue reducir el timepo un 22.5% a 19600 bps y  solo  14.5% a 25200 bps. No parece que mejorar sistema de compresión se vea recompensado con unas mejoras de tiempo significativas.

Claro que el sanpshot del Manic Miner puede adaptarse muy favorablemente a la compresion RLE y no ser muy representativo.