#include <iostream>
#include <string>
#include <fstream> // Librería para trabajar con archivos
#include <vector>

using namespace std;

// --- Clase que representa un proceso ---
class Proceso {
public:
    int id;             // Identificador único del proceso
    string nombre;      // Nombre del proceso
    int prioridad;      // Prioridad del proceso

    // Constructor para inicializar un proceso
    Proceso(int id, string nombre, int prioridad) {
        this->id = id;
        this->nombre = nombre;
        this->prioridad = prioridad;
    }
};

// --- Nodo para la lista enlazada de procesos ---
struct Nodo {
    Proceso* proceso;   // Apunta al proceso guardado
    Nodo* siguiente;    // Apunta al siguiente nodo de la lista
};

// --- Lista enlazada para almacenar procesos ---
class ListaProcesos {
private:
    Nodo* cabeza;   // Inicio de la lista

public:
    // Constructor: inicia lista vacía
    ListaProcesos() {
        cabeza = nullptr;
    }

    // Inserta un proceso nuevo al inicio de la lista
    void insertar(Proceso* p) {
        if (buscar(p->id) != nullptr) {
            cout << "Error: Ya existe un proceso con ese ID.\n";
            delete p; // Evita fuga de memoria
            return;
        }
        Nodo* nuevo = new Nodo{p, cabeza};
        cabeza = nuevo;
    }

    // Elimina un proceso por su ID
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
            delete actual->proceso;
            delete actual;
            cout << "Proceso eliminado.\n";
        } else {
            cout << "Proceso no encontrado.\n";
        }
    }

    // Busca un proceso por ID y retorna el puntero
    Proceso* buscar(int id) {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (actual->proceso->id == id)
                return actual->proceso;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Modifica la prioridad de un proceso existente
    void modificarPrioridad(int id, int nuevaPrioridad) {
        Proceso* p = buscar(id);
        if (p != nullptr) {
            p->prioridad = nuevaPrioridad;
            cout << "Prioridad modificada.\n";
        } else {
            cout << "Proceso no encontrado.\n";
        }
    }

    // Muestra todos los procesos registrados
    void mostrar() {
        if (cabeza == nullptr) {
            cout << "No hay procesos registrados.\n";
            return;
        }
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            cout << "ID: " << actual->proceso->id
                 << ", Nombre: " << actual->proceso->nombre
                 << ", Prioridad: " << actual->proceso->prioridad << endl;
            actual = actual->siguiente;
        }
    }

    // Guarda la lista de procesos en un archivo de texto
    void guardar(const string& archivo) {
        ofstream ofs(archivo);
        if (!ofs) {
            cout << "Error al abrir el archivo para guardar\n";
            return;
        }
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            ofs << actual->proceso->id << " " << actual->proceso->nombre << " " << actual->proceso->prioridad << "\n";
            actual = actual->siguiente;
        }
        ofs.close();
    }

    // Carga la lista de procesos desde un archivo de texto
    void cargar(const string& archivo) {
        ifstream ifs(archivo);
        if (!ifs) return;

        // Limpiar la lista actual antes de cargar
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            delete temp->proceso;
            delete temp;
        }
        cabeza = nullptr;

        int id, prioridad;
        string nombre;
        while (ifs >> id >> nombre >> prioridad) {
            insertar(new Proceso(id, nombre, prioridad));
        }
        ifs.close();
    }
};

// --- Nodo para la cola de prioridad (Gestor de Arranque) ---
struct NodoArranque {
    Proceso* proceso;           // Apunta a un proceso existente (no toma propiedad)
    NodoArranque* siguiente;    // Siguiente en la cola
};

// --- Cola de prioridad para pasos de arranque ---
class GestorArranque {
private:
    NodoArranque* frente;   // Frente de la cola

public:
    GestorArranque() : frente(nullptr) {}

    // Encola un proceso según su prioridad (mayor primero)
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

    // Desencola el proceso con mayor prioridad
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

    // Muestra todos los procesos en la cola de arranque
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

// --- Nodo para la pila de bloques de memoria ---
struct Bloque {
    int idProceso;      // ID del proceso que tiene asignado este bloque
    int tamano;         // Tamaño del bloque (MB)
    Bloque* siguiente;  // Siguiente bloque en la pila
};

// --- Pila para la gestión de memoria ---
class PilaMemoria {
private:
    Bloque* cima;   // Tope de la pila

public:
    PilaMemoria() {
        cima = nullptr;
    }

    // Asigna memoria (push)
    void push(int id, int tamano) {
        Bloque* nuevo = new Bloque{id, tamano, cima};
        cima = nuevo;
        cout << "Memoria asignada.\n";
    }

    // Libera memoria (pop)
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

    // Muestra los bloques de memoria asignados
    void mostrar() {
        if (cima == nullptr) {
            cout << "No hay memoria asignada.\n";
            return;
        }
        Bloque* actual = cima;
        while (actual != nullptr) {
            cout << "Proceso ID: " << actual->idProceso
                 << ", Tamaño: " << actual->tamano << "MB" << endl;
            actual = actual->siguiente;
        }
    }

    // Guarda la pila de memoria en un archivo
    void guardar(const string& archivo) {
        ofstream ofs(archivo);
        if (!ofs) {
            cout << "Error al abrir archivo para guardar pila de memoria\n";
            return;
        }
        Bloque* actual = cima;
        while (actual != nullptr) {
            ofs << actual->idProceso << " " << actual->tamano << "\n";
            actual = actual->siguiente;
        }
        ofs.close();
    }

    // Carga la pila de memoria desde un archivo
    void cargar(const string& archivo) {
        ifstream ifs(archivo);
        if (!ifs) return;

        // Elimina la pila actual antes de cargar
        while (cima != nullptr) {
            Bloque* temp = cima;
            cima = cima->siguiente;
            delete temp;
        }
        cima = nullptr;

        // Para cargar en el mismo orden que el archivo
        vector<pair<int,int>> bloques;
        int id, tamano;
        while (ifs >> id >> tamano) {
            bloques.push_back({id, tamano});
        }
        // Insertar en orden inverso para que la cima sea el último leído
        for (auto it = bloques.rbegin(); it != bloques.rend(); ++it) {
            push(it->first, it->second);
        }
        ifs.close();
    }
};

// --- MAIN: Menú principal del sistema ---
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
            case 5: {
                int id;
                cout << "ID del proceso a encolar: "; cin >> id;
                Proceso* p = lista.buscar(id);
                if (p) arranque.encolar(p);
                else cout << "Proceso no encontrado.\n";
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
                // Guardar datos antes de salir
                lista.guardar("procesos.txt");
                pila.guardar("memoria.txt");
                break;
            default:
                cout << "Opción no válida.\n";
        }

    } while (opcion != 0);

    return 0;
}
