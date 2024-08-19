#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int n = 100; // Size of the array
    double data[n];

    // Initialize the array on the root process
    if (world_rank == 0) {
        for (int i = 0; i < n; i++) {
            data[i] = i + 1; // Sample data: 1, 2, 3, ..., 100
        }
    }

    int local_n = n / world_size;
    double local_data[local_n];

    // Scatter the data to all processes
    MPI_Scatter(data, local_n, MPI_DOUBLE, local_data, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Each process computes the square root of its portion
    double local_sum = 0.0;
    for (int i = 0; i < local_n; i++) {
        local_sum += sqrt(local_data[i]);
    }

    // Sum all local sums to get the global sum
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // The root process calculates and prints the mean square root
    if (world_rank == 0) {
        double mean_sqrt = global_sum / n;
        printf("The mean square root is %f\n", mean_sqrt);
    }

    MPI_Finalize();
    return 0;
}
