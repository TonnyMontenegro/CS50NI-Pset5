# CS50Xni Pset 5: Mispellings 

Implementar:

- <a href="#Speller" class="btn btn-sm btn-default">Speller</a>
    - 1.  Implementa  `load`.
    - 2.  Implementa  `check`.
    - 3.  Implementa  `size`.
    - 4.  Implementa  `unload`.

<h2 id="Speller">Speller</h2>

Implementa un programa que verifique la ortografía de un archivo, como se muestra acá abajo.

```
$ ./speller texts/lalaland.txt
MISSPELLED WORDS
[...]
AHHHHHHHHHHHHHHHHHHHHHHHHHHHT
[...]
Shangri
[...]
fianc
[...]
Sebastian's
[...]
WORDS MISSPELLED:
WORDS IN DICTIONARY:
WORDS IN TEXT:
TIME IN load:
TIME IN check:
TIME IN size:
TIME IN unload:
TIME IN TOTAL:
```


### Understanding

Teóricamente, teniendo una entrada de tamaño  _n_, un algoritmo con un tiempo de ejecución  _n_  es asintóticamente equivalente a un algoritmo con un tiempo de ejecución de 2_n_. Sin embargo, en el mundo real, es un hecho de que este último se siente dos veces más lento que el primero.

El reto al que tienes que enfrentarte es, ¡implementar el corrector de ortografía más rápido que puedas! Aunque, al decir el “más rápido” estamos hablando del tiempo apreciable real en el mundo real - en esta ocasión, nada de esas definiciones asintóticas.

En  `speller.c`, hemos puesto un programa que está diseñado para verificar la ortografía de un archivo luego de cargar del disco duro a la memoria un diccionario de palabras. Desafortunadamente, no nos dimos a la tarea de implementar la parte en que se carga el diccionario, ni la parte en que verifica la ortografía. ¡Ambas (y un poquito más) las hemos dejado para ti! Pero primero, hagamos un tour.

#### `dictionary.{c,h}`

Abre  `dictionary.h`. En dicho archivo están declaradas cuatro funciones; toma nota de que debería hacer cada una. Ahora abre  `dictionary.c`, nota que hemos implementado esas cuatro funciones pero solo lo suficiente para que el código compile. Al final, tu trabajo es re-implementar esas funciones tan ingeniosamente como sea posible de tal forma que el corrector de ortografía funciones a como se espera, ¡y rápido!.

#### `Makefile`

Recuerda que  `make`  automatiza la compilación de tu código de tal forma que no tengas que ejecutar manualmente  `clang`  junto con otro montón de comandos. Sin embargo, a medida que tus programas comiencen a crecer, make no será capaz de inferir el contexto ni de cómo compilar tu programa; necesitarás comenzar a decirle cómo compilar tu programa, particularmente cuando se involucran archivos múltiples de código fuente (ejemplo,  `.c`), como es el caso de este problema. Es por eso que utilizaremos  `Makefile`, un archivo de configuración que le dice a make exactamente qué hacer. Abre  `Makefile`, y echemos un vistazo a sus líneas.

La línea de acá abajo define una variable que se llama  `CC`, la cual especifica que debería hacer  `clang`  para compilar.

```
CC = clang
```

La siguiente línea define una variables llamada  `CFLAGS`, la cual especifica que  `clang`  debería usar algunas banderas, la mayoría de las cuales deberían parecer conocidas.

```
CFLAGS = -fsanitize=integer -fsanitize=undefined -ggdb3 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wshadow
```

La línea de abajo define una variable que se llama  `EXE`, el valor que tome será el nombre de nuestro programa

```
EXE = speller
```

La siguiente línea define una variable que se llama  `HDRS`, cuyo valor será una lista de archivos de cabecera separados por espacio que son usados por  `speller`.

```
HDRS = dictionary.h
```

La línea abajo define una variable que se llama  `LIBS`, cuyo valor debería ser un lista de librerías separadas por espacios, donde cada una debería tener un prefijo de  `-l`. (Recuerde el uso de  `-lcs50`). Por suerte no tendrás que enumerar ninguna librería para este problema, pero hemos incluido la variables por si acaso.

```
LIBS =
```

La siguiente línea define una variables llamada  `SRCS`, cuyo valor debería ser una lista de archivos C separados por espacio que colectivamente implementarán  `speller`.

```
SRCS = speller.c dictionary.c
```

La siguiente línea define una variable llamada  `OBJS`, cuyo valor es idéntico a  `SRCS`, expecto que la extensión de cada archivo es  `.o`  y no  `.c`.

```
OBJS = $(SRCS:.c=.o)
```

La siguiente línea define un “objetivo” al usar estar variables y decile a make cómo compilar  `speller`.

```
$(EXE): $(OBJS) Makefile
$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
```

La siguiente línea especifica que nuestros archivos  `.o`  dependen de  `dictionary.h`  y  `Makefile`  de tal forma que cambios hechos en cualquiera de esos dos archivos inducen a volver a compilar el archivo cuando se ejecuta  `make`.

```
$(OBJS): $(HDRS) Makefile
```

Finalmente, la línea abajo define otro objetivo para limpiar el directorio de este problema.

```
clean:
rm -f core $(EXE) *.o
```

Debes saber que eres bienvenido a modificar el archivo  `Makefile`  a como mejor te parezca. De hecho deberías cambiarlo en caso de que crees por tu cuenta cualquier archivo  `.c`  o  `.h`. Pero solo asegúrate de no cambiar ninguna tabulación (por ejemplo  `\t`) a espacios, ya que  `make`  espera que ellas estén presenten debajo de cada objetivo.

El efecto neto de todas estas líneas es que tú puedes compilar  `speller`  con un único comando, aun cuando consta de pocos archivos:

```
make speller
```

Mejor aún, puedes solo ejecutar:

```
make
```

Y si en algún momento quisieras eliminar speller mas cualquier archivo  `core`  o  `.o`, puedes hacerlo con un simple comando:

```
make clean
```

En general, en cualquier momento que quieras compilar tu código para este problemas, debería bastar con ejecutar:

```
make
```

#### `speller.c`

Ok, ahora abre  `speller.c`  y dedícale algo de tiempo para revisar el código y los comentarios que ahí están. No necesitarás cambiar nada en este archivo pero de todas formas deberías entenderlo. Nota cómo a través de  `getrusage`, vamos a “parametrizar” (por ejemplo, el tiempo de ejecución) tus implementaciones de  `check`,  `load`,  `size`, y  `unload`. También nota cómo hacemos para pasar por  `check`  palabra por palabra el contenido de algún archivo a ser verificado. Finalmente, nosotros reportamos cada palabra mal escrita en ese archivo en conjunto con un montón más de datos estadísticos.

A propósito, note cómo hemos definido que será usado  `speller`

```
Usage: speller [dictionary] text
```

Donde asumimos que  `dictionary`  es un archivo que tiene una lista de palabras escritas en minúsculas, una por línea; y  `text`  es el archivo que se usará para hacer la revisión ortográfica. Como los corchetes sugieren, proveer el nombre del  `dictionary`  es opcional; si este argumento es omitido,  `speller`  usará por defecto  `dictionaries/large`. En otras palabras, ejecutar:

```
./speller text
```

Será equivalente a ejecutar

```
./speller dictionaries/large text
```

Donde  `text`  es el archivo que deseas revisarle la ortografía. Basta con decir que lo primero es más fácil de escribir. (Claro está que, ¡`speller`  no será capaz de cargar ningún diccionario hasta que implementes  `load`  en  `dictionary.c`! Hasta entonces, verás  **Could not load**  - No se pudo cargar.)

Por si te interesa saber, dentro del diccionario por defecto hay 143,091 palabras, y ¡todas deben cargarse a la memoria! De hecho, echa un vistazo a ese archivo para que te des una idea de su estructura y tamaño. Nota que cada palabra en el archivo aparece en minúsculas (incluso los nombres propios y acrónimos, con el fin de mantenerlo simple). Desde el inicio hasta el final, el archivo está ordenado lexicográficamente, con una sola letra por línea (cada línea termina con  `\n`). Ninguna palabra es mayor de 45 caracteres, y tampoco ninguna palabra aparece dos veces. Durante el desarrollo, podrás encontrar útil proveerle a  `speller`  un  `dictionary`  propio que contenga muchas palabras menos, no sea que tengas que lidiar en hacer debug a una estructura enorme en la memoria. En  `dictionary/small`  está un diccionario pequeño. Para usarlo, ejecuta

```
./speller dictionaries/small text
```

Donde  `text`  es el archivo que deseas revisar. No pases a la siguiente tarea del pset hasta que estés seguro de que entiendes cómo funciona  `speller`.

Las probabilidades son que no hayas dedicado mucho tiempo analizando  `speller.c`, así que regresa una cuadra hacia atrás y vuelve a caminar a través de esa calle una vez más.

#### `texts/`

Para que pudieras probar tu implementación de  `speller`, también te hemos proveído con un montón de textos, entre ellos el guión de  _La La Land_, el texto de la “Ley de Protección al Paciente y Cuidado de Salud Asequible” (nota: ley de EE.UU conocida como ACA), tres millones de bytes de Tolstoy, algunos extractos de  _“The Federalist Papers”_  y de Shakepeare, el texto completo de la Biblia King James V y del Corán, y muchos más. Para que sepas qué esperar de estos archivos, ábrelos y haz una lectura rápida de cada uno de los archivos. Todos ellos están el directorio que lleva por nombre  `texts`  dentro del directorio del  `pset5`.

Como ya debes de saber luego de haber leído cuidadosamente  `speller`.c, la salida de  `speller`, si se ejecuta lo siguiente:

```
./speller texts/lalaland.txt
```

Se parecerá eventualmente a lo siguiente abajo descrito. Por ahora, intenta ejecutar la solución del staff (usando el directorio por defecto)

```
~cs50/pset5/speller texts/lalaland.txt
```

Abajo están algunas de las salidas que podrás ver. Por el bien de la información, hemos seleccionado algunos ejemplos de “mispellings”. Y para no sea que le arruinemos la diversión, hemos omitido nuestras propias estadísticas por el momento.

```
MISSPELLED WORDS
[...]
AHHHHHHHHHHHHHHHHHHHHHHHHHHHT
[...]
Shangri
[...]
fianc
[...]
Sebastian's
[...]
WORDS MISSPELLED:
WORDS IN DICTIONARY:
WORDS IN TEXT:
TIME IN load:
TIME IN check:
TIME IN size:
TIME IN unload:
TIME IN TOTAL:
```

`TIME IN Load`  representa el número de segundos que  `speller`  empleó en ejecutar tu implementación de  `load`.  `TIME IN check`  representa el número de segundos que  `speller`  utilizó, en total, en ejecutar tu implementación de  `check`.  `TIME IN size`  representa el número de segundos que  `speller`  ocupó en ejecutar tu implementación de  `size`.`TIME IN unload` representa el número de segundos que  `speller`  ocupó en ejecutar tu implementación de  `unload`.  `TIME IN TOTAL`  es la suma de esas cuatro medidas.

**Note que estos tiempos pueden variar un poco en cada ejecución de  `speller`, y va a depender de qué esté haciendo el IDE de CS50 en ese momento, aun si no has hecho ningún cambio a tu código.**

Casualmente, para ser claros, cuando decimos que una palabra está mal escrita (“mispelled”) simplemente estamos diciendo que no está en el  `dictionary`  proveído.

### Especificaciones

Bueno, ahora el desafío ante en frente tuyo es de que implementes  `load`,  `check`,  `size`  y  `unload`  tan eficientemente como sea posible, de tal forma que el  `TIME IN load`,  `TIME IN check`,  `TIME IN size`, y  `TIME IN unload`  sean todos minimizados. Para estar seguros, no es tan obvio lo que significa “minimizado”, en tanto que estas métricas a ciencia cierta variarán dependiendo de las diferentes entradas de  `dictionary`  y  `text`  que le des a  `speller`. Pero es ahí donde yace el reto, si no es que la diversión, de este problema. Este problema es tu oportunidad para diseñar. Aunque te invitamos a minimizar el espacio, tu último adversario será el tiempo. Pero antes de que te sumerjas en el problema, te daremos algunas especificaciones.

-   No deberías modificar  `speller.c`.
    
-   Podrías modificar  `dictionary.c`  (y de hecho, debes modificarlo para poder completar las implementaciones de  `load`,  `check`,  `size`, y  `unload`), pero no debes modificar las declaraciones de  `load`,  `check`,  `size`,  `unload`.
    
-   Podrías modificar  `dictionary.h`, pero no deberías modificar las declaraciones de  `load`,  `check`,  `size`  y  `unload`.
    
-   Podrías modificar  `Makefile`.
    
-   Podrías agregar funciones a  `dictionary.c`  o a los archivos de tu propia creación siempre y cuando todo tu código se compile vía  `make`.
    
-   Tu implementación de  `check`  no debe ser sensible a las mayúsculas ni a las minúsculas. En otras palabras, si  `foo`  está en el diccionario, entonces  `check`  debería retornar verdadero sin importar si está escrita en mayúsculas o minúsculas; ningunas de las palabras  `foo`,  `foO`,  `fOo`,  `fOO`,  `Foo`,  FoO, ni  `FOO`  deberían ser consideradas como mal escritas.
    
-   Puesto de un lado el tema de las mayúsculas y minúsculas, tu implementación de  check  solo debería retornar  true  para las palabras que realmente están en  dictionary. Asegúrate de ser estricto con las palabras comunes, tales como “the”, no sea que le pasemos a tu implementación un  dictionary  sin esas mismas palabras. Mas aún, las únicas palabras permitidas son aquellas que están realmente en  dictionary. En otras palabras, sí  foo  está en  dictionary, pero aparece la palabra  foo’s  en el texto,  check  debería retornar  false  ya que  foo’s  no está en  dictionary.
    
-   Tu deberías asumir que a  `check`  solo le seran pasados strings con caracteres alfabeticos y/o apostrofes
    
-   Deberías asumir que cualquier  `dictionary`  que se le pase al programa estará estructurado exactamente como el nuestro, ordenado lexicográficamente desde inicio a fin con una sola palabra por línea, y cada línea termina con un  `\n`. También deberías asumir que  `dictionary`  contendrá al menos una palabra, ninguna palabra será más larga que  `LENGTH`  (longitud) (una constante definida en  `dictionary.h`), ninguna palabra aparecerá más de una vez, y que cada palabra estará escrita con caracteres alfabéticos en minúsculas y posiblemente apóstrofes.
    
-   Tu verificador de ortografía solo debería aceptar como entradas:  `text`  y de forma opcional  `dictionary`. Aunque te podrías inclinar (particularmente si te encuentras entre esos más cómodos) a procesar de antemano nuestro diccionario por defecto a fin de derivar una función hash ideal para tí. No deberías guardar en el disco duro la salida de ninguno de esos procesamientos previos a fin de cargarlos nuevamente en la memoria en ejecuciones subsecuentes de tu verificador de ortografía para ganar una ventaja.
    
-   Tu verificador de ortografía no debería tener ninguna fuga de memoria.
    
-   Puedes buscar (buenas) funciones hash en línea, siempre y cuando cites el origen de cualquier función hash que integres en tu propio código.
