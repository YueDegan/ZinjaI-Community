# `AAA_Directivas_de_Preprocesador`

Contiene una lista de directivas del preprocesador del lenguaje C/C++ utilizadas como recurso por el sistema de autocompletado de ZinjaI.

---

## Propósito

Proporcionar un conjunto de directivas del preprocesador de C/C++ para que puedan ser reconocidas y sugeridas por el IDE durante la edición de código fuente.

---

## Estado

- Estado: Documentado
- Verificado: Parcial

---

## Formato

Archivo de texto plano.

Características:

- Codificación UTF-8 (definido por compatiblidad).
- Una directiva por línea.
- No contiene comentarios ni metadatos.
- No posee una estructura jerárquica.

---

## Contenido

El archivo almacena las siguientes directivas del preprocesador:

| Directiva | Descripción |
|-----------|-------------|
| `#define` | Define una macro o constante simbólica. |
| `#undef` | Elimina la definición de una macro. |
| `#include` | Incluye el contenido de otro archivo fuente o cabecera. |
| `#if` | Inicia una compilación condicional basada en una expresión. |
| `#ifdef` | Compila el bloque si una macro está definida. |
| `#ifndef` | Compila el bloque si una macro no está definida. |
| `#else` | Define el bloque alternativo de una compilación condicional. |
| `#elif` | Evalúa una condición adicional dentro de una compilación condicional. |
| `#endif` | Finaliza un bloque de compilación condicional. |
| `#error` | Genera un error de compilación personalizado. |
| `#warning` | Genera una advertencia del compilador (cuando es soportada). |
| `#pragma` | Envía instrucciones específicas al compilador. |
| `#line` | Modifica la numeración de líneas y el nombre del archivo informados al compilador. |

---

## Funcionamiento

### Verificado

- El archivo contiene exclusivamente una lista de directivas del preprocesador de C/C++.
- Cada línea representa una única entrada.
- No contiene código ejecutable ni lógica de procesamiento.

### Inferido

- Es utilizado por el sistema de autocompletado para ofrecer sugerencias relacionadas con directivas del preprocesador.
- El contenido probablemente se carga durante la inicialización del motor de autocompletado o cuando este requiere construir la lista de sugerencias.

---

## Dependencias

No posee dependencias directas conocidas.

Su interpretación depende del componente del IDE encargado de procesar los archivos del directorio `autocomp/`.

---

## Utilizado por

No identificado.

Se determinará cuando se documente el módulo responsable del autocompletado.

---

## Flujo relacionado

```text
Usuario escribe '#'
        │
        ▼
Editor de código
        │
        ▼
Sistema de autocompletado
        │
        ▼
AAA_Directivas_de_Preprocesador
        │
        ▼
Lista de sugerencias mostrada al usuario
```

---

## Consideraciones de mantenimiento

Modificar este archivo únicamente cuando sea necesario:

- Incorporar nuevas directivas soportadas por versiones recientes del estándar de C o C++.
- Eliminar directivas obsoletas (si el IDE deja de soportarlas).
- Mantener una entrada por línea para preservar el formato esperado por el sistema de carga.

Antes de realizar modificaciones, verificar que el componente encargado de leer este archivo no dependa de un formato adicional no documentado.

---

## Pendiente de investigación

- Identificar el componente que carga este archivo.
- Verificar el formato esperado por el parser del autocompletado.
- Confirmar la codificación utilizada por todos los archivos del directorio `autocomp/`.
- Determinar si las sugerencias se cargan al iniciar el IDE o bajo demanda.
- Comprobar si el orden de las entradas afecta la prioridad de las sugerencias.
