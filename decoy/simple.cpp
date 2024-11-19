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

    const int N = 1000; // Size of the array
    std::vector<int> arr(N);
    int total_sum = 0;

    // Initialize the array with some values
    for (int i = 0; i < N; i++)
    {
        arr[i] = i + 1; // Array will contain 1, 2, ..., N
    }

    // Start timer
    double start_time = omp_get_wtime();

    // Parallel region with tasks
#pragma omp parallel
    {
        // A single region to avoid multiple threads creating tasks redundantly
#pragma omp single
        {
            for (int i = 0; i < N; i++)
            {
                // Create a task for each array element
#pragma omp task shared(total_sum)
                {
#pragma omp atomic
                    total_sum += arr[i]; // Atomically add to the total sum
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
