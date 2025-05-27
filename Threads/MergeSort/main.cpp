#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
using namespace std;

ofstream out("output8.txt");

void merge(int v[], int l, int m, int h) {
    int i = l, j = m + 1;
    vector<int> temp(h - l + 1);

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

    for (i = l, k = 0; i <= h; ++i)
        v[i] = temp[k++];
}

void MergeSort(int v[], int l, int h, int depth) {
    if (l < h) {
        int m = (l + h) / 2;

        if (depth <= 0) {
            MergeSort(v, l, m, depth - 1);
            MergeSort(v, m + 1, h, depth - 1);
        }
        else {
            thread t1(MergeSort, v, l, m, depth - 1);
            thread t2(MergeSort, v, m + 1, h, depth - 1);
            t1.join();
            t2.join();
        }

        merge(v, l, m, h);
    }
}

int main() {
    int numberOfElements;
    cout << "Number of elements: ";
    cin >> numberOfElements;

    int* v = new int[numberOfElements];
    for (int i = 0; i < numberOfElements; i++)
        v[i] = numberOfElements - i - 1;

    auto start = chrono::high_resolution_clock::now();
    MergeSort(v, 0, numberOfElements - 1, 3);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;

    for (int i = 0; i < numberOfElements; i++) {
        out << v[i] << " ";
        if (i % 10 == 0)
            out << endl;
    }

    cout << "\nMergeSort (parallel): " << duration.count() << " seconds" << endl;

    delete[] v;
    return 0;
}
