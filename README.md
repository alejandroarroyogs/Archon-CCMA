# Archon CCMA

Variación del juego Archon con ambientación en Star Wars. Proyecto final de Informática Industrial — ETSIDI UPM, curso 2025-2026.

Equipo CCMA: Alejandro Arroyo, Cristina Boned, Marcos Garrido, Carlos Bravo.

---

## Qué es

Archon es un híbrido de estrategia por turnos y combate en tiempo real. El tablero funciona como un ajedrez temático: mover piezas, controlar puntos de poder, lanzar hechizos. Pero cuando dos piezas se encuentran, en lugar de eliminar automáticamente a la más débil, se abre una arena de combate donde los jugadores se enfrentan en tiempo real.

Nuestra versión sustituye la ambientación original por el universo Star Wars: Baby Yoda y los Jedis contra Darth Vader y los Siths. Los hechizos pasan a llamarse Curación de la Fuerza, Salto Hiperespacial, Alteración Cronológica, Confusión Mental, Bloqueo Carbonita y Espíritu de la Fuerza.

## Cómo se juega

- **WASD** — mover el cursor por el tablero
- **Espacio** — seleccionar pieza / confirmar movimiento
- **Teclas 1-6** — lanzar hechizos (solo con el cursor sobre el líder propio)
- **Q** — cancelar selección
- **WASD / flechas** — mover la pieza en la arena de combate
- **Clic** — navegar los menús

Hay modo de un jugador contra la IA (con tres niveles de dificultad) y modo de dos jugadores en el mismo teclado.

## Cómo ejecutar

Descomprimir y hacer doble clic en `bin/Archon.exe`. Requiere Windows 10/11 de 64 bits. No necesita instalación.

Para compilar desde código fuente, abrir `Archon.sln` con Visual Studio y compilar en configuración `Debug x64` o `Release x64`.

## Tecnologías

C++ · OpenGL · FreeGLUT · ETSIDI (texturas, audio, texto) · FMOD
