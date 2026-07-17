# Árbol de Sucesión — Familia Mafiosa 🎩

Programa en **C++** que gestiona la línea de sucesión de una familia mafiosa mediante un **árbol binario**. Lee los datos desde un archivo CSV, permite visualizar la jerarquía, registrar muertes, encarcelamientos y modificar los datos de cada miembro. La sucesión del jefe se asigna automáticamente según las reglas establecidas.

---


## Clonar el Repositorio

Abre una terminal (CMD, PowerShell o Git Bash) y ejecuta:

```bash
git clone https://github.com/Santixpadron/arbol_sucesion_mafia.git
```

Ingresa a la carpeta del proyecto:

```bash
cd arbol_sucesion_mafia
```

---

## Estructura del Proyecto

```
arbol_sucesion_mafia/
├── README.md                    # Este archivo
├── Makefile                     # Archivo de compilación
├── .gitignore                   # Archivos ignorados por Git
├── bin/
│   ├── familia.csv              # Datos de la familia (entrada/salida)
│   └── arbol_sucesion.exe       # Ejecutable generado (después de compilar)
└── src/
    ├── main.cpp                 # Menú interactivo principal
    ├── arbol.h                  # Definición del struct y prototipos del árbol
    ├── arbol.cpp                # Implementación del árbol binario
    ├── csv_parser.h             # Prototipos de lectura/escritura CSV
    ├── csv_parser.cpp           # Implementación del parser CSV
    ├── sucesion.h               # Prototipos de lógica de sucesión
    └── sucesion.cpp             # Implementación de reglas de sucesión
```

---

## Compilación

### Opción A: Usar `make` (recomendado)

Desde la **raíz del proyecto** (`arbol_sucesion_mafia/`), ejecuta:

```bash
make
```

Esto compilará todos los archivos fuente y generará el ejecutable en `bin/arbol_sucesion.exe`.

Si deseas limpiar el ejecutable anterior antes de recompilar:

```bash
make clean
make
```

### Opción B: Compilación manual (sin `make`)

Si no tienes `make` instalado, puedes compilar directamente con `g++`. Desde la **raíz del proyecto**, ejecuta:

```bash
g++ -Wall -Wextra -std=c++11 -o bin/arbol_sucesion.exe src/main.cpp src/arbol.cpp src/csv_parser.cpp src/sucesion.cpp
```

**Explicación de los flags:**
| Flag | Significado |
|------|-------------|
| `-Wall` | Activa todos los warnings comunes |
| `-Wextra` | Activa warnings adicionales |
| `-std=c++11` | Usa el estándar C++11 |
| `-o bin/arbol_sucesion.exe` | Nombre y ubicación del ejecutable de salida |

Si la compilación es exitosa, no mostrará ningún mensaje y el archivo `bin/arbol_sucesion.exe` será creado.

---

## Ejecución

### Paso 1: Verifica que el CSV existe

Antes de ejecutar, asegúrate de que el archivo `bin/familia.csv` existe. Este archivo contiene los datos iniciales de la familia. Si no existe, el programa mostrará un error.

### Paso 2: Ejecuta el programa

**Desde la raíz del proyecto** (`arbol_sucesion_mafia/`), ejecuta:

**En CMD o PowerShell:**
```bash
.\bin\arbol_sucesion.exe
```

**En Git Bash o Linux:**
```bash
./bin/arbol_sucesion.exe
```

> ⚠️ **MUY IMPORTANTE:** El programa **DEBE** ejecutarse desde la raíz del proyecto (`arbol_sucesion_mafia/`), NO desde la carpeta `bin/`. Esto es porque el programa busca el archivo CSV en la ruta relativa `bin/familia.csv`.

### Paso 3: Interactúa con el menú

Al iniciar, verás el siguiente menú:

```
============================================
  ARBOL DE SUCESION - FAMILIA MAFIOSA
============================================

Cargando datos desde 'bin/familia.csv'...
Arbol cargado exitosamente. 15 miembros vivos.

============================================
              MENU PRINCIPAL
============================================
  1. Mostrar linea de sucesion (vivos)
  2. Mostrar arbol completo
  3. Registrar muerte de un miembro
  4. Enviar a prision
  5. Liberar de prision
  6. Modificar datos de un miembro
  7. Buscar miembro por ID
  8. Guardar datos al CSV
  0. Salir
============================================
Seleccione una opcion:
```

Escribe el **número** de la opción que deseas y presiona **Enter**.

---

## Opciones del Menú

### 1. Mostrar línea de sucesión (vivos)
Muestra el árbol jerárquico **solo con miembros vivos**. Los muertos se omiten. Los presos aparecen con la etiqueta `[PRESO]` y los ex-jefes con `[EX-JEFE]`.

### 2. Mostrar árbol completo
Muestra **todos** los miembros del árbol, incluyendo muertos y presos, con etiquetas visuales:
- `[JEFE]` — Jefe actual de la familia
- `[MUERTO]` — Miembro fallecido
- `[PRESO]` — Miembro en prisión
- `[EX-JEFE]` — Fue jefe anteriormente

### 3. Registrar muerte de un miembro
Solicita el **ID** del miembro. Lo marca como muerto. Si el miembro era el jefe, **automáticamente** busca y asigna un nuevo jefe según las reglas de sucesión.

### 4. Enviar a prisión
Solicita el **ID** del miembro y lo envía a prisión. Si era el jefe, se asigna un nuevo jefe automáticamente.

### 5. Liberar de prisión
Solicita el **ID** del miembro y lo libera de prisión.

### 6. Modificar datos de un miembro
Solicita el **ID** del miembro y muestra un submenú para modificar:
- Nombre
- Apellido
- Género (H/M)
- Edad
- Estado de muerte
- Estado de cárcel
- Si fue jefe
- Si es jefe

> **Nota:** No se pueden modificar el ID ni el ID del jefe.

### 7. Buscar miembro por ID
Solicita el **ID** y muestra toda la información del miembro: nombre, apellido, género, edad, jefe, estados, etc.

### 8. Guardar datos al CSV
Guarda el estado actual de todo el árbol de vuelta al archivo `bin/familia.csv`. Los cambios realizados durante la sesión se escriben en el archivo.

### 0. Salir
Guarda automáticamente los datos al CSV y cierra el programa.

---

## Reglas de Sucesión

Cuando el jefe muere, va a prisión, o supera los 70 años de edad:

1. Se busca el **primer sucesor vivo y libre** en su subárbol (hijos y descendientes).
2. Si no hay sucesores disponibles, se busca en el **árbol del hermano** del jefe (el otro hijo de su jefe).
3. Si el hermano está **vivo, libre y sin sucesores propios**, él asume la jefatura.
4. Se sube por el árbol buscando **ancestros con sucesores disponibles**.
5. Si **no hay ningún miembro libre**, se aplican las mismas reglas pero con **presos vivos**.
6. Si un jefe **supera los 70 años** de edad, se le retira automáticamente y se asigna un sucesor.
7. Si no queda **ningún miembro vivo**, se muestra: *"La familia ha caído"*.

---

## Formato del Archivo CSV

El archivo `bin/familia.csv` tiene el siguiente formato:

```csv
id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss
1,Vito,Corleone,H,72,0,0,0,0,1
2,Sonny,Corleone,H,50,1,0,0,0,0
```

| Campo | Descripción | Valores |
|-------|-------------|---------|
| `id` | Identificador único del miembro | Número entero |
| `name` | Nombre del miembro | Texto |
| `last_name` | Apellido del miembro | Texto |
| `gender` | Género | `H` (hombre) o `M` (mujer) |
| `age` | Edad del miembro | Número entero |
| `id_boss` | ID del jefe directo | Número (0 = raíz del árbol) |
| `is_dead` | ¿Está muerto? | `0` (No) o `1` (Sí) |
| `in_jail` | ¿Está en prisión? | `0` (No) o `1` (Sí) |
| `was_boss` | ¿Fue jefe? | `0` (No) o `1` (Sí) |
| `is_boss` | ¿Es el jefe actual? | `0` (No) o `1` (Sí) |

---

## Datos de Prueba

El archivo `bin/familia.csv` incluye **17 miembros** organizados jerárquicamente con los siguientes casos de prueba ya configurados:
- Miembros muertos (ej: Rocco, Clemenza)
- Miembros en prisión (ej: Carlo, Fredo)
- Un jefe activo (Vito Corleone, 72 años — activa la regla de retiro por edad)

---

## Solución de Problemas

| Problema | Solución |
|----------|----------|
| `g++ no reconocido` | Asegúrate de que MinGW está en el PATH y abriste una **nueva** terminal |
| `No se pudo abrir bin/familia.csv` | Ejecuta el programa desde la raíz del proyecto, no desde `bin/` |
| `make no reconocido` | Usa `mingw32-make` en vez de `make`, o compila manualmente con `g++` |
| El programa no compila | Verifica que tienes todos los archivos `.cpp` y `.h` en la carpeta `src/` |

---

Creado por Santiago Padrón CI 32070142
