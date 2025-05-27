#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cmath>
using namespace std;

void merge(int v[], int temp[], int l, int m, int h) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= h)
        if (v[i] < v[j])
            temp[k++] = v[i++];
        else
            temp[k++] = v[j++];
    while (i <= m)
        temp[k++] = v[i++];
    while (j <= h)
        temp[k++] = v[j++];
    for (i = l; i <= h; ++i)
        v[i] = temp[i];
}

void MergeSort(int v[], int temp[], int l, int h, int depth) {
    if (l >= h) return;

    int m = (l + h) / 2;

    if (depth > 0) {
        thread t1(MergeSort, v, temp, l, m, depth - 1);
        thread t2(MergeSort, v, temp, m + 1, h, depth - 1);
        t1.join();
        t2.join();
    }
    else {
        MergeSort(v, temp, l, m, 0);
        MergeSort(v, temp, m + 1, h, 0);
    }

    merge(v, temp, l, m, h);
}

int main() {
    int numberOfElements;
    cout << "Number of elements: ";
    cin >> numberOfElements;

    int* v = new int[numberOfElements];
    int* temp = new int[numberOfElements];

    for (int i = 0; i < numberOfElements; i++)
        v[i] = numberOfElements - i - 1;

    int depth = log2(thread::hardware_concurrency());

    auto start = chrono::high_resolution_clock::now();
    MergeSort(v, temp, 0, numberOfElements - 1, depth);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;

    ofstream out("output8.txt");
    for (int i = 0; i < numberOfElements; i++) {
        out << v[i] << " ";
        if (i % 10 == 0)
            out << endl;
    }

    cout << "\n MergeSort: " << duration.count() << " seconds\n";

    delete[] v;
    delete[] temp;
    return 0;
}
