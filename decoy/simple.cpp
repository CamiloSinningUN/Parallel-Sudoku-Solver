#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib> // For std::atoi

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <number_of_threads>" << std::endl;
        return 1;
    }

    // Read the number of threads from the command line
    int num_threads = std::atoi(argv[1]);
    if (num_threads <= 0)
    {
        std::cerr << "Error: Number of threads must be a positive integer." << std::endl;
        return 1;
    }

    omp_set_num_threads(num_threads);

    // Print the number of processors available
    int num_procs = omp_get_num_procs();
    std::cout << "Number of processors available: " << num_procs << std::endl;

    const int N = 10000000; // Larger size for meaningful workload
    std::vector<int> arr(N);
    long long total_sum = 0;

    // Initialize the array with some values
    for (int i = 0; i < N; i++)
    {
        arr[i] = i + 1; // Array will contain 1, 2, ..., N
    }

    // Start timer
    double start_time = omp_get_wtime();

    // Parallel region with tasks
    int chunk_size = N / num_threads;
#pragma omp parallel
    {
#pragma omp single
        {
            for (int i = 0; i < num_threads; i++)
            {
                int start = i * chunk_size;
                int end = (i == num_threads - 1) ? N : start + chunk_size;
#pragma omp task shared(total_sum)
                {
                    long long local_sum = 0;
                    for (int j = start; j < end; j++)
                    {
                        local_sum += arr[j];
                    }
#pragma omp atomic
                    total_sum += local_sum;
                }
            }
        }
    }

    // End timer
    double end_time = omp_get_wtime();

    // Print the total sum and elapsed time
    std::cout << "Total Sum: " << total_sum << std::endl;
    std::cout << "Time Taken: " << (end_time - start_time) << " seconds" << std::endl;

    return 0;
}