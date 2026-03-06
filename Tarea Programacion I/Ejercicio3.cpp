#include <iostream>
using namespace std;

int main() {
    int n;

    cout << "Cuantos numeros se ingresaran?";
    cin >> n;

    int numeros[n];
    int inversos[n];

    for (int i = 0; i < n; i++) {
        cout << "Numero " << i + 1 << ": ";
        cin >> numeros[i];
    }

    for (int i = 0; i < n; i++) {
        inversos[i] = -numeros[i];
    }

    cout << "Inversos: ";
    for (int i = 0; i < n; i++) {
        cout << inversos[i] << " ";
    }

    return 0;
}