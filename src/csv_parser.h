#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "arbol.h"
#include <fstream>
#include <sstream>

/* Cargar el arbol desde un archivo CSV. Retorna la raiz del arbol. */
Miembro *cargar_csv(const char *ruta);

/* Guardar el arbol de vuelta al CSV */
void guardar_csv(const char *ruta, Miembro *raiz);

#endif
