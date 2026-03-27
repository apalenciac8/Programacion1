#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

// Estructura para archivo binario
struct Alumno {
    int id;
    char nombre[50];
    float nota;
};

// ============================================================
// EJERCICIO 1: Crear alumnos.txt y guardar un alumno
// ============================================================
void crearAlumnoTexto(int id, string nombre, float nota) {
    // ios::app agrega al final sin borrar lo existente
    ofstream archivo("alumnos.txt", ios::app);

    // Verificar que el archivo se abrio correctamente
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir alumnos.txt" << endl;
        return;
    }

    // Guardar el registro en el archivo
    archivo << id << " " << nombre << " " << nota << endl;
    archivo.close();
    cout << "Alumno guardado correctamente." << endl;
}

// ============================================================
// EJERCICIO 2: Listar todos los registros del archivo de texto
// ============================================================
void listarAlumnosTexto() {
    ifstream archivo("alumnos.txt");

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir alumnos.txt" << endl;
        return;
    }

    int id;
    string nombre;
    float nota;

    cout << "\n--- Lista de Alumnos ---" << endl;

    // Leer mientras haya datos en el archivo
    while (archivo >> id >> nombre >> nota) {
        cout << "ID: " << id << " | Nombre: " << nombre << " | Nota: " << nota << endl;
    }

    archivo.close();
}

// ============================================================
// EJERCICIO 3: Buscar un alumno por ID
// ============================================================
void buscarAlumnoPorId(int idBuscado) {
    ifstream archivo("alumnos.txt");

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir alumnos.txt" << endl;
        return;
    }

    int id;
    string nombre;
    float nota;
    bool encontrado = false;

    while (archivo >> id >> nombre >> nota) {
        if (id == idBuscado) {
            cout << "\nAlumno encontrado:" << endl;
            cout << "ID: " << id << " | Nombre: " << nombre << " | Nota: " << nota << endl;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "No se encontro un alumno con ID " << idBuscado << endl;
    }

    archivo.close();
}

// ============================================================
// EJERCICIO 4: Actualizar la nota de un alumno usando archivo temporal
// ============================================================
void actualizarNota(int idBuscado, float nuevaNota) {
    ifstream archivO("alumnos.txt");
    ofstream temporal("temp.txt");

    if (!archivO.is_open() || !temporal.is_open()) {
        cout << "Error al abrir archivos." << endl;
        return;
    }

    int id;
    string nombre;
    float nota;
    bool actualizado = false;

    // Copiar todos los registros al archivo temporal
    // Si el ID coincide, se guarda con la nueva nota
    while (archivO >> id >> nombre >> nota) {
        if (id == idBuscado) {
            temporal << id << " " << nombre << " " << nuevaNota << endl;
            actualizado = true;
        } else {
            temporal << id << " " << nombre << " " << nota << endl;
        }
    }

    archivO.close();
    temporal.close();

    // Reemplazar el archivo original con el temporal
    remove("alumnos.txt");
    rename("temp.txt", "alumnos.txt");

    if (actualizado) {
        cout << "Nota actualizada correctamente." << endl;
    } else {
        cout << "No se encontro el alumno con ID " << idBuscado << endl;
    }
}

// ============================================================
// EJERCICIO 5: Crear archivo binario alumnos.dat con struct
// ============================================================
void crearAlumnoBinario(int id, string nombre, float nota) {
    // ios::binary para escritura binaria, ios::app para agregar
    ofstream archivo("alumnos.dat", ios::binary | ios::app);

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir alumnos.dat" << endl;
        return;
    }

    // Llenar la estructura con los datos
    Alumno alumno;
    alumno.id = id;
    alumno.nota = nota;
    strncpy(alumno.nombre, nombre.c_str(), 49);
    alumno.nombre[49] = '\0'; // Asegurar terminacion de cadena

    // Escribir el struct completo en binario
    archivo.write(reinterpret_cast<char*>(&alumno), sizeof(Alumno));
    archivo.close();

    cout << "Alumno guardado en archivo binario." << endl;
}

// ============================================================
// EJERCICIO 6: Leer un registro especifico con acceso aleatorio
// ============================================================
void leerRegistroBinario(int posicion) {
    ifstream archivo("alumnos.dat", ios::binary);

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir alumnos.dat" << endl;
        return;
    }

    Alumno alumno;

    // seekg mueve el puntero a la posicion indicada
    // Cada registro ocupa sizeof(Alumno) bytes
    archivo.seekg(posicion * sizeof(Alumno));

    // Leer el registro en esa posicion
    archivo.read(reinterpret_cast<char*>(&alumno), sizeof(Alumno));

    if (archivo.good()) {
        cout << "\nRegistro en posicion " << posicion << ":" << endl;
        cout << "ID: " << alumno.id << " | Nombre: " << alumno.nombre << " | Nota: " << alumno.nota << endl;
    } else {
        cout << "No existe un registro en la posicion " << posicion << endl;
    }

    archivo.close();
}

// ============================================================
// EJERCICIO 7: Recorrer el archivo binario y mostrar todos
// ============================================================
void listarAlumnosBinarios() {
    ifstream archivo("alumnos.dat", ios::binary);

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir alumnos.dat" << endl;
        return;
    }

    Alumno alumno;
    int contador = 0;

    cout << "\n--- Lista de Alumnos (Binario) ---" << endl;

    // Leer registro por registro hasta el fin del archivo
    while (archivo.read(reinterpret_cast<char*>(&alumno), sizeof(Alumno))) {
        cout << "ID: " << alumno.id << " | Nombre: " << alumno.nombre << " | Nota: " << alumno.nota << endl;
        contador++;
    }

    cout << "Total de registros: " << contador << endl;
    archivo.close();
}

// ============================================================
// MENU PRINCIPAL
// ============================================================
int main() {
    int opcion;

    cout << "=== Sistema de Alumnos ===" << endl;

    do {
        cout << "\n1. Agregar alumno (texto)" << endl;
        cout << "2. Listar alumnos (texto)" << endl;
        cout << "3. Buscar alumno por ID" << endl;
        cout << "4. Actualizar nota" << endl;
        cout << "5. Agregar alumno (binario)" << endl;
        cout << "6. Leer registro binario por posicion" << endl;
        cout << "7. Listar todos los binarios" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            int id; string nombre; float nota;
            cout << "ID: "; cin >> id;
            cout << "Nombre: "; cin >> nombre;
            cout << "Nota: "; cin >> nota;
            crearAlumnoTexto(id, nombre, nota);

        } else if (opcion == 2) {
            listarAlumnosTexto();

        } else if (opcion == 3) {
            int id;
            cout << "ID a buscar: "; cin >> id;
            buscarAlumnoPorId(id);

        } else if (opcion == 4) {
            int id; float nota;
            cout << "ID del alumno: "; cin >> id;
            cout << "Nueva nota: "; cin >> nota;
            actualizarNota(id, nota);

        } else if (opcion == 5) {
            int id; string nombre; float nota;
            cout << "ID: "; cin >> id;
            cout << "Nombre: "; cin >> nombre;
            cout << "Nota: "; cin >> nota;
            crearAlumnoBinario(id, nombre, nota);

        } else if (opcion == 6) {
            int pos;
            cout << "Posicion (0 en adelante): "; cin >> pos;
            leerRegistroBinario(pos);

        } else if (opcion == 7) {
            listarAlumnosBinarios();
        }

    } while (opcion != 0);

    cout << "Programa finalizado." << endl;
    return 0;
}
