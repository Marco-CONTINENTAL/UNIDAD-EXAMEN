#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <list>
using namespace std;

// Estructura para representar un proceso
struct Proceso {
    string id;
    int prioridad;
    int memoria;
};

// Lista de procesos
list<Proceso> listaProcesos;

// Cola para paso de arranque
queue<Proceso> colaArranque;

// Pila para gestión de memoria
stack<Proceso> pilaMemoria;

// Funciones para Lista de Procesos
void registrarProceso() {
    Proceso p;
    cout << "Ingrese ID del proceso: ";
    cin >> p.id;

    for (const auto& proc : listaProcesos) {
        if (proc.id == p.id) {
            cout << "Ya existe un proceso con ese ID.\n";
            return;
        }
    }

    cout << "Ingrese prioridad: ";
    cin >> p.prioridad;
    p.memoria = 0;
    listaProcesos.push_back(p);
    cout << "Proceso registrado con éxito.\n";
}

void eliminarProceso() {
    string id;
    cout << "Ingrese ID del proceso a eliminar: ";
    cin >> id;
    for (auto it = listaProcesos.begin(); it != listaProcesos.end(); ++it) {
        if (it->id == id) {
            listaProcesos.erase(it);
            cout << "Proceso eliminado.\n";
            return;
        }
    }
    cout << "Proceso no encontrado.\n";
}

void mostrarProcesos() {
    cout << "Lista de procesos:\n";
    for (const auto& p : listaProcesos) {
        cout << "ID: " << p.id << ", Prioridad: " << p.prioridad << ", Memoria: " << p.memoria << "\n";
    }
}

void modificarPrioridad() {
    string id;
    int nuevaPrioridad;
    cout << "Ingrese ID del proceso: ";
    cin >> id;
    cout << "Ingrese nueva prioridad: ";
    cin >> nuevaPrioridad;
    for (auto& p : listaProcesos) {
        if (p.id == id) {
            p.prioridad = nuevaPrioridad;
            cout << "Prioridad modificada.\n";
            return;
        }
    }
    cout << "Proceso no encontrado.\n";
}

// Funciones para Cola de Arranque
void encolarArranque() {
    string id;
    cout << "Ingrese ID del proceso a encolar: ";
    cin >> id;

    queue<Proceso> temp = colaArranque;
    while (!temp.empty()) {
        if (temp.front().id == id) {
            cout << "El proceso ya está en la cola.\n";
            return;
        }
        temp.pop();
    }

    for (const auto& p : listaProcesos) {
        if (p.id == id) {
            colaArranque.push(p);
            cout << "Proceso encolado.\n";
            return;
        }
    }
    cout << "Proceso no encontrado.\n";
}


void ejecutarArranque() {
    if (!colaArranque.empty()) {
        Proceso p = colaArranque.front();
        colaArranque.pop();
        cout << "Ejecutando proceso ID: " << p.id << "\n";
    } else {
        cout << "La cola está vacía.\n";
    }
}

void mostrarColaArranque() {
    queue<Proceso> temp = colaArranque;
    cout << "Cola de arranque:\n";
    while (!temp.empty()) {
        Proceso p = temp.front();
        temp.pop();
        cout << "ID: " << p.id << ", Prioridad: " << p.prioridad << "\n";
    }
}

// Funciones para Gestión de Memoria (Pila)
void asignarMemoria() {
    string id;
    cout << "Ingrese ID del proceso: ";
    cin >> id;
    for (auto& p : listaProcesos) {
        if (p.id == id) {
            cout << "Ingrese cantidad de memoria a asignar: ";
            cin >> p.memoria;
            pilaMemoria.push(p);
            cout << "Memoria asignada.\n";
            return;
        }
    }
    cout << "Proceso no encontrado.\n";
}

void liberarMemoria() {
    if (!pilaMemoria.empty()) {
        Proceso p = pilaMemoria.top();
        pilaMemoria.pop();
        cout << "Memoria liberada del proceso ID: " << p.id << "\n";
    } else {
        cout << "La pila de memoria está vacía.\n";
    }
}

void mostrarMemoria() {
    stack<Proceso> temp = pilaMemoria;
    cout << "Procesos en pila de memoria:\n";
    while (!temp.empty()) {
        Proceso p = temp.top();
        temp.pop();
        cout << "ID: " << p.id << ", Memoria: " << p.memoria << "\n";
    }
}

// Menú principal con secciones mejoradas
void mostrarMenu() {
    cout << "==============================================\n";
    cout << "         SISTEMA DE GESTIÓN DE PROCESOS       \n";
    cout << "==============================================\n";

    cout << "\n>> GESTIÓN DE LISTA DE PROCESOS\n";
    cout << " 1. Registrar Proceso\n";
    cout << " 2. Eliminar Proceso\n";
    cout << " 3. Mostrar Lista de Procesos\n";
    cout << " 4. Modificar Prioridad\n";

    cout << "\n>> COLA DE ARRANQUE\n";
    cout << " 5. Encolar Paso de Arranque\n";
    cout << " 6. Ejecutar Paso de Arranque\n";
    cout << " 7. Mostrar Cola de Arranque\n";

    cout << "\n>> GESTIÓN DE MEMORIA (PILA)\n";
    cout << " 8. Asignar Memoria a Proceso\n";
    cout << " 9. Liberar Memoria\n";
    cout << "10. Mostrar Memoria\n";

    cout << "\n==============================================\n";
    cout << " 0. Salir\n";
    cout << "==============================================\n";
    cout << "Seleccione una opción: ";
}

// Función principal
int main() {
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        switch (opcion) {
            case 1: registrarProceso(); break;
            case 2: eliminarProceso(); break;
            case 3: mostrarProcesos(); break;
            case 4: modificarPrioridad(); break;
            case 5: encolarArranque(); break;
            case 6: ejecutarArranque(); break;
            case 7: mostrarColaArranque(); break;
            case 8: asignarMemoria(); break;
            case 9: liberarMemoria(); break;
            case 10: mostrarMemoria(); break;
            case 0: cout << "Saliendo del sistema...\n"; break;
            default: cout << "Opción inválida. Intente nuevamente.\n";
        }
        cout << endl;
    } while (opcion != 0);

    return 0;
}

