# Módulo: Autocompletado

## Descripción

El directorio `autocomp/` contiene los recursos utilizados por el sistema de autocompletado de ZinjaI.

Su función principal es proporcionar listas de palabras clave, bibliotecas y símbolos que el editor puede utilizar para ofrecer sugerencias mientras el usuario escribe código.

Este directorio no contiene código fuente ejecutable; almacena archivos de datos utilizados por el motor de autocompletado.

---

## Organización

Actualmente el módulo contiene los siguientes archivos:

| Archivo | Descripción |
|---------|-------------|
| [AAA_Directivas_de_Preprocesador](AAA_Directivas_de_Preprocesador.md) | Directivas del preprocesador de C/C++. |
| [AAA_Estandar_C](AAA_Estandar_C.md) | Funciones y símbolos de la biblioteca estándar de C. |
| [AAA_Estandar_Cpp]() | Biblioteca estándar de C++. |
| [AAA_Estandar_Cpp_11]() | Componentes introducidos en C++11. |
| [AAA_Palabras_Reservadas]() | Palabras reservadas del lenguaje C++. |
| [AAA_Palabras_Reservadas_11]() | Palabras reservadas añadidas en C++11. |
| [AAA_STL]() | Componentes de la STL clásica. |
| [AAA_STL_11]() | Componentes de la STL incorporados en C++11. |
| [OpenGL_gl]() | API principal de OpenGL. |git 
| [OpenGL_glu]() | Biblioteca GLU. |
| [OpenGL_glut]() | Biblioteca GLUT. |
| [OpenGL_glx]() | Extensiones GLX. |

---

## Funcionamiento general

Cuando el usuario escribe código, el sistema de autocompletado consulta estos archivos para obtener las sugerencias disponibles.

En términos generales, el flujo esperado es:

```text
Editor
      │
      ▼
Motor de Autocompletado
      │
      ▼
Archivos de autocomp/
      │
      ▼
Lista de sugerencias
```

La lógica que interpreta estos archivos se encuentra en el código fuente del IDE y se documentará en la referencia técnica (Doxygen) cuando analicemos el módulo correspondiente.

---

## Estado

**Estado actual:** En análisis.

La estructura del directorio ha sido identificada, pero aún no se ha documentado el formato interno de todos los archivos ni el mecanismo utilizado por el motor de autocompletado para cargarlos.

## Formato de los archivos

Todos los archivos de este directorio son archivos de texto plano.

Cada línea representa una entrada independiente utilizada por el sistema de autocompletado.

Actualmente no se conocen directivas adicionales, comentarios ni metadatos asociados. Esta información se actualizará conforme se analice el código fuente responsable de procesarlos.

## Mantenimiento

Cuando se añadan nuevas palabras clave, bibliotecas o estándares del lenguaje, deberán actualizarse los archivos correspondientes de este directorio.

Después de realizar cambios, se recomienda verificar:

- Que el autocompletado continúe funcionando correctamente.
- Que no existan entradas duplicadas.
- Que la nueva documentación refleje los cambios realizados.
