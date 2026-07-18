#include "csv_parser.h"

struct NodoPendiente {
    Miembro *miembro;
    NodoPendiente *sig;
};

// Escribe un nodo y sus hijos al archivo CSV
static void escribir_nodo_csv(ofstream &f, Miembro *nodo) {
    if (!nodo) return;

    f << nodo->id << "," << nodo->nombre << "," << nodo->apellido << ","
      << nodo->genero << "," << nodo->edad << "," << nodo->id_jefe << ","
      << nodo->esta_muerto << "," << nodo->en_carcel << ","
      << nodo->fue_jefe << "," << nodo->es_jefe << endl;

    escribir_nodo_csv(f, nodo->izq);
    escribir_nodo_csv(f, nodo->der);
}

// Intenta insertar nodos pendientes en el arbol
static void intentar_insertar_pendientes(Miembro *raiz, NodoPendiente *&pendientes) {
    bool insertado_alguno = true;
    while (insertado_alguno) {
        insertado_alguno = false;
        NodoPendiente *actual = pendientes;
        NodoPendiente *anterior = NULL;

        while (actual) {
            if (insertar_miembro(raiz, actual->miembro)) {
                insertado_alguno = true;
                NodoPendiente *eliminar = actual;
                if (anterior) {
                    anterior->sig = actual->sig;
                } else {
                    pendientes = actual->sig;
                }
                actual = actual->sig;
                delete eliminar;
            } else {
                anterior = actual;
                actual = actual->sig;
            }
        }
    }
}

// Busca si un ID ya esta registrado en la lista de pendientes
static bool buscar_en_pendientes(NodoPendiente *pendientes, int id) {
    NodoPendiente *actual = pendientes;
    while (actual) {
        if (actual->miembro && actual->miembro->id == id) {
            return true;
        }
        actual = actual->sig;
    }
    return false;
}

Miembro *cargar_csv(const char *ruta) {
    ifstream f(ruta);
    if (!f.is_open()) {
        cout << "Error: No se pudo abrir el archivo '" << ruta << "'." << endl;
        return NULL;
    }

    string linea;
    Miembro *raiz = NULL;
    NodoPendiente *pendientes = NULL;
    bool es_cabecera = true;

    while (getline(f, linea)) {
        // Eliminar retorno de carro
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }
        if (linea.empty()) continue;

        if (es_cabecera) {
            es_cabecera = false;
            continue;
        }

        // Parsear campos con stringstream
        stringstream ss(linea);
        string token;

        int id, edad, id_jefe, esta_muerto, en_carcel, fue_jefe, es_jefe;
        char nombre[50], apellido[50];
        char genero;

        if (!getline(ss, token, ',')) continue;
        id = atoi(token.c_str());

        if (!getline(ss, token, ',')) continue;
        strncpy(nombre, token.c_str(), 49);
        nombre[49] = '\0';

        if (!getline(ss, token, ',')) continue;
        strncpy(apellido, token.c_str(), 49);
        apellido[49] = '\0';

        if (!getline(ss, token, ',')) continue;
        genero = token[0];

        if (!getline(ss, token, ',')) continue;
        edad = atoi(token.c_str());

        if (!getline(ss, token, ',')) continue;
        id_jefe = atoi(token.c_str());

        if (!getline(ss, token, ',')) continue;
        esta_muerto = atoi(token.c_str());

        if (!getline(ss, token, ',')) continue;
        en_carcel = atoi(token.c_str());

        if (!getline(ss, token, ',')) continue;
        fue_jefe = atoi(token.c_str());

        if (!getline(ss, token, ',')) continue;
        es_jefe = atoi(token.c_str());

        // Validar si el ID ya existe en el arbol o pendientes
        if ((raiz && buscar_por_id(raiz, id)) || buscar_en_pendientes(pendientes, id)) {
            cout << "Error: ID duplicado detectado en CSV: " << id 
                 << " (" << nombre << " " << apellido << "). Ignorando registro." << endl;
            continue;
        }

        Miembro *nuevo = crear_miembro(id, nombre, apellido, genero,
                                       edad, id_jefe, esta_muerto, en_carcel,
                                       fue_jefe, es_jefe);
        if (!nuevo) continue;

        if (id_jefe == 0) {
            if (raiz != NULL) {
                cout << "Error: Se detecto un segundo jefe de familia con ID " << id 
                     << " (" << nombre << " " << apellido << "). Solo se permite un jefe supremo (id_jefe = 0)." << endl;
                delete nuevo;
                continue;
            }
            raiz = nuevo;
            intentar_insertar_pendientes(raiz, pendientes);
        } else {
            if (raiz && insertar_miembro(raiz, nuevo)) {
                // Insertado correctamente
            } else {
                NodoPendiente *pend = new NodoPendiente();
                pend->miembro = nuevo;
                pend->sig = pendientes;
                pendientes = pend;
            }
        }
    }

    f.close();

    // Insertar pendientes restantes
    if (raiz && pendientes) {
        intentar_insertar_pendientes(raiz, pendientes);
    }

    // Liberar pendientes que no se pudieron insertar
    if (pendientes) {
        if (!raiz) {
            cout << "Error critico: No se encontro un jefe de familia principal (con id_jefe = 0)." << endl;
        } else {
            cout << "Error de Consistencia CSV: Se detectaron miembros del arbol que no pudieron insertarse." << endl;
            cout << "Esto se debe a que su jefe (id_jefe) no existe o forma una relacion circular (bucle):" << endl;
        }
        while (pendientes) {
            NodoPendiente *temp = pendientes;
            cout << "  - " << temp->miembro->nombre << " " << temp->miembro->apellido
                 << " (ID: " << temp->miembro->id << ", ID Jefe: " << temp->miembro->id_jefe << ")" << endl;
            delete temp->miembro;
            pendientes = temp->sig;
            delete temp;
        }
    }

    return raiz;
}

void guardar_csv(const char *ruta, Miembro *raiz) {
    ofstream f(ruta);
    if (!f.is_open()) {
        cout << "Error: No se pudo abrir el archivo '" << ruta << "' para escritura." << endl;
        return;
    }

    f << "id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss" << endl;
    escribir_nodo_csv(f, raiz);
    f.close();
    cout << "Datos guardados exitosamente en '" << ruta << "'." << endl;
}
