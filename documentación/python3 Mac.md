Fuente: [http://lingwars.github.io/blog/python-mac.html](http://lingwars.github.io/blog/python-mac.html)


Por defecto, tu Mac viene con Python 2 preinstalado. Sin embargo, nosotros vamos a instalar Python 3. ¿Por qué? Aunque muchos proyectos siguen utilizando Python 2, el futuro está en Python 3 y la wiki oficial de Python nos dice claramente que Python 2.x está anticuado (Python 2.x is legacy). Además, Python 3 nos ofrece muchas facilidades para gestionar cadenas de texto y ésta es una gran ventaja para nuestra labor lingüística. Así que, ¡a por Python 3!

# El Terminal
Todas nuestras manipulaciones las haremos desde el Terminal de nuestro Mac. Si nunca has usado el Terminal, no te preocupes, es muy sencillo.

Para abrirlo, simplemente tienes que teclear la palabra Terminal en el Finder o en el Spotlight, y darle un click. Se abrirá una ventana de este estilo:

ventana del Terminal de Mac.
La primera cosa que haremos será comprobar la versión de Python que tenemos por defecto. Para esto en el Terminal teclearemos:

```
Tanias-MacBook-Pro:~ tania$ python --version
```

El símbolo $ y todo lo que le precede ya está en el Terminal y por supuesto no tienes que volver a teclearlo. Solo teclearás lo que viene después del $.

Tras darle a Enter, obtendrás la respuesta:

```
Python 2.7.5
```

El resultado es bastante previsible: tenemos Python 2 instalado en nuestro Mac, concretamente la versión 2.7.5.

# Instalar Python 3.x.x
> **AVISO IMPORTANTE:** bajo ningún concepto desinstales la versión de Python 2 preinstalada en tu Mac. A lo mejor nunca la vas a utilizar, pero tu sistema operativo sí que la necesita para funcionar con normalidad. Varias versiones de Python pueden coexistir perfectamente en el mismo Mac, así que dejaremos Python 2.x en paz y abriremos la [página de descargas de Python](https://www.python.org/downloads/).

Ahí, tendremos la posibilidad de elegir entre la última versión de Python 3 y la última versión de Python 2 para Mac OS X (en el momento de escribir este post, 3.5.1 y 2.7.11 respectivamente). Por supuesto, elegiremos la primera opción. Una vez terminada la descarga, pincharemos en el archivo .pkg descargado y seguiremos los pasos habituales de instalación.

Cuando termine la instalación podemos volver al Terminal para comprobar que todo ha ido bien. Volveremos a teclear python -- version y veremos que ... no ha cambiado nada: el Terminal nos sigue diciendo que tenemos Python 2. ¿Qué hemos hecho mal? Nada. Simplemente ya tenemos dos versiones de Python pero dado que Python 2 sigue siendo la versión por defecto, al escribir python a secas estamos invocando Python 2. Si queremos invocar Python 3 tendremos que escribir python3:

```
Tanias-MacBook-Pro:~ tania$ python3 --version
Python 3.5.1
```

Ahora vemos que Python 3.5.1 se ha instalado correctamente. ¡Genial!
