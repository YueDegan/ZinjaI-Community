### AAA_STL_11

---

#### Propósito

Proporcionar al sistema de autocompletado las declaraciones correspondientes a los componentes de la Standard Template Library (STL) incorporados o ampliados con el estándar C++11, permitiendo reconocer nuevos contenedores, estructuras de datos y utilidades propias de esta versión del lenguaje.

---

#### Estado

- Estado: Documentado
- Verificado: Parcial

---

#### Formato

Archivo de texto plano.

Características:

- Organizado por cabeceras de la STL.
- Contiene declaraciones simplificadas de clases, funciones y plantillas.
- Incluye el espacio de nombres `std`.

---

### Contenido

El archivo agrupa declaraciones correspondientes a los principales componentes de la STL introducidos con C++11.

Bibliotecas identificadas:

| Cabecera | Descripción |
|----------|-------------|
| `<unordered_set>` | Contenedores asociativos basados en tablas hash (`unordered_set` y `unordered_multiset`). |
| `<unordered_map>` | Contenedores asociativos clave-valor basados en tablas hash (`unordered_map` y `unordered_multimap`). |
| `<forward_list>` | Lista simplemente enlazada optimizada para inserciones y eliminaciones. |
| `<array>` | Contenedor de tamaño fijo con interfaz compatible con la STL. |

Además de las cabeceras, el archivo declara los principales tipos, operadores y funciones asociados a estos contenedores, incluyendo:

- Comparadores (`==`, `!=`, `<`, `>`, `<=`, `>=`).
- Funciones `swap()`.
- Iteradores.
- Constructores.
- Operaciones de inserción, eliminación y acceso.
- Métodos específicos de cada contenedor.

---

### Funcionamiento

Este archivo amplía la base de datos utilizada por el motor de autocompletado con los nuevos contenedores y estructuras incorporados por la STL de C++11.

Gracias a estas declaraciones, el IDE puede sugerir correctamente clases, métodos y operadores relacionados con los contenedores modernos sin depender del análisis de las cabeceras reales del compilador.

#### Verificado

- El archivo declara el espacio de nombres `std`.
- El contenido está organizado por cabeceras de la STL.
- Incluye las nuevas estructuras introducidas en C++11.
- Se declaran clases, operadores, iteradores y funciones asociadas a cada contenedor.

#### Inferido

- El motor de autocompletado carga este archivo cuando el proyecto utiliza el estándar C++11 o superior.
- Las declaraciones son utilizadas únicamente como fuente de información para el autocompletado.
- Este archivo complementa a `AAA_STL`, que contiene los componentes tradicionales de la STL.

---

#### Dependencias

Ninguna.

Es un archivo de datos estáticos utilizado por el sistema de autocompletado.

---

#### Utilizado por

Pendiente de localizar en el código fuente.

Se presume que es consumido por el motor de autocompletado (`autocomp`) cuando el estándar configurado corresponde a C++11 o superior.

---

#### Flujo relacionado

```text
Editor
    │
    ▼
Motor de Autocompletado
    │
    ▼
AAA_STL_11
    │
    ▼
Contenedores STL introducidos en C++11
    │
    ▼
Sugerencias al usuario
```

---

### Consideraciones de mantenimiento

Modificar este archivo cuando:

- Se incorporen nuevos contenedores compatibles con C++11.
- Se detecten declaraciones incompletas o incorrectas.
- Se amplíe el soporte del IDE para versiones posteriores del estándar.
- Se actualicen operadores o miembros públicos de los contenedores soportados.

Debe mantenerse consistente con la implementación del estándar C++11 soportada por el IDE.

---

## Pendiente de investigación

- ¿Cómo selecciona el motor de autocompletado este archivo según el estándar configurado?
- ¿Existe una carga independiente de cada cabecera o se procesa el archivo completo?
- ¿Qué parser interpreta internamente estas declaraciones?
- ¿Cómo interactúa este archivo con `AAA_STL` durante la construcción de la base de símbolos?

---