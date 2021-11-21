# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

es la palabra mas larga de nuestro diccionario y de todo el idioma ingles

## According to its man page, what does `getrusage` do?

devuelve la cantidad de tareas y ejecuciones tanto primarias com secundarias

## Per that same man page, how many members are in a variable of type `struct rusage`?

2

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

para ahorrar recursos ya que pasar por valor seria crear una copia y trabajar con esa copia mientras que por referencia editamos el original

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

primero chequea que los argumentos sean validos, declara las
estructuras y variables que mediran nuestro tiempo de ejecucion
carga el diccionrio y de no poder cargarlo sale del programa e
imprime un error, trata de abrir el texto de poderse abrir
procede a verificar cada palabra dentro del texto e ignora las
palabras con numeros, si hay un error leyendo las palabras cierra
el archivo y manda un aviso, si termina de escanear todas las
palabras cierra el archivo y procede a calcular nuestros tiempos
de ejeucion y mostrarlos.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

porque fscanf lee palabras muy largas y no tiene un fin definido
mientras que fgetc termina de escanear cuando encuentra un \0


## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

son valores que no variaran en nuestro programa y usaremos dentro del mismo
