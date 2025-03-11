#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <pthread.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

const int N = 500;       // Matrix size
const int NUM_THREADS = 4; // Number of threads (adjust as needed)
int n = N;
vector<double> A, B, C;

struct ThreadData {
    int start_row;
    int end_row;
};

// Thread function: each thread computes the assigned rows of the output matrix.
void* thread_matmul(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    for (int i = data->start_row; i < data->end_row; i++){
        for (int j = 0; j < n; j++){
            double sum = 0.0;
            for (int k = 0; k < n; k++){
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
    return nullptr;
}

void init_matrix(vector<double>& matrix, int n) {
    for (int i = 0; i < n * n; i++){
        matrix[i] = static_cast<double>(rand()) / RAND_MAX;
    }
}

int main() {
    n = N;
    A.resize(n * n);
    B.resize(n * n);
    C.resize(n * n);
    srand(time(0));
    init_matrix(A, n);
    init_matrix(B, n);
    
    vector<pthread_t> threads(NUM_THREADS);
    vector<ThreadData> thread_data(NUM_THREADS);
    int rows_per_thread = n / NUM_THREADS;
    int remainder = n % NUM_THREADS;
    
    // Start timing the multiplication portion.
    auto start = high_resolution_clock::now();
    
    int current_row = 0;
    for (int i = 0; i < NUM_THREADS; i++){
        thread_data[i].start_row = current_row;
        int rows = rows_per_thread + (i < remainder ? 1 : 0);
        thread_data[i].end_row = current_row + rows;
        current_row += rows;
        pthread_create(&threads[i], nullptr, thread_matmul, &thread_data[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], nullptr);
    }
    
    auto end = high_resolution_clock::now();
    double elapsed = duration_cast<duration<double>>(end - start).count();
    cout << "Time taken for parallel matrix multiplication (pthread): " << elapsed << " seconds" << endl;
    
    // Write the result to a file.
    ofstream fout("result_pthread.txt");
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
