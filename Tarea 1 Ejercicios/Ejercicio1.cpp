#include <iostream>
#include <string>
using namespace std;

int main() {
    string binario;
    int decimal = 0;

    cout << "Ingresa un numero en codigo binario: ";
    cin >> binario;

    for (int i = 0; i < binario.length(); i++) {
        decimal = decimal * 2 + (binario[i] - '0');
    }

    cout << "El numero en decimales es: " << decimal << endl;

    return 0;
}