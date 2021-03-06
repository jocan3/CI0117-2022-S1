# CI0117 - Actividad de clase 1

Para esta actividad, suba las soluciones a su repositorio personal de Github, siguiendo la siguiente estructura de folders:

```
/acividades/[número de actividad]/[nombre de ejercicio]/
```

Utilice como base el [ejemplo de hola mundo](https://github.com/jocan3/CI0117-2022-S1/blob/main/ejemplos/01_pthreads/main.c) básico visto durante la clase.

## Ejercicio 1 [hello_goodbye]

- En un archivo hello_goodbye.c cree un programa que realice lo siguiente:
  - El hilo principal ejecuta el siguiente comando y guarda el valor obtenido en una variable (incluya la bibilioteca *<unistd.h>*):
  ```
   size_t value = sysconf(_SC_NPROCESSORS_ONLN);
  ```
  - El hilo principal imprime el mensaje *Hello and goodbye world!* seguido del valor obtenido con el comando sysconf. 
  - Un hilo secundario imprime el mensaje *Hello world!* seguido del valor obtenido por el hilo principal con el comando sysconf.
  - Un hilo secundario imprime el mensaje *Goodbye world!* seguido del valor obtenido por el hilo principal con el comando sysconf.
- Asegúrese de compilar y ejecutar el código en su máquina de forma individual.
- Discuta junto a su grupo de trabajo lo siguiente:
  - ¿Cuántos hilos diferentes se ejecutan en el programa?
  - ¿Existe concurrencia en su programa? Si es así, ¿cuáles líneas de código se ejecutan de manera concurrente?
  - ¿El valor obtenido con el comando sysconf es igual para todos?¿Qué representa dicho valor?
- Anote las respuestas a las preguntas anteriores, las cuales serán discutidas luego de la activdad.

## Ejercicio 2 [hello_multiple]

Junto a su grupo de trabajo realice lo siguiente y discuta las preguntas planteadas. NO es necesario realizar ningún cambio en el código.

- Comprenda el código del archivo hello_multiple.c. Sin cambiarlo, compile y ejecute el programa en su máquina de forma individual.
  ```
  cc -g hello_multiple.c -Wall -Wextra -o hello_multiple -pthread
  .\hello_multiple
  ```
 ¿Se obtiene el mismo resultado siempre que se ejecuta?¿Qué diferentes salidas se obtienen?

- Ejecute el programa utilizando *valgrind*.
  ```
  valgrind ./hello_multiple
  ```
  ¿Se detecta algún errror en el programa?

- Compile el archivo utilizando *clang* y los *sanitizers* de memoria creando diferentes ejecutables para cada comando. Luego ejecute los archivos.
  ```
	clang -g hello_multiple.c -fsanitize=memory -Wall -Wextra -o hello_multiple_msan -pthread
	clang -g hello_multiple.c -fsanitize=address -Wall -Wextra -o hello_multiple_asan -pthread
	clang -g hello_multiple.c -fsanitize=thread -Wall -Wextra -o hello_multiple_tsan -pthread
  ./hello_multiple_msan
  ./hello_multiple_asan
  ./hello_multiple_tsan
  ```
  ¿La ejecución de los archivos muestra algún error?
  
