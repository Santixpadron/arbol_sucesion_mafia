#ifndef SUCESION_H
#define SUCESION_H

#include "arbol.h"

/* Mostrar la linea de sucesion actual (solo vivos) */
void mostrar_linea_sucesion(Miembro *raiz);

/* Procesar la muerte de un miembro. Si era jefe, asignar nuevo jefe. */
void procesar_muerte(Miembro *raiz, int id);

/* Procesar encarcelamiento. Si era jefe, asignar nuevo jefe. */
void procesar_encarcelamiento(Miembro *raiz, int id);

/* Procesar liberacion de carcel */
void procesar_liberacion(Miembro *raiz, int id);

/* Verificar si un jefe paso de 70 y reasignar si es necesario */
void verificar_edad_jefe(Miembro *raiz);

/* Asignar nuevo jefe segun las reglas de sucesion */
void asignar_nuevo_jefe(Miembro *raiz, Miembro *jefe_muerto);

#endif
