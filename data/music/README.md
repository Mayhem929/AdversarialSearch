## Música de fondo para el Parchis

Esta es solo una propuesta de música de fondo para usar en la práctica 3 de Inteligencia Artificial mientras juegas partidas al Parchís.

No es necesario usarla, y el programa funcionará perfectamente sin ella. Pero si te parece más divertido jugar con música, adelante. ¡También puedes poner tú tu propia música!

Para añadir este conjunto de músicas, simplemente copia esta carpeta dentro de la carpeta data/, manteniendo su nombre actual (music) en tu directorio de la práctica. Dentro de data/music/ deben estar todos los ficheros de sonido.

Si quieres usar otra música de fondo u efecto de sonido, puedes sustituir el archivo de música que desees con el que tú elijas. Tienen que ser .wav siempre.

Si para la música de fondo has elegido una melodía que puede ciclar, puedes indicar, en un fichero .loop con el mismo nombre, los instantes de inicio y final del ciclo (en segundos, pueden tener decimales). Simplemente añade en el fichero .loop ambos números en la misma línea separados por un espacio. Si no añades este fichero, la música volverá a empezar desde el principio cuando acabe.

Estos son los ficheros de sonido que suenan durante el juego, y cuándo se usan:

- background_theme.wav: música de fondo del juego (de normal)
- background_theme_hurryup.wav: música de fondo del juego cuando la partida está cerca de su final.
- background_theme{1-n}.wav: algunas alternativas a la música de fondo del juego. Para que suenen renómbralas a background_theme.wav o background_theme_hurryup.wav.
- applause.wav: efecto de sonido para cuando una ficha llega a la meta.
- bad_news.wav: efecto de sonido para cuando una ficha es comida.
- boing.wav: efecto de sonido para cuando una ficha rebota en la meta.
- epic_win.wav: efecto de sonido para cuando un jugador gana.
- epic_lose.wav: efecto de sonido para cuando un jugador pierde.
- nope.wav: efecto de sonido para cuando se intenta hacer un movimiento prohibido.
- teleport.wav: efecto de sonido para los movimientos normales de una ficha.
- explosion.wav: efecto de sonido para los golpeos de los caparazones.
- boo.wav: efecto de sonido para cuando se activa un dado fantasma.
- bullet.wav: efecto de sonido para cuando se activa un dado bala.
- horn.wav: efecto de sonido para cuando se activa un dado bocina.
- mega.wav: música de fondo para cuando hay megafichas en el tablero.
- star.wav: música de fondo para cuando hay estrellas por el tablero.
- starhit.wav: efecto de sonido para cuando la estrella se lleva por delante a otra ficha.
- shock.wav: efecto de sonido para cuando se usa un rayo.
- background_theme_shock.wav: música de fondo para cuando hay fichas pequeñas en el tablero.

En el juego, el botón con la nota musical activa la música de fondo, mientras que el botón con el altavoz activa los efectos de sonido.