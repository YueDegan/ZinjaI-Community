### AAA_Estandar_Cpp

---

#### Propósito

Proporcionar al sistema de autocompletado un conjunto de declaraciones correspondientes a las bibliotecas fundamentales de la biblioteca estándar de C++, permitiendo sugerencias de clases, funciones, tipos y objetos propios del lenguaje sin depender directamente de las cabeceras del compilador.

---

#### Estado

- Estado: Documentado
- Verificado: Parcial

---

#### Formato

Archivo de texto plano.

Características:

- Codificación UTF-8.
- Organizado por cabeceras de la biblioteca estándar.
- Contiene declaraciones similares a prototipos y definiciones simplificadas.
- Incluye el espacio de nombres `std`.
- Puede contener clases, funciones, objetos globales y tipos definidos por la biblioteca estándar.

---

### Contenido

El archivo agrupa declaraciones correspondientes a las principales bibliotecas fundamentales de C++.

Bibliotecas identificadas:

| Cabecera | Descripción |
|----------|-------------|
| `<ios>` | Definiciones base del sistema de entrada y salida. |
| `<iostream>` | Flujos estándar (`cin`, `cout`, `cerr`, `clog`). |
| `<iomanip>` | Manipuladores para el formato de entrada y salida. |
| `<fstream>` | Lectura y escritura de archivos mediante flujos. |
| `<sstream>` | Flujos asociados a cadenas de caracteres. |
| `<string>` | Clase `std::string` y operaciones relacionadas. |
| `<typeinfo>` | Información de tipos en tiempo de ejecución (RTTI). |
| `<limits>` | Límites numéricos de los tipos fundamentales (`std::numeric_limits`). |
| `<stdexcept>` | Jerarquía de excepciones estándar de C++. |

Estas bibliotecas representan componentes esenciales del lenguaje C++ y complementan el contenido definido en `AAA_Estandar_C`.

---

### Funcionamiento

Este archivo actúa como una base de datos de símbolos utilizada por el motor de autocompletado del IDE.

Cuando el usuario trabaja con código C++, el sistema puede sugerir clases, funciones, objetos y tipos pertenecientes a estas bibliotecas sin necesidad de analizar las cabeceras reales del compilador.

#### Verificado

- El archivo comienza declarando el espacio de nombres `std`.
- Las declaraciones están organizadas por cabeceras estándar.
- El contenido corresponde a bibliotecas fundamentales del estándar C++.

#### Inferido

- El motor de autocompletado interpreta este archivo para construir el conjunto de símbolos disponibles de la biblioteca estándar.
- Las declaraciones probablemente son procesadas únicamente como datos para el autocompletado y no participan en la compilación.
- El archivo complementa a `AAA_Estandar_C`, mientras que los componentes específicos de la STL parecen estar separados en `AAA_STL` y `AAA_STL_11`.

---

#### Dependencias

Ninguna.

Es un archivo de datos estáticos utilizado por el sistema de autocompletado.

---

#### Utilizado por

Pendiente de localizar en el código fuente.

Se presume que es consumido por el motor de autocompletado (`autocomp`).

---

#### Flujo relacionado

``` none
Editor
    │
    ▼
Motor de Autocompletado
    │
    ▼
AAA_Estandar_Cpp
    │
    ▼
Símbolos de la biblioteca estándar de C++
    │
    ▼
Sugerencias al usuario
```

---

### Consideraciones de mantenimiento

Modificar este archivo cuando:

- Se incorporen nuevas cabeceras de la biblioteca estándar de C++.
- Se amplíe el soporte para nuevas versiones del lenguaje.
- Se detecten declaraciones incorrectas o incompletas.
- Se actualicen clases o componentes fundamentales soportados por el IDE.

Debe mantenerse coherente con la versión del estándar C++ soportada por ZinjaI.

---

## Pendiente de investigación

- ¿Qué componente carga este archivo durante la inicialización del autocompletado?
- ¿Existe un parser específico para interpretar estas declaraciones?
- ¿Cómo determina el IDE qué cabeceras mostrar según el estándar seleccionado?
- ¿Se cargan todas las bibliotecas al iniciar el IDE o únicamente cuando son necesarias?
- ¿Qué relación existe entre este archivo y `AAA_STL` durante el proceso de autocompletado?

---
