# AAA_Estandar_C

---

#### Propósito

Proporcionar al sistema de autocompletado un conjunto de declaraciones correspondientes a la biblioteca estándar del lenguaje C (y las interfaces compatibles utilizadas desde C++), permitiendo sugerencias de funciones, constantes y espacios de nombres sin necesidad de analizar los archivos de cabecera del compilador.

---

#### Estado

- Estado: En análisis
- Verificado: Parcial

---

#### Formato

Archivo de texto plano.

Características:

- Codificación UTF-8.
- Organización por bibliotecas estándar.
- Declaraciones similares a prototipos de funciones.
- Puede incluir espacios de nombres, cabeceras, funciones, sobrecargas y constantes.

---

### Bibliotecas representadas

Este archivo contiene declaraciones correspondientes a las principales cabeceras de la biblioteca estándar de C, accesibles desde C++ mediante sus equivalentes con prefijo `c`.

Incluye actualmente:

| Cabecera | Descripción |
|----------|-------------|
| `<cmath>` | Funciones matemáticas. |
| `<cstring>` | Manipulación de cadenas y memoria. |
| `<cstdlib>` | Utilidades generales, conversión, memoria dinámica y control del proceso. |
| `<cstdio>` | Entrada y salida estándar. |
| `<ctime>` | Manejo de fecha y hora. |
| `<cassert>` | Macros de comprobación mediante `assert`. |
| `<cctype>` | Clasificación y conversión de caracteres. |
| `<cerrno>` | Manejo de códigos de error del sistema. |
| `<cfloat>` | Límites de tipos de punto flotante. |
| `<climits>` | Límites de tipos enteros. |
| `<clocale>` | Configuración regional (locale). |
| `<csetjmp>` | Saltos no locales (`setjmp` / `longjmp`). |
| `<csignal>` | Manejo de señales. |
| `<cstdarg>` | Argumentos variables. |
| `<cstddef>` | Definiciones básicas como `size_t` y `nullptr_t` (según el estándar). |

Su estructura imita parcialmente las declaraciones existentes en las bibliotecas estándar para facilitar el autocompletado.

---

### Funcionamiento

El archivo actúa como una base de datos de símbolos utilizada por el motor de autocompletado.

Cuando el usuario escribe código relacionado con la biblioteca estándar, el IDE puede sugerir funciones disponibles sin consultar directamente los archivos del compilador.

#### Verificado

- El archivo contiene declaraciones de funciones organizadas por biblioteca estándar.
- Se observan múltiples sobrecargas de una misma función.
- Se incluyen constantes utilizadas habitualmente en programación científica.

#### Inferido

- El motor de autocompletado interpreta este archivo como una lista de símbolos disponibles.
- Las declaraciones probablemente no son compiladas, sino procesadas como información para generar sugerencias..

---

#### Dependencias

Ninguna.

Es un archivo de datos estáticos.

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
AAA_Estandar_C
    │
    ▼
Lista de símbolos de la biblioteca estándar
    │
    ▼
Sugerencias al usuario
```

---

### Consideraciones de mantenimiento

Modificar este archivo cuando:

- Se incorporen nuevas funciones de la biblioteca estándar.
- Se añadan nuevas cabeceras soportadas por el IDE.
- Se detecten declaraciones incorrectas o incompletas.
- Se amplíe el soporte para nuevos estándares del lenguaje.

Debe mantenerse sincronizado con la evolución de la biblioteca estándar compatible con la versión objetivo del compilador.

---

## Pendiente de investigación

- ¿Quién carga este archivo?
- ¿Existe un parser específico para este formato?
- ¿Cómo identifica el sistema el inicio de cada cabecera (`<cmath>`, `<vector>`, etc.)?
- ¿Las declaraciones son utilizadas únicamente para autocompletado o también para ayuda contextual?
- ¿El archivo cubre toda la biblioteca estándar o únicamente una selección de funciones frecuentes?

---

