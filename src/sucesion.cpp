#include "sucesion.h"

// Obtiene el hermano de un nodo en el arbol
static Miembro *obtener_hermano(Miembro *raiz, Miembro *nodo) {
    Miembro *padre = encontrar_padre(raiz, nodo->id);
    if (!padre) return NULL;

    if (padre->izq && padre->izq->id == nodo->id)
        return padre->der;
    if (padre->der && padre->der->id == nodo->id)
        return padre->izq;
    return NULL;
}

// Busca el primer sucesor vivo y libre entre los hijos
static Miembro *buscar_sucesor_en_hijos(Miembro *nodo) {
    if (!nodo) return NULL;

    Miembro *encontrado = NULL;

    if (nodo->izq) {
        encontrado = primer_sucesor_vivo_libre(nodo->izq);
        if (encontrado) return encontrado;
    }
    if (nodo->der) {
        encontrado = primer_sucesor_vivo_libre(nodo->der);
        if (encontrado) return encontrado;
    }
    return NULL;
}

// Busca el primer sucesor vivo (incluyendo presos) entre los hijos
static Miembro *buscar_sucesor_en_hijos_preso(Miembro *nodo) {
    if (!nodo) return NULL;

    Miembro *encontrado = NULL;

    if (nodo->izq) {
        encontrado = primer_sucesor_vivo(nodo->izq);
        if (encontrado) return encontrado;
    }
    if (nodo->der) {
        encontrado = primer_sucesor_vivo(nodo->der);
        if (encontrado) return encontrado;
    }
    return NULL;
}

// Imprime la sucesion recursivamente (solo vivos)
static void imprimir_sucesion_recursivo(Miembro *nodo, int nivel) {
    if (!nodo) return;
    if (nodo->esta_muerto) {
        imprimir_sucesion_recursivo(nodo->izq, nivel);
        imprimir_sucesion_recursivo(nodo->der, nivel);
        return;
    }

    for (int i = 0; i < nivel; i++) {
        if (i == nivel - 1)
            cout << "|-- ";
        else
            cout << "|   ";
    }

    cout << nodo->nombre << " " << nodo->apellido
         << " (id:" << nodo->id << ", edad:" << nodo->edad << ")";
    if (nodo->es_jefe) cout << " [JEFE]";
    if (nodo->en_carcel) cout << " [PRESO]";
    if (nodo->fue_jefe) cout << " [EX-JEFE]";
    cout << endl;

    imprimir_sucesion_recursivo(nodo->izq, nivel + 1);
    imprimir_sucesion_recursivo(nodo->der, nivel + 1);
}

void mostrar_linea_sucesion(Miembro *raiz) {
    cout << endl;
    cout << "========================================" << endl;
    cout << "   LINEA DE SUCESION (solo vivos)" << endl;
    cout << "========================================" << endl;
    imprimir_sucesion_recursivo(raiz, 0);
    cout << "========================================" << endl << endl;
}

// Transfiere la jefatura de un miembro a otro
static void transferir_jefatura(Miembro *viejo_jefe, Miembro *nuevo_jefe) {
    if (viejo_jefe) {
        viejo_jefe->es_jefe = 0;
        viejo_jefe->fue_jefe = 1;
    }
    if (nuevo_jefe) {
        nuevo_jefe->es_jefe = 1;
        cout << endl << "*** " << nuevo_jefe->nombre << " " << nuevo_jefe->apellido
             << " (id:" << nuevo_jefe->id << ") es el nuevo JEFE de la familia. ***"
             << endl << endl;
    }
}

void asignar_nuevo_jefe(Miembro *raiz, Miembro *jefe_anterior) {
    Miembro *nuevo = NULL;

    // Regla 1: buscar sucesor vivo y libre en hijos del jefe
    nuevo = buscar_sucesor_en_hijos(jefe_anterior);
    if (nuevo) {
        transferir_jefatura(jefe_anterior, nuevo);
        return;
    }

    // Regla 2-4: buscar en el arbol del hermano
    Miembro *padre = encontrar_padre(raiz, jefe_anterior->id);
    if (padre) {
        Miembro *hermano = obtener_hermano(raiz, jefe_anterior);
        if (hermano) {
            // Regla 3: hermano vivo, libre y sin sucesores
            if (!hermano->esta_muerto && !hermano->en_carcel &&
                !hermano->izq && !hermano->der) {
                transferir_jefatura(jefe_anterior, hermano);
                return;
            }

            // Regla 2: sucesor vivo y libre en arbol del hermano
            nuevo = primer_sucesor_vivo_libre(hermano);
            if (nuevo) {
                transferir_jefatura(jefe_anterior, nuevo);
                return;
            }
        }

        // Regla 4-5: subir buscando ancestros con sucesores
        Miembro *ancestro = padre;
        while (ancestro) {
            Miembro *hermano_ancestro = obtener_hermano(raiz, ancestro);
            if (hermano_ancestro) {
                nuevo = primer_sucesor_vivo_libre(hermano_ancestro);
                if (nuevo) {
                    transferir_jefatura(jefe_anterior, nuevo);
                    return;
                }
            }
            Miembro *abuelo = encontrar_padre(raiz, ancestro->id);
            ancestro = abuelo;
        }
    }

    // Regla 6: buscar entre presos vivos
    nuevo = buscar_sucesor_en_hijos_preso(jefe_anterior);
    if (nuevo) {
        transferir_jefatura(jefe_anterior, nuevo);
        return;
    }

    if (padre) {
        Miembro *hermano = obtener_hermano(raiz, jefe_anterior);
        if (hermano) {
            nuevo = primer_sucesor_vivo(hermano);
            if (nuevo) {
                transferir_jefatura(jefe_anterior, nuevo);
                return;
            }
        }

        Miembro *ancestro = padre;
        while (ancestro) {
            Miembro *hermano_ancestro = obtener_hermano(raiz, ancestro);
            if (hermano_ancestro) {
                nuevo = primer_sucesor_vivo(hermano_ancestro);
                if (nuevo) {
                    transferir_jefatura(jefe_anterior, nuevo);
                    return;
                }
            }
            ancestro = encontrar_padre(raiz, ancestro->id);
        }
    }

    // Ultimo recurso: buscar en todo el arbol
    nuevo = primer_sucesor_vivo_libre(raiz);
    if (!nuevo) {
        nuevo = primer_sucesor_vivo(raiz);
    }
    if (nuevo) {
        transferir_jefatura(jefe_anterior, nuevo);
    } else {
        cout << endl << "*** No hay sucesores disponibles. La familia ha caido. ***"
             << endl << endl;
    }
}

void procesar_muerte(Miembro *raiz, int id) {
    Miembro *miembro = buscar_por_id(raiz, id);
    if (!miembro) {
        cout << "Error: No se encontro al miembro con id " << id << "." << endl;
        return;
    }
    if (miembro->esta_muerto) {
        cout << miembro->nombre << " " << miembro->apellido << " ya estaba muerto." << endl;
        return;
    }

    miembro->esta_muerto = 1;
    cout << miembro->nombre << " " << miembro->apellido
         << " (id:" << miembro->id << ") ha muerto." << endl;

    if (miembro->es_jefe) {
        cout << "El jefe ha muerto. Buscando sucesor..." << endl;
        asignar_nuevo_jefe(raiz, miembro);
    }
}

void procesar_encarcelamiento(Miembro *raiz, int id) {
    Miembro *miembro = buscar_por_id(raiz, id);
    if (!miembro) {
        cout << "Error: No se encontro al miembro con id " << id << "." << endl;
        return;
    }
    if (miembro->esta_muerto) {
        cout << miembro->nombre << " " << miembro->apellido
             << " esta muerto, no puede ir a prision." << endl;
        return;
    }
    if (miembro->en_carcel) {
        cout << miembro->nombre << " " << miembro->apellido
             << " ya esta en prision." << endl;
        return;
    }

    miembro->en_carcel = 1;
    cout << miembro->nombre << " " << miembro->apellido
         << " (id:" << miembro->id << ") ha ido a prision." << endl;

    if (miembro->es_jefe) {
        cout << "El jefe fue encarcelado. Buscando sucesor..." << endl;
        asignar_nuevo_jefe(raiz, miembro);
    }
}

void procesar_liberacion(Miembro *raiz, int id) {
    Miembro *miembro = buscar_por_id(raiz, id);
    if (!miembro) {
        cout << "Error: No se encontro al miembro con id " << id << "." << endl;
        return;
    }
    if (!miembro->en_carcel) {
        cout << miembro->nombre << " " << miembro->apellido
             << " no esta en prision." << endl;
        return;
    }

    miembro->en_carcel = 0;
    cout << miembro->nombre << " " << miembro->apellido
         << " (id:" << miembro->id << ") ha sido liberado de prision." << endl;
}

void verificar_edad_jefe(Miembro *raiz) {
    Miembro *jefe = encontrar_jefe(raiz);
    if (!jefe) return;

    if (jefe->edad > 70) {
        cout << "El jefe " << jefe->nombre << " " << jefe->apellido
             << " tiene " << jefe->edad << " anios (mayor de 70). Se debe pasar el puesto."
             << endl;
        asignar_nuevo_jefe(raiz, jefe);
    }
}
