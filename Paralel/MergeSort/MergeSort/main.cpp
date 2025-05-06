#include <iostream>
#include <fstream>
#include <chrono>
#include <mpi.h>
using namespace std;

ofstream out("output7.txt");

void Merge(int* v, int* temp, int l, int m, int h) {
    int i = l;
    int j = m + 1;
    int k = l;

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

    for (i = l; i <= h; i++) {
        v[i] = temp[i];
    }
}

void MergeSort(int* v, int* temp, int l, int h) {
    if (l < h) {
        int m = (l + h) / 2;
        MergeSort(v, temp, l, m);
        MergeSort(v, temp, m + 1, h);
        Merge(v, temp, l, m, h);
    }
}

int main(int argc, char** argv) {
    int rank, size;
    int numberOfElements;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        cout << "Number of elements: ";
        cin >> numberOfElements;
    }

    MPI_Bcast(&numberOfElements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int* v = nullptr;
    int* temp = nullptr;
    if (rank == 0) {
        v = new int[numberOfElements];
        temp = new int[numberOfElements];
        for (int i = 0; i < numberOfElements; i++)
            v[i] = numberOfElements - i - 1;
    }

    chrono::high_resolution_clock::time_point start, end;
    if (rank == 0) {
        start = chrono::high_resolution_clock::now();
    }

    int localSize = numberOfElements / size;
    int* localV = new int[localSize];
    int* localTemp = new int[localSize];

    MPI_Scatter(v, localSize, MPI_INT, localV, localSize, MPI_INT, 0, MPI_COMM_WORLD);

    MergeSort(localV, localTemp, 0, localSize - 1);

    MPI_Gather(localV, localSize, MPI_INT, v, localSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int startIdx = i * localSize;
            int endIdx = startIdx + localSize - 1;
            Merge(v, temp, 0, startIdx - 1, endIdx);
        }

        end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;

        for (int i = 0; i < numberOfElements; i++) {
            out << v[i] << " ";
            if ((i + 1) % 10 == 0)
                out << endl;
        }

        cout << "MergeSort with MPI: " << duration.count() << " seconds" << endl;

        delete[] v;
        delete[] temp;
    }

    delete[] localV;
    delete[] localTemp;

    MPI_Finalize();
    return 0;
}
