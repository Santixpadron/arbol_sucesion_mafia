#ifndef SUCESION_H
#define SUCESION_H

#include "arbol.h"

// Mostrar linea de sucesion (solo vivos)
void mostrar_linea_sucesion(Miembro *raiz);

// Procesar muerte de un miembro
void procesar_muerte(Miembro *raiz, int id);

// Procesar encarcelamiento
void procesar_encarcelamiento(Miembro *raiz, int id);

// Procesar liberacion de carcel
void procesar_liberacion(Miembro *raiz, int id);

// Verificar si el jefe supera los 70 anios
void verificar_edad_jefe(Miembro *raiz);

// Asignar nuevo jefe segun reglas de sucesion
void asignar_nuevo_jefe(Miembro *raiz, Miembro *jefe_muerto);

#endif
