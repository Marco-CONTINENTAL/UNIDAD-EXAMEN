#include <iostream>
#include <string>
using namespace std;

// --- Clase Proceso ---
class Proceso {
public:
    int id;
    string nombre;
    int prioridad;

    Proceso(int id, string nombre, int prioridad) {
        this->id = id;
        this->nombre = nombre;
        this->prioridad = prioridad;
    }
};

// --- Lista enlazada para procesos ---
struct Nodo {
    Proceso* proceso;
    Nodo* siguiente;
};

class ListaProcesos {
private:
    Nodo* cabeza;

public:
    ListaProcesos() {
        cabeza = nullptr;
    }

    void insertar(Proceso* p) {
        Nodo* nuevo = new Nodo{p, cabeza};
        cabeza = nuevo;
    }

    void eliminar(int id) {
        Nodo *actual = cabeza, *anterior = nullptr;
        while (actual != nullptr && actual->proceso->id != id) {
            anterior = actual;
            actual = actual->siguiente;
        }
        if (actual != nullptr) {
            if (anterior == nullptr)
                cabeza = actual->siguiente;
            else
                anterior->siguiente = actual->siguiente;
            delete actual;
            cout << "Proceso eliminado.\n";
        } else {
            cout << "Proceso no encontrado.\n";
        }
    }

    Proceso* buscar(int id) {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (actual->proceso->id == id)
                return actual->proceso;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void modificarPrioridad(int id, int nuevaPrioridad) {
        Proceso* p = buscar(id);
        if (p != nullptr) {
            p->prioridad = nuevaPrioridad;
            cout << "Prioridad modificada.\n";
        } else {
            cout << "Proceso no encontrado.\n";
        }
    }

    void mostrar() {
        if (cabeza == nullptr) {
            cout << "No hay procesos registrados.\n";
            return;
        }
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            cout << "ID: " << actual->proceso->id << ", Nombre: " << actual->proceso->nombre
                 << ", Prioridad: " << actual->proceso->prioridad << endl;
            actual = actual->siguiente;
        }
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
    GestorArranque() : frente(nullptr) {}

    // encola por prioridad (mayor número, mayor prioridad)
    void encolar(Proceso* p) {
        NodoArranque* nuevo = new NodoArranque{p, nullptr};
        if (frente == nullptr || p->prioridad > frente->proceso->prioridad) {
            nuevo->siguiente = frente;
            frente = nuevo;
        } else {
            NodoArranque* actual = frente;
            while (actual->siguiente != nullptr &&
                   actual->siguiente->proceso->prioridad >= p->prioridad) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
        cout << "Paso de arranque encolado.\n";
    }

    void desencolar() {
        if (frente == nullptr) {
            cout << "La cola de arranque está vacía.\n";
            return;
        }
        cout << "Iniciando paso de arranque: " << frente->proceso->nombre << endl;
        NodoArranque* temp = frente;
        frente = frente->siguiente;
        delete temp;
    }

    void mostrar() {
        if (frente == nullptr) {
            cout << "La cola de arranque está vacía.\n";
            return;
        }
        NodoArranque* actual = frente;
        while (actual != nullptr) {
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
    PilaMemoria() {
        cima = nullptr;
    }

    void push(int id, int tamano) {
        Bloque* nuevo = new Bloque{id, tamano, cima};
        cima = nuevo;
        cout << "Memoria asignada.\n";
    }

    void pop() {
        if (cima == nullptr) {
            cout << "No hay memoria que liberar.\n";
            return;
        }
        cout << "Liberando memoria del proceso: " << cima->idProceso << endl;
        Bloque* temp = cima;
        cima = cima->siguiente;
        delete temp;
    }

    void mostrar() {
        if (cima == nullptr) {
            cout << "No hay memoria asignada.\n";
            return;
        }
        Bloque* actual = cima;
        while (actual != nullptr) {
            cout << "Proceso ID: " << actual->idProceso << ", Tamaño: " << actual->tamano << "MB" << endl;
            actual = actual->siguiente;
        }
    }
};

// --- MAIN ---
int main() {
    ListaProcesos lista;
    GestorArranque arranque;   
    PilaMemoria pila;
    int opcion;

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

        switch(opcion) {
            case 1: {
                int id, prioridad;
                string nombre;
                cout << "ID: "; cin >> id;
                cout << "Nombre: "; cin >> nombre;
                cout << "Prioridad (mayor número = mayor prioridad): "; cin >> prioridad;
                lista.insertar(new Proceso(id, nombre, prioridad));
                break;
            }
            case 2: {
                int id;
                cout << "ID a eliminar: "; cin >> id;
                lista.eliminar(id);
                break;
            }
            case 3:
                lista.mostrar();
                break;
            case 4: {
                int id, prio;
                cout << "ID del proceso: "; cin >> id;
                cout << "Nueva prioridad: "; cin >> prio;
                lista.modificarPrioridad(id, prio);
                break;
            }
            case 5: {                       // Encolar
               int id;
               cout << "ID del proceso a encolar: "; cin >> id;
               Proceso* p = lista.buscar(id);
               if (p) arranque.encolar(p);          // ← usa arranque
              else cout << "Proceso no encontrado.\n";
              break;
          }
           case 6:                         // Ejecutar
              arranque.desencolar();
              break;

          case 7:                         // Mostrar cola
             arranque.mostrar();
            break;

            case 8: {
                int id, tamano;
                cout << "ID Proceso: "; cin >> id;
                cout << "Tamaño memoria (MB): "; cin >> tamano;
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
                break;
            default:
                cout << "Opción no válida.\n";
        }

    } while (opcion != 0);

    return 0;
}

