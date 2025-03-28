#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

struct ThreadArgs {
    int** A;
    int** B;
    int** C;
    int N;
    int startRow;
    int endRow;
};

void initializeMatrix(int** matrix, int N) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 9);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dist(gen);
        }
    }
}

void* multiplySubmatrix(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*)args;
    for (int i = tArgs->startRow; i < tArgs->endRow; i++) {
        for (int j = 0; j < tArgs->N; j++) {
            tArgs->C[i][j] = 0;
            for (int k = 0; k < tArgs->N; k++) {
                tArgs->C[i][j] += tArgs->A[i][k] * tArgs->B[k][j];
            }
        }
    }
    pthread_exit(nullptr);
}

void writeMatrixToFile(int** matrix, int N, const string& filename) {
    ofstream outfile(filename);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            outfile << matrix[i][j] << " ";
        }
        outfile << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <N> <num_threads>" << endl;
        return 1;
    }

    int N = stoi(argv[1]);
    int numThreads = stoi(argv[2]);

    // Allocate matrices
    int** A = new int*[N];
    int** B = new int*[N];
    int** C = new int*[N];
    for (int i = 0; i < N; i++) {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    initializeMatrix(A, N);
    initializeMatrix(B, N);

    pthread_t threads[numThreads];
    ThreadArgs tArgs[numThreads];
    int rowsPerThread = N / numThreads;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < numThreads; i++) {
        tArgs[i].A = A;
        tArgs[i].B = B;
        tArgs[i].C = C;
        tArgs[i].N = N;
        tArgs[i].startRow = i * rowsPerThread;
        tArgs[i].endRow = (i == numThreads - 1) ? N : (i + 1) * rowsPerThread;

        pthread_create(&threads[i], nullptr, multiplySubmatrix, &tArgs[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], nullptr);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Pthreads time (" << numThreads << " threads): " << duration.count() << " ms" << endl;

    writeMatrixToFile(C, N, "pthread_result.txt");

    // Free memory
    for (int i = 0; i < N; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
