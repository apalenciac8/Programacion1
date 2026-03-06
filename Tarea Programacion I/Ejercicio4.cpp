#include <iostream>
#include <string>
using namespace std;

int main() {
    string texto;
    int a, b;

    cout << "Ingrese una palabra: ";
    cin >> texto;

    cout << "Primera letra a intercambiar: ";
    cin >> a;

    cout << "Segunda letra a intercambiar: ";
    cin >> b;

    if (b >= texto.length()) {
        b = texto.length() - 1;
    }

    while (a < b) {
        char temp = texto[a];
        texto[a] = texto[b];
        texto[b] = temp;
        a++;
        b--;
    }

    cout << "Resultado: " << texto << endl;

    return 0;
}