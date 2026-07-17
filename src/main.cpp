#include <iostream>
#include <cstring>
#include "arbol.h"
#include "csv_parser.h"
#include "sucesion.h"

using namespace std;

#define RUTA_CSV "bin/familia.csv"

static void modificar_miembro(Miembro *raiz) {
    int id;
    cout << "Ingrese el ID del miembro a modificar: ";
    cin >> id;
    cin.ignore();

    Miembro *m = buscar_por_id(raiz, id);
    if (!m) {
        cout << "Error: No se encontro al miembro con id " << id << "." << endl;
        return;
    }

    cout << endl << "Datos actuales de " << m->nombre << " " << m->apellido
         << " (id:" << m->id << "):" << endl;
    cout << "  1. Nombre: " << m->nombre << endl;
    cout << "  2. Apellido: " << m->apellido << endl;
    cout << "  3. Genero: " << m->genero << endl;
    cout << "  4. Edad: " << m->edad << endl;
    cout << "  5. Esta muerto: " << m->esta_muerto << endl;
    cout << "  6. En carcel: " << m->en_carcel << endl;
    cout << "  7. Fue jefe: " << m->fue_jefe << endl;
    cout << "  8. Es jefe: " << m->es_jefe << endl;
    cout << "  0. Cancelar" << endl;
    cout << endl << "Nota: No se puede modificar el ID (" << m->id
         << ") ni el ID del jefe (" << m->id_jefe << ")." << endl;

    int opcion;
    cout << endl << "Que campo desea modificar? ";
    cin >> opcion;
    cin.ignore();

    switch (opcion) {
        case 1: {
            char nombre[50];
            cout << "Nuevo nombre: ";
            cin >> nombre;
            cin.ignore();
            strncpy(m->nombre, nombre, 49);
            m->nombre[49] = '\0';
            cout << "Nombre actualizado a '" << m->nombre << "'." << endl;
            break;
        }
        case 2: {
            char apellido[50];
            cout << "Nuevo apellido: ";
            cin >> apellido;
            cin.ignore();
            strncpy(m->apellido, apellido, 49);
            m->apellido[49] = '\0';
            cout << "Apellido actualizado a '" << m->apellido << "'." << endl;
            break;
        }
        case 3: {
            char genero;
            cout << "Nuevo genero (H/M): ";
            cin >> genero;
            cin.ignore();
            if (genero == 'H' || genero == 'M' || genero == 'h' || genero == 'm') {
                m->genero = (genero == 'h') ? 'H' : (genero == 'm') ? 'M' : genero;
                cout << "Genero actualizado a '" << m->genero << "'." << endl;
            } else {
                cout << "Genero invalido. Debe ser H o M." << endl;
            }
            break;
        }
        case 4: {
            int edad;
            cout << "Nueva edad: ";
            cin >> edad;
            cin.ignore();
            if (edad > 0 && edad < 150) {
                m->edad = edad;
                cout << "Edad actualizada a " << m->edad << "." << endl;
                if (m->es_jefe && m->edad > 70) {
                    verificar_edad_jefe(raiz);
                }
            } else {
                cout << "Edad invalida." << endl;
            }
            break;
        }
        case 5: {
            int val;
            cout << "Esta muerto? (0=No, 1=Si): ";
            cin >> val;
            cin.ignore();
            if (val == 0 || val == 1) {
                if (val == 1 && !m->esta_muerto) {
                    procesar_muerte(raiz, m->id);
                } else {
                    m->esta_muerto = val;
                    cout << "Estado actualizado." << endl;
                }
            }
            break;
        }
        case 6: {
            int val;
            cout << "En carcel? (0=No, 1=Si): ";
            cin >> val;
            cin.ignore();
            if (val == 1 && !m->en_carcel) {
                procesar_encarcelamiento(raiz, m->id);
            } else if (val == 0 && m->en_carcel) {
                procesar_liberacion(raiz, m->id);
            }
            break;
        }
        case 7: {
            int val;
            cout << "Fue jefe? (0=No, 1=Si): ";
            cin >> val;
            cin.ignore();
            if (val == 0 || val == 1) {
                m->fue_jefe = val;
                cout << "Estado actualizado." << endl;
            }
            break;
        }
        case 8: {
            int val;
            cout << "Es jefe? (0=No, 1=Si): ";
            cin >> val;
            cin.ignore();
            if (val == 0 || val == 1) {
                m->es_jefe = val;
                cout << "Estado actualizado." << endl;
            }
            break;
        }
        case 0:
            cout << "Operacion cancelada." << endl;
            break;
        default:
            cout << "Opcion invalida." << endl;
    }
}

static void buscar_miembro_menu(Miembro *raiz) {
    int id;
    cout << "Ingrese el ID del miembro a buscar: ";
    cin >> id;
    cin.ignore();

    Miembro *m = buscar_por_id(raiz, id);
    if (!m) {
        cout << "No se encontro al miembro con id " << id << "." << endl;
        return;
    }

    cout << endl << "--- Datos del miembro ---" << endl;
    cout << "  ID: " << m->id << endl;
    cout << "  Nombre: " << m->nombre << " " << m->apellido << endl;
    cout << "  Genero: " << m->genero << endl;
    cout << "  Edad: " << m->edad << endl;
    cout << "  ID Jefe: " << m->id_jefe << endl;
    cout << "  Esta muerto: " << (m->esta_muerto ? "Si" : "No") << endl;
    cout << "  En carcel: " << (m->en_carcel ? "Si" : "No") << endl;
    cout << "  Fue jefe: " << (m->fue_jefe ? "Si" : "No") << endl;
    cout << "  Es jefe: " << (m->es_jefe ? "Si" : "No") << endl;
    cout << "-------------------------" << endl;
}

int main() {
    cout << "============================================" << endl;
    cout << "  ARBOL DE SUCESION - FAMILIA MAFIOSA" << endl;
    cout << "============================================" << endl << endl;

    cout << "Cargando datos desde '" << RUTA_CSV << "'..." << endl;
    Miembro *raiz = cargar_csv(RUTA_CSV);

    if (!raiz) {
        cout << "Error fatal: No se pudo cargar el arbol." << endl;
        return 1;
    }

    cout << "Arbol cargado exitosamente. " << contar_vivos(raiz) << " miembros vivos." << endl;

    // Verificar edad del jefe al inicio
    verificar_edad_jefe(raiz);

    int opcion;
    do {
        cout << endl;
        cout << "============================================" << endl;
        cout << "              MENU PRINCIPAL" << endl;
        cout << "============================================" << endl;
        cout << "  1. Mostrar linea de sucesion (vivos)" << endl;
        cout << "  2. Mostrar arbol completo" << endl;
        cout << "  3. Registrar muerte de un miembro" << endl;
        cout << "  4. Enviar a prision" << endl;
        cout << "  5. Liberar de prision" << endl;
        cout << "  6. Modificar datos de un miembro" << endl;
        cout << "  7. Buscar miembro por ID" << endl;
        cout << "  8. Guardar datos al CSV" << endl;
        cout << "  0. Salir" << endl;
        cout << "============================================" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                mostrar_linea_sucesion(raiz);
                break;
            case 2:
                cout << endl;
                cout << "========================================" << endl;
                cout << "        ARBOL COMPLETO" << endl;
                cout << "========================================" << endl;
                imprimir_arbol(raiz, 0);
                cout << "========================================" << endl;
                break;
            case 3: {
                int id;
                cout << "Ingrese el ID del miembro que murio: ";
                cin >> id;
                cin.ignore();
                procesar_muerte(raiz, id);
                break;
            }
            case 4: {
                int id;
                cout << "Ingrese el ID del miembro a encarcelar: ";
                cin >> id;
                cin.ignore();
                procesar_encarcelamiento(raiz, id);
                break;
            }
            case 5: {
                int id;
                cout << "Ingrese el ID del miembro a liberar: ";
                cin >> id;
                cin.ignore();
                procesar_liberacion(raiz, id);
                break;
            }
            case 6:
                modificar_miembro(raiz);
                break;
            case 7:
                buscar_miembro_menu(raiz);
                break;
            case 8:
                guardar_csv(RUTA_CSV, raiz);
                break;
            case 0:
                cout << endl << "Guardando datos..." << endl;
                guardar_csv(RUTA_CSV, raiz);
                cout << "Arrivederci!" << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);

    liberar_arbol(raiz);
    return 0;
}
