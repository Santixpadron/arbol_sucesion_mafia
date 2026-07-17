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

        Miembro *nuevo = crear_miembro(id, nombre, apellido, genero,
                                       edad, id_jefe, esta_muerto, en_carcel,
                                       fue_jefe, es_jefe);
        if (!nuevo) continue;

        if (id_jefe == 0) {
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
    while (pendientes) {
        NodoPendiente *temp = pendientes;
        cout << "Advertencia: No se pudo insertar a " << temp->miembro->nombre
             << " " << temp->miembro->apellido
             << " (id:" << temp->miembro->id
             << ", id_jefe:" << temp->miembro->id_jefe << ")" << endl;
        delete temp->miembro;
        pendientes = temp->sig;
        delete temp;
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
