#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits> // Para numeric_limits
//#include <memory> // No incluir memory para evitar unique_ptr

using namespace std;

// --- Clase Proceso ---
class Proceso {
public:
    int id;
    string nombre;
    int prioridad;

    Proceso(int id, string nombre, int prioridad) : id(id), nombre(nombre), prioridad(prioridad) {}

    ~Proceso() {
        // No es necesario liberar memoria aquí si no se asigna memoria dinámica dentro de la clase
    }
};

// --- Lista enlazada para procesos ---
struct Nodo {
    Proceso* proceso; // Usar puntero crudo
    Nodo* siguiente;
};

class ListaProcesos {
private:
    Nodo* cabeza;

public:
    ListaProcesos() : cabeza(NULL) {}

    ~ListaProcesos() {
        Nodo* actual = cabeza;
        while (actual != NULL) {
            Nodo* siguiente = actual->siguiente;
            delete actual->proceso; // Liberar la memoria del proceso
            delete actual;
            actual = siguiente;
        }
    }

    void insertar(Proceso* p) {
        if (buscar(p->id) != NULL) {
            cout << "Error: Ya existe un proceso con ese ID.\n";
            delete p; // Liberar la memoria si no se inserta
            return;
        }
        Nodo* nuevo = new Nodo;
        nuevo->proceso = p;
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    void eliminar(int id) {
        Nodo *actual = cabeza, *anterior = NULL;
        while (actual != NULL && actual->proceso->id != id) {
            anterior = actual;
            actual = actual->siguiente;
        }
        if (actual != NULL) {
            if (anterior == NULL)
                cabeza = actual->siguiente;
            else
                anterior->siguiente = actual->siguiente;

            delete actual->proceso;
            delete actual;
            cout << "Proceso eliminado.\n";
        } else {
            cout << "Proceso no encontrado.\n";
        }
    }

    Proceso* buscar(int id) {
        Nodo* actual = cabeza;
        while (actual != NULL) {
            if (actual->proceso->id == id)
                return actual->proceso;
            actual = actual->siguiente;
        }
        return NULL;
    }

    void modificarPrioridad(int id, int nuevaPrioridad) {
        Proceso* p = buscar(id);
        if (p != NULL) {
            p->prioridad = nuevaPrioridad;
            cout << "Prioridad modificada.\n";
        } else {
            cout << "Proceso no encontrado.\n";
        }
    }

    void mostrar() {
        if (cabeza == NULL) {
            cout << "No hay procesos registrados.\n";
            return;
        }
        Nodo* actual = cabeza;
        while (actual != NULL) {
            cout << "ID: " << actual->proceso->id << ", Nombre: " << actual->proceso->nombre
                 << ", Prioridad: " << actual->proceso->prioridad << endl;
            actual = actual->siguiente;
        }
    }

    void guardar(const string& archivo) {
        ofstream ofs(archivo.c_str()); // Convertir string a const char*
        if (!ofs) {
            cout << "Error al abrir el archivo para guardar\n";
            return;
        }
        Nodo* actual = cabeza;
        while (actual != NULL) {
            ofs << actual->proceso->id << " " << actual->proceso->nombre << " " << actual->proceso->prioridad << "\n";
            actual = actual->siguiente;
        }
        ofs.close();
    }

    void cargar(const string& archivo) {
        ifstream ifs(archivo.c_str()); // Convertir string a const char*
        if (!ifs) {
            cout << "No se pudo abrir el archivo para cargar procesos.\n";
            return;
        }

        // Limpiar la lista actual antes de cargar nuevos procesos
        Nodo* actual = cabeza;
        while (actual != NULL) {
            Nodo* siguiente = actual->siguiente;
            delete actual->proceso;
            delete actual;
            actual = siguiente;
        }
        cabeza = NULL;

        int id, prioridad;
        string nombre;

        while (ifs >> id) {
            ifs.ignore(numeric_limits<streamsize>::max(), ' '); // Ignorar hasta el primer espacio
            getline(ifs, nombre, ' '); // Leer el nombre hasta el siguiente espacio
            ifs >> prioridad;
            ifs.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la línea

            if (ifs.fail()) {
                cout << "Error al leer el archivo de procesos.\n";
                break;
            }

            insertar(new Proceso(id, nombre, prioridad));
        }

        ifs.close();
    }
};

// --- Cola de prioridad para GESTOR DE ARRANQUE ---
struct NodoArranque {
    Proceso* proceso;
    NodoArranque* siguiente;
};

class GestorArranque {
private:
    NodoArranque* frente;

public:
    GestorArranque() : frente(NULL) {}

    ~GestorArranque() {
        while (frente != NULL) {
            NodoArranque* temp = frente;
            frente = frente->siguiente;
            delete temp;
        }
    }

    void encolar(Proceso* p) {
        NodoArranque* nuevo = new NodoArranque;
        nuevo->proceso = p;
        nuevo->siguiente = NULL;

        if (frente == NULL || p->prioridad > frente->proceso->prioridad) {
            nuevo->siguiente = frente;
            frente = nuevo;
        } else {
            NodoArranque* actual = frente;
            while (actual->siguiente != NULL &&
                   actual->siguiente->proceso->prioridad >= p->prioridad) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
        cout << "Paso de arranque encolado.\n";
    }

    void desencolar() {
        if (frente == NULL) {
            cout << "La cola de arranque está vacía.\n";
            return;
        }
        cout << "Iniciando paso de arranque: " << frente->proceso->nombre << endl;
        NodoArranque* temp = frente;
        frente = frente->siguiente;
        delete temp;
    }

    void mostrar() {
        if (frente == NULL) {
            cout << "La cola de arranque está vacía.\n";
            return;
        }
        NodoArranque* actual = frente;
        while (actual != NULL) {
            cout << "ID: " << actual->proceso->id
                 << ", Nombre: " << actual->proceso->nombre
                 << ", Prioridad: " << actual->proceso->prioridad << endl;
            actual = actual->siguiente;
        }
    }
};

// --- Pila para gestión de memoria ---
struct Bloque {
    int idProceso;
    int tamano;
    Bloque* siguiente;
};

class PilaMemoria {
private:
    Bloque* cima;

public:
    PilaMemoria() : cima(NULL) {}

    ~PilaMemoria() {
        while (cima != NULL) {
            Bloque* temp = cima;
            cima = cima->siguiente;
            delete temp;
        }
    }

    void push(int id, int tamano) {
        Bloque* nuevo = new Bloque;
        nuevo->idProceso = id;
        nuevo->tamano = tamano;
        nuevo->siguiente = cima;
        cima = nuevo;
        cout << "Memoria asignada.\n";
    }

    void pop() {
        if (cima == NULL) {
            cout << "No hay memoria que liberar.\n";
            return;
        }
        cout << "Liberando memoria del proceso: " << cima->idProceso << endl;
        Bloque* temp = cima;
        cima = cima->siguiente;
        delete temp;
    }

    void mostrar() {
        if (cima == NULL) {
            cout << "No hay memoria asignada.\n";
            return;
        }
        Bloque* actual = cima;
        while (actual != NULL) {
            cout << "Proceso ID: " << actual->idProceso << ", Tamaño: " << actual->tamano << "MB" << endl;
            actual = actual->siguiente;
        }
    }

    void guardar(const string& archivo) {
        ofstream ofs(archivo.c_str()); // Convertir string a const char*
        if (!ofs) {
            cout << "Error al abrir archivo para guardar pila de memoria\n";
            return;
        }
        Bloque* actual = cima;
        while (actual != NULL) {
            ofs << actual->idProceso << " " << actual->tamano << "\n";
            actual = actual->siguiente;
        }
        ofs.close();
    }

    void cargar(const string& archivo) {
        ifstream ifs(archivo.c_str()); // Convertir string a const char*
        if (!ifs) {
            cout << "No se pudo abrir el archivo para cargar la pila de memoria.\n";
            return;
        }

        // Limpiar la pila actual antes de cargar nuevos bloques
        while (cima != NULL) {
            Bloque* temp = cima;
            cima = cima->siguiente;
            delete temp;
        }

        int id, tamano;
        while (ifs >> id >> tamano) {
            if (ifs.fail()) {
                cout << "Error al leer el archivo de memoria.\n";
                break;
            }
            push(id, tamano);
        }

        ifs.close();
    }
};

// --- MAIN ---
int main() {
    ListaProcesos lista;
    GestorArranque arranque;
    PilaMemoria pila;
    int opcion;

    // Cargar datos guardados al iniciar el programa
    lista.cargar("procesos.txt");
    pila.cargar("memoria.txt");

    do {
        cout << "\n===== SISTEMA DE GESTIÓN DE PROCESOS =====\n";
        cout << "1. Registrar Proceso\n";
        cout << "2. Eliminar Proceso\n";
        cout << "3. Mostrar Lista de Procesos\n";
        cout << "4. Modificar Prioridad\n";
        cout << "5. Encolar Paso de Arranque\n";
        cout << "6. Ejecutar Paso de Arranque\n";
        cout << "7. Mostrar Cola de Arranque\n";
        cout << "8. Asignar Memoria a Proceso\n";
        cout << "9. Liberar Memoria\n";
        cout << "10. Mostrar Memoria\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if (cin.fail()) {
            cout << "Entrada inválida. Por favor, ingrese un número.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1;
        }

        switch (opcion) {
            case 1: {
                int id, prioridad;
                string nombre;

                cout << "ID: ";
                cin >> id;
                if (cin.fail()) {
                    cout << "Error: ID debe ser un número entero.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                cout << "Nombre: ";
                cin.ignore(); // Consume newline
                getline(cin, nombre);

                cout << "Prioridad (mayor número = mayor prioridad): ";
                cin >> prioridad;
                if (cin.fail()) {
                    cout << "Error: Prioridad debe ser un número entero.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                Proceso* nuevoProceso = new Proceso(id, nombre, prioridad);
                lista.insertar(nuevoProceso);
                break;
            }
            case 2: {
                int id;
                cout << "ID a eliminar: ";
                cin >> id;
                if (cin.fail()) {
                    cout << "Error: ID debe ser un número entero.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                lista.eliminar(id);
                break;
            }
            case 3:
                lista.mostrar();
                break;
            case 4: {
                int id, prio;
                cout << "ID del proceso: ";
                cin >> id;
                if (cin.fail()) {
                    cout << "Error: ID debe ser un número entero.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                cout << "Nueva prioridad: ";
                cin >> prio;
                if (cin.fail()) {
                    cout << "Error: Prioridad debe ser un número entero.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                lista.modificarPrioridad(id, prio);
                break;
            }
            case 5: {
                int id;
                cout << "ID del proceso a encolar: ";
                cin >> id;
                if (cin.fail()) {
                    cout << "Error: ID debe ser un número entero.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                Proceso* p = lista.buscar(id);
                if (p)
                    arranque.encolar(p);
                else
                    cout << "Proceso no encontrado.\n";
                break;
            }
            case 6:
                arranque.desencolar();
                break;
            case 7:
                arranque.mostrar();
                break;
            case 8: {
                int id, tamano;
                cout << "ID Proceso: ";
                cin >> id;
                if (cin.fail()) {
                    cout << "Error: ID debe ser un número entero.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                cout << "Tamaño memoria (MB): ";
                cin >> tamano;
                if (cin.fail()) {
                    cout << "Error: El tamaño debe ser un número entero.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                pila.push(id, tamano);
                break;
            }
            case 9:
                pila.pop();
                break;
            case 10:
                pila.mostrar();
                break;
            case 0:
                cout << "Saliendo...\n";
                lista.guardar("procesos.txt");
                pila.guardar("memoria.txt");
                break;
            default:
                cout << "Opción no válida.\n";
        }

    } while (opcion != 0);

    return 0;
}
