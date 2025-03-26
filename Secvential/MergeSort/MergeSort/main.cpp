#include <iostream>
#include <fstream>
#include <chrono> 
using namespace std;

ofstream out("output.txt");

int *temp, *v;

void MergeSort(int v[], int l, int h) {

    if (l < h) {
        int m = (l + h) / 2;

        MergeSort(v, l, m);
        MergeSort(v, m + 1, h);

        int i = l;
        int j = m + 1;

        int k = 0;
        while (i <= m && j <= h) {
            if (v[i] < v[j])
                temp[k++] = v[i++];
            else
                temp[k++] = v[j++];
        }

        while (i <= m)
            temp[k++] = v[i++];

        while (j <= h)
            temp[k++] = v[j++];

        k = 0;
        for (i = l; i <= h; i++) {
            v[i] = temp[k++];
        }
    }
}

int main() {

    int numberOfElements;
   
    cout << "Number of elements: ";
    cin >> numberOfElements;

    v = new int[numberOfElements];
    temp = new int[numberOfElements];

    int k = 0;
    for (int i = numberOfElements - 1; i >= 0; i--)
        v[k++] = i;

    auto start = chrono::high_resolution_clock::now();

    MergeSort(v, 0, numberOfElements - 1);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
   

    for (int i = 0; i <= numberOfElements - 1; i++) {
        out << v[i] << " ";
        if (i % 10 == 0)
            out << endl;
    }

    cout << endl;
    cout << "MergeSort: " << duration.count() << " seconds" << endl;
    return 0;
}
