fstream archivo("datos.txt", ios::in | ios::out);
archivo << "nuevo dato";
archivo.seekg(0);
string contenido;
archivo >> contenido;




ifstream f("archivo.txt", ios::in);
ofstream f("archivo.txt", ios::out);

ofstream f("log.txt", ios::app);
f << "Nueva entrada de log" << endl;

fstream f("datos.txt", ios::out | ios::trunc);

fstream f("imagen.bin", ios::in | ios::binary);

ifstream archivo("datos.txt");
if (!archivo.is_open()) {
    cerr << "Error: No se pudo abrir el archivo" << endl;
}

int numero;
archivo >> numero;
if (archivo.fail()) {
    cout << "Error de lectura o formato incorrecto" << endl;
}

while (archivo.good()) {
    getline(archivo, linea);
}

while (getline(archivo, linea)) {
    procesar(linea);
}

#include <fstream>
#include <iostream>
using namespace std;
int main() {
    ofstream archivo("alumnos.txt");
    if (!archivo.is_open()) { return 1; }
    archivo << "ID,Nombre,Nota" << endl;
    archivo << "001,Ana Lopez,95.5" << endl;
    archivo << "002,Juan Perez,87.0" << endl;
    archivo.close();
    return 0;
}

ifstream original("alumnos.txt");
ofstream temporal("temp.txt");
string linea;
while (getline(original, linea)) {
    if (linea.find("001") != string::npos)
        temporal << "001,Ana Lopez,98.0" << endl;
    else
        temporal << linea << endl;
}
original.close(); temporal.close();
remove("alumnos.txt");
rename("temp.txt", "alumnos.txt");

struct Alumno { int id; char nombre[50]; float nota; };
ofstream archivo("alumnos.bin", ios::binary);
Alumno a = {1, "Ana Lopez", 95.5};
archivo.write(reinterpret_cast<char*>(&a), sizeof(Alumno));

ifstream archivo("alumnos.bin", ios::binary);
Alumno a;
archivo.read(reinterpret_cast<char*>(&a), sizeof(Alumno));
cout << a.nombre << " - Nota: " << a.nota << endl;


int n = 2; // Tercer registro (base 0)
archivo.seekg(n * sizeof(Alumno), ios::beg);
archivo.read(reinterpret_cast<char*>(&a), sizeof(Alumno));

archivo.seekg(0, ios::beg);  // Ir al inicio
archivo.seekg(0, ios::end);  // Ir al final
archivo.seekg(-10, ios::end); // 10 bytes antes del final

archivo.seekg(0, ios::end);
int total = archivo.tellg() / sizeof(Alumno);
cout << "Total de registros: " << total << endl;
archivo.seekg(-sizeof(Alumno), ios::end);
archivo.read(reinterpret_cast<char*>(&a), sizeof(Alumno));

struct Producto { int codigo; char descripcion[80]; double precio; };
ifstream archivo("productos.bin", ios::binary);
if (!archivo.is_open()) { return 1; }
Producto p;
while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
    cout << p.codigo << " | " << p.descripcion;
    cout << " | Q" << p.precio << endl;
}
archivo.close();


archivo.read(buffer, sizeof(Producto));
if (archivo.gcount() == sizeof(Producto)) {
    // Lectura completa y exitosa
}


double total = 0; int contador = 0;
while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
    if (p.precio > 100.0) { total += p.precio; contador++; }
}
cout << "Productos sobre Q100: " << contador << endl;
cout << "Suma total: Q" << total << endl;


ifstream archivo("datos.txt");
string linea;
getline(archivo, linea); // Lee una línea completa

ofstream salida("resultado.txt");
salida << "Hola, mundo!" << endl;

struct Alumno {
    int   id;                   // 4 bytes  – identificador único
    char  nombre[MAX_NOMBRE];   // 50 bytes – nombre (tamaño fijo)
    float nota;                 // 4 bytes  – calificación
};


ofstream archivo;
archivo.open("alumnos.txt");
if (!archivo.is_open()) { cerr << "Error..."; return; }
archivo << 1 << " " << "Ana_Lopez" << " " << 88.5 << endl;
archivo.close();

while (archivo >> a.id >> a.nombre >> a.nota) {
    cout << left << setw(6) << a.id
         << setw(20)        << a.nombre
         << right << setw(8)<< a.nota << endl;
}

while (entrada >> a.id >> a.nombre >> a.nota) {
    if (a.id == id) a.nota = nuevaNota;
    temporal << a.id << " " << a.nombre << " " << a.nota << endl;
}
remove("alumnos.txt");
rename("temp.txt", "alumnos.txt");

ofstream bin("alumnos.dat", ios::binary | ios::out);
bin.write(reinterpret_cast<char*>(&a), sizeof(Alumno));

streampos offset = posicion * sizeof(Alumno);
binario.seekg(offset, ios::beg);
binario.read(reinterpret_cast<char*>(&a), sizeof(Alumno));

while (binario.read(reinterpret_cast<char*>(&a), sizeof(Alumno))) {
    cout << a.id << "  " << a.nombre << "  " << a.nota << endl;
}
