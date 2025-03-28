#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

void multiplyMatrices(int** A, int** B, int** C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
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
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <N>" << endl;
        return 1;
    }

    int N = stoi(argv[1]);

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

    auto start = high_resolution_clock::now();
    multiplyMatrices(A, B, C, N);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Sequential time: " << duration.count() << " ms" << endl;

    writeMatrixToFile(C, N, "sequential_result.txt");

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
