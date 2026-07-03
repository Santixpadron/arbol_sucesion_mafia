#include "arbol.h"

Miembro *crear_miembro(int id, const char *nombre, const char *apellido,
                       char genero, int edad, int id_jefe,
                       int esta_muerto, int en_carcel,
                       int fue_jefe, int es_jefe) {
    Miembro *nuevo = new Miembro();
    nuevo->id = id;
    strncpy(nuevo->nombre, nombre, 49);
    nuevo->nombre[49] = '\0';
    strncpy(nuevo->apellido, apellido, 49);
    nuevo->apellido[49] = '\0';
    nuevo->genero = genero;
    nuevo->edad = edad;
    nuevo->id_jefe = id_jefe;
    nuevo->esta_muerto = esta_muerto;
    nuevo->en_carcel = en_carcel;
    nuevo->fue_jefe = fue_jefe;
    nuevo->es_jefe = es_jefe;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}

Miembro *buscar_por_id(Miembro *raiz, int id) {
    if (!raiz) return NULL;
    if (raiz->id == id) return raiz;

    Miembro *encontrado = buscar_por_id(raiz->izq, id);
    if (encontrado) return encontrado;

    return buscar_por_id(raiz->der, id);
}

int insertar_miembro(Miembro *raiz, Miembro *nuevo) {
    if (!raiz || !nuevo) return 0;

    if (nuevo->id_jefe == 0) {
        return 0;
    }

    Miembro *padre = buscar_por_id(raiz, nuevo->id_jefe);
    if (!padre) {
        return 0;
    }

    if (!padre->izq) {
        padre->izq = nuevo;
        return 1;
    } else if (!padre->der) {
        padre->der = nuevo;
        return 1;
    } else {
        cout << "Error: El miembro " << padre->nombre << " " << padre->apellido
             << " (id=" << padre->id << ") ya tiene 2 sucesores." << endl;
        return 0;
    }
}

static void imprimir_estado(Miembro *m) {
    if (m->es_jefe) cout << " [JEFE]";
    if (m->esta_muerto) cout << " [MUERTO]";
    if (m->en_carcel) cout << " [PRESO]";
    if (m->fue_jefe) cout << " [EX-JEFE]";
}

void imprimir_arbol(Miembro *raiz, int nivel) {
    if (!raiz) return;

    for (int i = 0; i < nivel; i++) {
        if (i == nivel - 1)
            cout << "|-- ";
        else
            cout << "|   ";
    }

    cout << raiz->nombre << " " << raiz->apellido
         << " (id:" << raiz->id << ", edad:" << raiz->edad << ", " << raiz->genero << ")";
    imprimir_estado(raiz);
    cout << endl;

    imprimir_arbol(raiz->izq, nivel + 1);
    imprimir_arbol(raiz->der, nivel + 1);
}

Miembro *encontrar_jefe(Miembro *raiz) {
    if (!raiz) return NULL;
    if (raiz->es_jefe) return raiz;

    Miembro *encontrado = encontrar_jefe(raiz->izq);
    if (encontrado) return encontrado;

    return encontrar_jefe(raiz->der);
}

Miembro *encontrar_padre(Miembro *raiz, int id) {
    if (!raiz) return NULL;

    if ((raiz->izq && raiz->izq->id == id) ||
        (raiz->der && raiz->der->id == id)) {
        return raiz;
    }

    Miembro *encontrado = encontrar_padre(raiz->izq, id);
    if (encontrado) return encontrado;

    return encontrar_padre(raiz->der, id);
}

Miembro *primer_sucesor_vivo_libre(Miembro *nodo) {
    if (!nodo) return NULL;

    if (!nodo->esta_muerto && !nodo->en_carcel) {
        return nodo;
    }

    Miembro *encontrado = primer_sucesor_vivo_libre(nodo->izq);
    if (encontrado) return encontrado;

    return primer_sucesor_vivo_libre(nodo->der);
}

Miembro *primer_sucesor_vivo(Miembro *nodo) {
    if (!nodo) return NULL;

    if (!nodo->esta_muerto) {
        return nodo;
    }

    Miembro *encontrado = primer_sucesor_vivo(nodo->izq);
    if (encontrado) return encontrado;

    return primer_sucesor_vivo(nodo->der);
}

void liberar_arbol(Miembro *raiz) {
    if (!raiz) return;
    liberar_arbol(raiz->izq);
    liberar_arbol(raiz->der);
    delete raiz;
}

int contar_vivos(Miembro *raiz) {
    if (!raiz) return 0;
    int cuenta = 0;
    if (!raiz->esta_muerto) cuenta = 1;
    return cuenta + contar_vivos(raiz->izq) + contar_vivos(raiz->der);
}
