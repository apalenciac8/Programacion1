#include <iostream>
#include <string>
using namespace std;

// ─────────────────────────────────────────
// Clase Alumno
// ─────────────────────────────────────────
class Alumno {
public:
    string nombres;
    string apellidos;
    string carnet;
    string email;
    string telefono;
    double notas[5];

    // Constructor por defecto
    Alumno() {
        for (int i = 0; i < 5; i++) notas[i] = 0.0;
    }

    // Calcula el promedio de las 5 notas
    double promedio() const {
        double suma = 0.0;
        for (int i = 0; i < 5; i++) suma += notas[i];
        return suma / 5.0;
    }

    // Muestra la información del alumno
    void mostrar() const {
        cout << "\n  Nombres   : " << nombres;
        cout << "\n  Apellidos : " << apellidos;
        cout << "\n  Carnet    : " << carnet;
        cout << "\n  Email     : " << email;
        cout << "\n  Telefono  : " << telefono;
        cout << "\n  Notas     : ";
        for (int i = 0; i < 5; i++) {
            cout << notas[i];
            if (i < 4) cout << ", ";
        }
        cout << "\n  Promedio  : " << promedio() << "\n";
    }
};

// ─────────────────────────────────────────
// Nodo de la lista enlazada
// ─────────────────────────────────────────
class Nodo {
public:
    Alumno alumno;     
    Nodo*  siguiente;

    Nodo() : siguiente(nullptr) {}
};

// ─────────────────────────────────────────
// Lista enlazada simple
// ─────────────────────────────────────────
class ListaEnlazada {
private:
    Nodo* cabeza;

public:
    ListaEnlazada() : cabeza(nullptr) {}

    // Inserta un nuevo alumno al final de la lista
    void insertar(const Alumno& a) {
        Nodo* nuevo = new Nodo();
        nuevo->alumno = a;

        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
    }

    // Recorre la lista y muestra todos los alumnos
    void mostrarTodos() const {
        if (cabeza == nullptr) {
            cout << "\nLa lista esta vacia.\n";
            return;
        }

        Nodo* actual = cabeza;
        int contador = 1;
        while (actual != nullptr) {
            cout << "\n--- Alumno " << contador++ << " ---";
            actual->alumno.mostrar();
            actual = actual->siguiente;
        }
    }

    // Libera toda la memoria dinámica
    ~ListaEnlazada() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
};

// ─────────────────────────────────────────
// Funciones auxiliares de ingreso
// ─────────────────────────────────────────

// Solicita una nota válida (0 – 10)
double pedirNota(int numero) {
    double nota;
    while (true) {
        cout << "    Nota " << numero << " (0-10): ";
        cin  >> nota;

        if (cin.fail() || nota < 0.0 || nota > 10.0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "    Valor invalido. Ingrese un numero entre 0 y 10.\n";
        } else {
            cin.ignore(1000, '\n');
            return nota;
        }
    }
}

// Solicita y devuelve un objeto Alumno con sus datos
Alumno pedirAlumno() {
    Alumno a;
    cin.ignore(1000, '\n'); // limpiar buffer antes de getline

    cout << "  Nombres   : "; getline(cin, a.nombres);
    cout << "  Apellidos : "; getline(cin, a.apellidos);
    cout << "  Carnet    : "; getline(cin, a.carnet);
    cout << "  Email     : "; getline(cin, a.email);
    cout << "  Telefono  : "; getline(cin, a.telefono);

    cout << "  Ingrese las 5 notas:\n";
    for (int i = 0; i < 5; i++) {
        a.notas[i] = pedirNota(i + 1);
    }
    return a;
}

// ─────────────────────────────────────────
// main
// ─────────────────────────────────────────
int main() {
    ListaEnlazada lista;
    char respuesta;

    cout << "========================================\n";
    cout << "   Sistema de Registro de Alumnos\n";
    cout << "========================================\n";

    do {
        cout << "\nIngrese los datos del alumno:\n";
        Alumno nuevo = pedirAlumno();
        lista.insertar(nuevo);

        cout << "\n¿Desea agregar otro alumno? (s/n): ";
        cin  >> respuesta;
        cin.ignore(1000, '\n');

    } while (respuesta == 's' || respuesta == 'S');

    cout << "\n========================================\n";
    cout << "   Informacion de todos los alumnos\n";
    cout << "========================================\n";
    lista.mostrarTodos();

    cout << "\n========================================\n";
    cout << "Programa finalizado. Memoria liberada.\n";
    return 0;
}
