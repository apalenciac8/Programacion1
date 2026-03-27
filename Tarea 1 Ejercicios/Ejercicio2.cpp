#include <iostream>
using namespace std;

int main() {
    int n;

    cout << "Ingresa un numero que sea positivo: ";
    cin >> n;

    int arreglo[n];

    for (int i = 0; i < n; i++) {
        arreglo[i] = n - i;
    }

    for (int i = 0; i < n; i++) {
        cout << arreglo[i] << " ";
    }

    return 0;
}