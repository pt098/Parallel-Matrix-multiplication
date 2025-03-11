#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <chrono>
using namespace std;
using namespace std::chrono;

const int N = 500; // Adjust matrix size for testing

// Initialize a matrix with random double values.
void init_matrix(vector<double>& matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = static_cast<double>(rand()) / RAND_MAX;
    }
}

// Sequential matrix multiplication: C = A x B
void matmul(const vector<double>& A, const vector<double>& B, vector<double>& C, int n) {
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            double sum = 0.0;
            for (int k = 0; k < n; k++){
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

int main() {
    int n = N;
    vector<double> A(n * n), B(n * n), C(n * n);
    srand(time(0));
    init_matrix(A, n);
    init_matrix(B, n);
    
    // Time only the multiplication part.
    auto start = high_resolution_clock::now();
    matmul(A, B, C, n);
    auto end = high_resolution_clock::now();
    
    double elapsed = duration_cast<duration<double>>(end - start).count();
    cout << "Time taken for sequential matrix multiplication: " << elapsed << " seconds" << endl;
    
    // Write the output matrix to a file.
    ofstream fout("result_sequential.txt");
    if (fout.is_open()){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                fout << C[i * n + j] << " ";
            }
            fout << "\n";
        }
        fout.close();
    }
    
    return 0;
}
