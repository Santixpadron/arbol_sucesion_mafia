#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "arbol.h"
#include <fstream>
#include <sstream>

// Cargar el arbol desde un archivo CSV
Miembro *cargar_csv(const char *ruta);

// Guardar el arbol al CSV
void guardar_csv(const char *ruta, Miembro *raiz);

#endif
