
“Polygonal” es un secuenciador por pasos MIDI desarrollado en openFrameworks, pensado para trabajar en conjunto con dispositivos que envíen o reciban mensajes MIDI y visualización reactiva al audio.

El objetivo de Polygonal es explorar la interacción Imagen → Sonido y Sonido → Imagen, así como la comunicación entre distintos dispositivos a través del protocolo MIDI.
Al mismo tiempo, se buscó desarrollar un secuenciador por pasos intuitivo y flexible, que permita visualizar múltiples patterns en simultáneo de un modo visualmente sugerente, favoreciendo la creación de polirritmias.

La idea está basada en el texto “The Geometry of Musical Rhythm” de Godfried Toussaint, y uno de los objetivos de este proyecto es poner en práctica los ejemplos allí desarrollados.

[Introducción (Drive)](https://drive.google.com/file/d/1ZOYT7SDBXCOxy4kJa1APnBfPdt4CTb6m/view?usp=sharing)

[![Introducción](https://img.youtube.com/vi/plEfalXWJqw.jpg)](https://youtu.be/plEfalXWJqw)


DESCRIPCIÓN GENERAL

El secuenciador (al que también llamaremos “círculo”) está programado para ser visualizado y operado desde la pantalla de openFrameworks (oF). Este recibe mensajes desde un controlador MIDI y desde el teclado de la computadora y también
 envía mensajes a Ableton Live, encargado de la generación del sonido, y a su ves,  openFrameworks recibe una señales de audio para lograr visuales reactivas.

Al visualizar la pantalla, se observa una configuración de nueve secuenciadores:
Tres de ellos están formados por un único círculo, y los otros dos dos están compuestos por otros secuenciadores concéntricos respectivamente. 
Uno de estos es reactivo al audio generado por él mismo.,

Además, se incluye un panel GUI desde el cual es posible ajustar en tiempo real el BPM general y el volumen de cada objeto, un pequeño cartel en el rincón inferior izquierdo con información extra sobre la asignación de teclas y sus funciones, y un letrero dinámico en el margen superior que solo aparece cuando posamos el mouse sobre un “paso”, para mostrar el nombre de una nota musical asignada.


PASOS 

Cada secuenciador posee n pasos, formados por círculos pequeños de color gris, ubicados sobre la circunferencia del círculo principal, distribuidos de manera equidistante dividiendo el total de la circunferencia  en n partes iguales. 

Existe además un círculo adicional —al que llamaremos marcador marcador—, de radio ligeramente mayor y de color configurable, que indica el avance del secuenciador paso a paso.

Cada paso puede adoptar tres estados visuales:
- Gris: estado por defecto.
- Blanco: acompaña el avance del marcador, generando un efecto visual de estela.
- Magenta: indica que el paso está activo.

Los pasos se activan o desactivan posicionando el mouse sobre el paso elegido y haciendo click. Cuando el secuenciador alcanza un paso activo, se envía un mensaje MIDI que dispara una nota musical. 


NOTAS

Cada paso posee su propia nota, asignada al momento del setup de cada secuenciador. Inicialmente, todos los pasos comienzan con la misma nota.

Por ejemplo, el secuenciador 1  está configurado con la nota MIDI 36 (Do2).

Es posible visualizar en pantalla el nombre de la nota y su octava simplemente al ubicar el mouse sobre cualquier paso. La información se imprime utilizando el mismo color del secuenciador correspondiente.

Mientras la nota está siendo visualizada, puede modificarse mediante las teclas “q” y “w”, que incrementan o decrementan la nota por semitonos respectivamente. El cambio realizado queda guardado a lo largo de toda la sesión.


MOVIMIENTO

El secuenciador gira con una velocidad angular ω y puede hacerlo en sentido horario o antihorario. La velocidad y el sentido de giro se ajustan mediante los parámetros BPM y figura.

Ejemplos:
 - BPM = 60 
- > figura = 1 → un giro por segundo.
- > figura = 2 → un giro cada dos segundos.
- > figura = 0.5 → dos giros por segundo.
- > figura < 0 → giro en sentido antihorario.

Por defecto, todos los secuenciadores comparten el mismo BPM, aunque este puede ajustarse individualmente para cada objeto.


POLÍGONOS

Si el número de pasos activos de un secuenciador es mayor o igual a dos, al activar Polígonos se traza una línea recta entre ellos.

Cuando el número de puntos activos es mayor o igual a tres, se visualizan polígonos cuyos lados corresponden a la cantidad de puntos activos. Estos polígonos también pueden rellenarse con color.

Este aspecto es principalmente lúdico y no afecta el funcionamiento del sistema pero constituye una de las principales motivaciones del proyecto, ya que permite pensar los ritmos como formas geométricas.



CONTROL

OpenFrameworks gestiona señales MIDI en ambas direcciones, permitiendo tanto el envío como la recepción de mensajes. Lo mismo ocurre con los comandos provenientes del teclado y del mouse de la computadora: el sistema recibe, modifica, interactúa y reenvía información posibilitando un control en tiempo real de los distintos procesos.
Por ejemplo:
- Se reciben mensajes desde el dispositivo MIDI Korg nanoKONTROL a través del CC#5, que luego son redireccionados al CC #70 en Ableton Live, asignado a la frecuencia de corte de un filtro pasa bajos (LPF).
- Mediante el mouse es posible activar o desactivar pasos del secuenciador, o mutear canales del mixer de Ableton desde la pantalla de openFrameworks.
- Con la barra espaciadora se puede iniciar o detener la reproducción de los secuenciadores, o con las letras q y w buscar y seleccionar una nota deseada.

SONIDO
El sonido es gestionado íntegramente desde Ableton Live, utilizando una configuración de salidas múltiples.
Se emplea BlackHole como interface de audio virtual junto a la salida de auriculares integrada de la computadora.
Desde Ableton se configura el master a las salidas 1 y 2 con destino a la salida de línea de la computador, mientras que las salidas 5 y 6  son usadas por un bus auxiliar para enviar la señal de audio de vuelta hacia openFrameworks, y las salidas 3 y 4 van a la aplicación nativa que capturan el video y el audio de lo que se ve en pantalla. 
La señal enviada por las salidas 5 y 6  es tomada por oF para controlar el brillo del relleno del Secuenciador 4  y generar un efecto visual reactivo.
Cada secuenciador controla un sintetizador virtual o un sampler. En el caso del Secuenciador 2, dos secuenciadores independientes envían información a un mismo instrumento permitiendo polifonía.


Referencias

- Toussaint, Godfried T. 2013. The Geometry of Musical Rhythm: What Makes a “Good” Rhythm Good?. Chapman & Hall/CRC (Taylor & Francis Group).

- Okazaki, Miles. 2015. Fundamentals of Guitar. Mel Bay Publications, Inc.
