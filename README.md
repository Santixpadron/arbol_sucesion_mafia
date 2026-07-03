# Árbol de Sucesión - Familia Mafiosa

Programa en C++ que gestiona la línea de sucesión de una familia mafiosa mediante un **árbol binario**. Lee los datos desde un archivo CSV, permite visualizar la jerarquía, registrar muertes, encarcelamientos y modificar los datos de cada miembro. La sucesión del jefe se asigna automáticamente según las reglas establecidas.

## Estructura del Proyecto

```
arbol_sucesion_mafia/
├── README.md
├── Makefile
├── bin/
│   ├── familia.csv            # Datos de la familia (entrada/salida)
│   └── arbol_sucesion.exe     # Ejecutable generado
└── src/
    ├── main.cpp                 # Menú interactivo principal
    ├── arbol.h / arbol.cpp      # Estructura del árbol binario
    ├── csv_parser.h / csv_parser.cpp  # Lectura y escritura del CSV
    └── sucesion.h / sucesion.cpp      # Lógica de sucesión del jefe
```

## Compilación

Requiere `g++` (MinGW en Windows). Desde la raíz del proyecto:

```bash
make
```

Esto genera el ejecutable en `bin/arbol_sucesion.exe`.

**Compilación manual (sin Make):**

```bash
g++ -Wall -Wextra -std=c++11 -o bin/arbol_sucesion.exe src/main.cpp src/arbol.cpp src/csv_parser.cpp src/sucesion.cpp
```

## Ejecución

Desde la raíz del proyecto:

```bash
./bin/arbol_sucesion.exe
```

> **Importante:** El programa debe ejecutarse desde la raíz del proyecto para que encuentre el archivo `bin/familia.csv`.

## Funcionalidades

1. **Mostrar línea de sucesión** — Muestra el árbol solo con miembros vivos.
2. **Mostrar árbol completo** — Incluye muertos y presos con etiquetas visuales.
3. **Registrar muerte** — Marca un miembro como muerto; si era jefe, asigna sucesor automáticamente.
4. **Enviar a prisión** — Encarcela un miembro; si era jefe, asigna sucesor.
5. **Liberar de prisión** — Libera un miembro de la cárcel.
6. **Modificar datos** — Permite cambiar nombre, apellido, género, edad y estados (no ID ni ID del jefe).
7. **Buscar miembro** — Consulta los datos de un miembro por su ID.
8. **Guardar datos** — Guarda el estado actual del árbol de vuelta al CSV.

## Reglas de Sucesión

Cuando el jefe muere o va a prisión:

1. Se busca el **primer sucesor vivo y libre** en su subárbol.
2. Si no tiene sucesores, se busca en el **árbol del otro sucesor de su jefe** (su "hermano").
3. Si el hermano está **vivo, libre y sin sucesores**, él se vuelve jefe.
4. Se sube por el árbol buscando **ancestros con sucesores disponibles**.
5. Si **todos los sucesores libres murieron**, se aplican las mismas reglas con **presos vivos**.
6. Si un jefe **pasa de 70 años**, se le retira y se asigna sucesor.

## Formato del CSV

```
id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss
```

- `gender`: H (hombre) o M (mujer)
- `is_dead`, `in_jail`, `was_boss`, `is_boss`: 0 o 1
- `id_boss`: ID del jefe directo (0 si es la raíz del árbol)

## Datos de Prueba

El archivo `bin/familia.csv` incluye 17 miembros organizados jerárquicamente con casos de prueba: muertos, presos, y un jefe activo.