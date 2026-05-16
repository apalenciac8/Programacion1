#include <iostream>
using namespace std;

// 1. Estructura del nodo
struct Nodo {
    int dato;
    Nodo* siguiente;
};

// Función para agregar un nodo al final de la lista
void agregarNodo(Nodo*& cabeza, int valor) {
    // 2. Crear nodo dinámicamente con 'new'
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->dato = valor;
    nuevoNodo->siguiente = nullptr;

    if (cabeza == nullptr) {
        cabeza = nuevoNodo;
    } else {
        // Recorrer hasta el último nodo y enlazar
        Nodo* temp = cabeza;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }
}

// 4. Mostrar la lista completa
void mostrarLista(Nodo* cabeza) {
    cout << "\nContenido de la lista:" << endl;
    Nodo* temp = cabeza;
    while (temp != nullptr) {
        cout << "[" << temp->dato << " | * ]";
        if (temp->siguiente != nullptr) {
            cout << " -> ";
        } else {
            cout << " -> [NULL]";
        }
        temp = temp->siguiente;
    }
    cout << endl;
}

// 5. Liberar memoria con 'delete'
void liberarMemoria(Nodo*& cabeza) {
    Nodo* temp;
    while (cabeza != nullptr) {
        temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}

int main() {
    Nodo* cabeza = nullptr;
    char respuesta;
    int valor;

    cout << "=== Lista Enlazada Dinamica ===" << endl;

    // 3. Menú interactivo
    do {
        cout << "\n¿Desea ingresar un nuevo valor? (S/N): ";
        cin >> respuesta;
        respuesta = toupper(respuesta);

        if (respuesta == 'S') {
            cout << "Ingrese el valor: ";
            cin >> valor;
            agregarNodo(cabeza, valor);
            cout << "Nodo agregado correctamente." << endl;
        } else if (respuesta != 'N') {
            cout << "Opcion invalida. Por favor ingrese S o N." << endl;
        }

    } while (respuesta != 'N');

    // Mostrar lista si tiene elementos
    if (cabeza == nullptr) {
        cout << "\nLa lista esta vacia." << endl;
    } else {
        mostrarLista(cabeza);
    }

    // Liberar memoria
    liberarMemoria(cabeza);
    cout << "\nMemoria liberada correctamente." << endl;

    return 0;
}