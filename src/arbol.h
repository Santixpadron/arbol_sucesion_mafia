#ifndef ARBOL_H
#define ARBOL_H

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Miembro {
    int id;
    char nombre[50];
    char apellido[50];
    char genero; /* 'H' o 'M' */
    int edad;
    int id_jefe;
    int esta_muerto;
    int en_carcel;
    int fue_jefe;
    int es_jefe;
    Miembro *izq;  /* primer sucesor (hijo izquierdo) */
    Miembro *der;  /* segundo sucesor (hijo derecho) */
};

/* Crear un nuevo nodo miembro */
Miembro *crear_miembro(int id, const char *nombre, const char *apellido,
                       char genero, int edad, int id_jefe,
                       int esta_muerto, int en_carcel,
                       int fue_jefe, int es_jefe);

/* Buscar un miembro por id en el arbol */
Miembro *buscar_por_id(Miembro *raiz, int id);

/* Insertar un miembro en el arbol segun su id_jefe */
int insertar_miembro(Miembro *raiz, Miembro *nuevo);

/* Imprimir el arbol con indentacion */
void imprimir_arbol(Miembro *raiz, int nivel);

/* Encontrar al jefe actual */
Miembro *encontrar_jefe(Miembro *raiz);

/* Encontrar al padre de un nodo */
Miembro *encontrar_padre(Miembro *raiz, int id);

/* Encontrar el primer sucesor vivo y libre en un subarbol */
Miembro *primer_sucesor_vivo_libre(Miembro *nodo);

/* Encontrar el primer sucesor vivo (incluyendo presos) en un subarbol */
Miembro *primer_sucesor_vivo(Miembro *nodo);

/* Liberar toda la memoria del arbol */
void liberar_arbol(Miembro *raiz);

/* Contar miembros vivos */
int contar_vivos(Miembro *raiz);

#endif
