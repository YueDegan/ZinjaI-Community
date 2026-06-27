# Manual del Desarrollador

## Propósito

Este manual documenta la estructura interna del IDE ZinjaI desde la perspectiva de desarrollo.

Su objetivo es explicar cómo está organizado el sistema, cómo interactúan sus módulos y qué responsabilidades tiene cada componente principal.

Está orientado a desarrolladores que:

- Desean entender la arquitectura del proyecto.
- Quieren modificar o extender funcionalidades.
- Necesitan ubicar rápidamente qué parte del código modificar.

---

## Estado

- Estado: En construcción
- Verificado: Parcial

Este manual se encuentra en fase inicial de documentación. La estructura general ha sido definida, pero el análisis detallado de cada módulo está en progreso.

---

## Alcance

Este documento no describe:

- Funciones individuales del código fuente (ver Referencia Técnica - Doxygen).
- Uso del programa desde la perspectiva del usuario (ver Manual del Usuario).

---

## Arquitectura general

ZinjaI está organizado en módulos funcionales independientes que interactúan entre sí.

```text id="arch01"
                 ┌──────────────┐
                 │    Editor    │
                 └──────┬───────┘
                        │
                        ▼
               ┌─────────────────┐
               │ Autocompletado  │
               └─────────────────┘
                        │
                        ▼
               ┌─────────────────┐
               │    Parser       │
               └─────────────────┘
                        │
                        ▼
               ┌─────────────────┐
               │ Sistema build   │
               └─────────────────┘
```

---

## Módulos principales

Los siguientes módulos y componentes componen el sistema ZinjaI:

| Nombre | Tipo | Descripción | Estado |
|--------|------|-------------|--------|
| [`autocomp`](autocomp/index.md) | Módulo | Sistema de autocompletado basado en listas estáticas y análisis de contexto. | En análisis |
| [`colours`](colours/index.md) | Módulo | Definición y gestión de esquemas de colores. | Pendiente |
| [`complements`](complements/index.md) | Módulo | Componentes auxiliares del sistema. | En análisis |
| [`configs`](configs/index.md) | Módulo | Manejo de configuraciones del usuario y del entorno del IDE. | Pendiente |
| [`cppreference`](cppreference/index.md) | Recurso | Colección de documentación de referencia del lenguaje C/C++ utilizada por el IDE. | Pendiente |
| [`docs`](docs/index.md) | Documentación | Manuales y documentación técnica del proyecto (usuario, desarrollador y referencia de la API). | En construcción |
| [`guihelp`](guihelp/index.md) | Módulo | Sistema de ayuda integrada dentro de la interfaz gráfica. | Pendiente |
| [`imgs`](imgs/index.md) | Recurso | Recursos gráficos utilizados por el IDE. | Pendiente |
| [`lang`](lang/index.md) | Módulo | Gestión de idiomas e internacionalización del IDE. | Pendiente |
| [`parser`](parser/index.md) | Módulo | Analiza el código fuente y construye la estructura interna del programa. | Pendiente |
| [`samples`](samples/index.md) | Módulo | Ejemplos de código o proyectos de demostración. | Pendiente |
| [`skins`](skins/index.md) | Módulo | Sistema de temas visuales e interfaz gráfica. | Pendiente |
| [`src`](src/index.md) | Módulo | Núcleo principal del IDE (lógica central del sistema). | Pendiente |
| [`src_extras`](src_extras/index.md) | Módulo | Extensiones o funcionalidades adicionales del núcleo. | Pendiente |
| [`templates`](templates/index.md) | Módulo | Plantillas de proyectos y archivos. | Pendiente |
| [`third`](third/index.md) | Módulo | Bibliotecas o dependencias externas incluidas en el proyecto. | Pendiente |
| [`toolchains`](toolchains/index.md) | Módulo | Gestión de compiladores y herramientas externas. | Pendiente |
| [`.gitignore`](.gitignore) | Configuración | Archivos ignorados por Git. | Pendiente |
| [`.gitattributes`](.gitattributes) | Configuración | Configuración de atributos de Git. | Pendiente |
| [`compiling.txt`](compiling.txt) | Documento | Información relacionada con compilación. | Pendiente |
| [`Doxyfile`](Doxyfile) | Configuración | Archivo de configuración de Doxygen. | Pendiente |
| [`encoding_report.txt`](encoding_report.txt) | Documento | Reporte de codificación del proyecto. | Pendiente |
| [`LICENSE.md`](LICENSE.md) | Documento | Licencia del proyecto. | Pendiente |
| [`Makefile`](Makefile) | Configuración | Script de compilación del proyecto. | Pendiente |
| [`README.md`](README.md) | Documento | Documentación principal del repositorio. | Pendiente |
| [`readme.txt`](readme.txt) | Documento | Documentación principal del repositorio. | Pendiente |
| [`zinjai.dir`](zinjai.dir) | Configuración | Archivo de configuración del proyecto IDE. | Pendiente |

## Flujo general del sistema

```text id="flow01"
Usuario escribe código
        │
        ▼
Editor
        │
        ▼
Parser ─────► Autocompletado
        │
        ▼
Sistema de compilación
        │
        ▼
Ejecutable generado
```

---

## Relación con otros manuales

Este documento forma parte de un sistema de documentación dividido en tres niveles:

- **Manual del Usuario** → Uso del IDE.
- **Manual del Desarrollador** → Arquitectura interna (este documento).
- **Referencia Técnica (Doxygen)** → API del código fuente.

---

## Cómo usar este manual

Para entender el sistema de forma progresiva:

1. Leer esta página (visión general).
2. Explorar los módulos individuales.
3. Consultar la referencia técnica (Doxygen) para detalles de implementación.

---

## Pendiente de investigación

- Detalle exacto del flujo de inicialización del IDE.
- Relación real entre editor y parser.
- Orden de carga de módulos al iniciar el programa.
- Arquitectura de eventos internos del sistema GUI.