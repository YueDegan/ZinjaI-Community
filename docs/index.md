# ZinjaI Community Documentation

Sistema de documentación del proyecto ZinjaI Community.

---

## Propósito

Este sitio centraliza toda la documentación del proyecto en tres niveles:

- Manual del Usuario → Uso del IDE desde la perspectiva del usuario final.
- Manual del Desarrollador → Arquitectura interna, módulos y diseño del sistema.
- Referencia Técnica (API) → Documentación generada automáticamente desde el código fuente (Doxygen).

---

## Estado del proyecto

- Estado: En construcción
- Cobertura: Inicial
- Estructura: Definida, en expansión

La documentación se encuentra en proceso de construcción progresiva. Los módulos serán documentados de forma incremental a medida que se analice el código fuente.

---

## Secciones disponibles

### Manual del Usuario

Documentación orientada al uso del IDE.

Incluye:

- Instalación
- Configuración básica
- Uso del editor
- Flujo de compilación
- Herramientas integradas

Acceder a: [`User Manual`](user/index.md)

---

### Manual del Desarrollador

Documentación técnica de la arquitectura interna del sistema.

Incluye:

- Estructura del proyecto
- Módulos principales
- Flujo interno del IDE
- Interacción entre componentes
- Análisis progresivo del código fuente

Acceder a: [`Developer Manual`](developer/index.md)

---

### Referencia Técnica (API)

Documentación generada automáticamente mediante Doxygen.

Incluye:

- Clases
- Funciones
- Estructuras
- APIs internas del sistema

Acceder a: [`API Reference`](api/index.html)

---

## Flujo de navegación recomendado

```text id="navflow01"
Inicio
  │
  ├── User Manual
  ├── Developer Manual
  └── API Reference
```

---

## Notas importantes

- La documentación del desarrollador y usuario está escrita manualmente en Markdown.
- La referencia técnica es generada automáticamente desde el código fuente.
- Las tres secciones están diseñadas para complementarse entre sí.

---

## Objetivo del sistema

El objetivo de esta documentación es:

- Facilitar la comprensión del código fuente de ZinjaI.
- Permitir mantenimiento a largo plazo del proyecto.
- Servir como guía para desarrolladores nuevos.
- Centralizar conocimiento técnico y funcional en un único punto.

---

## Estado de evolución

Este sistema de documentación evolucionará en fases:

1. Estructura base (actual)
2. Documentación de módulos principales
3. Documentación completa de arquitectura interna
4. Integración profunda con código fuente (Doxygen + MkDocs)