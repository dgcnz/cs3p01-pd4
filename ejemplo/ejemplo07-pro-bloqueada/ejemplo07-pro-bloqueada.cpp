#include <mpi.h>
#include <math.h>
#include <ctime>
#include <cstdlib>  
#include <iostream>

int main(int argc, char **argv) {
  // MPI configuration
  int mpi_rank, mpi_size;
  MPI_Status stat;
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank); // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size); // Total number of processors
  MPI_Request request1, request2;           // Request handler
  double t0 = 0.0, tf = 0.0;                // Time variables

  // Algorithm configuration
  const int array_size = (int)pow(10,6);
  const int subpr_size = array_size/mpi_size;
  float send_arr[array_size], recv_arr[subpr_size];
  float result = 0.0, local_sum = 0.0, global_sum[mpi_size];

  // Generate an array[10^6] with numbers 0-1000
  if (mpi_rank == 0) {
    for (int i = 0; i < mpi_size; ++i)
      global_sum[i] = 0.0;

    srand((int)time(0));
    for (int i = 0; i < array_size; ++i)
      send_arr[i] = rand() % 1001;
  }

  /* ------------ BLOCKED COMMUNICATION ------------ */
  MPI_Barrier(MPI_COMM_WORLD);
  if (mpi_rank == 0)
    t0 = MPI_Wtime(); // Measure time

  MPI_Scatter(send_arr, subpr_size, MPI_INT, &recv_arr, subpr_size, MPI_INT, 0, MPI_COMM_WORLD);

  // Compute local average
  for (int i = 0; i < subpr_size; ++i)
    local_sum += recv_arr[i];
  local_sum /= subpr_size;

  MPI_Gather(&local_sum, 1, MPI_DOUBLE, global_sum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (mpi_rank == 0) {
    for (int i = 0; i < mpi_size; ++i)
      result += global_sum[i];
    result /= mpi_size; // Compute global average
    tf = MPI_Wtime();   // Measure time
    printf("Global average using blocked comm: %1.4f (Time taken: %1.6fms)\n", result, 1000*(tf-t0));

    // Empty the array
    for (int i = 0; i < mpi_size; ++i)
      global_sum[i] = 0.0;
  }

  // Reset parameters
  result = 0.0;
  local_sum = 0.0; 

  /* ------------ NON-BLOCKED COMMUNICATION ------------ */
  MPI_Barrier(MPI_COMM_WORLD);
  if (mpi_rank == 0)
    t0 = MPI_Wtime(); // Measure time

  MPI_Iscatter(send_arr, subpr_size, MPI_INT, &recv_arr, subpr_size, MPI_INT, 0, MPI_COMM_WORLD, &request1);

  // Compute local average
  for (int i = 0; i < subpr_size; ++i)
    local_sum += recv_arr[i];
  local_sum /= subpr_size;

  MPI_Igather(&local_sum, 1, MPI_DOUBLE, global_sum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request2);
  MPI_Wait(&request2, &stat);

  if (mpi_rank == 0) {
    for (int i = 0; i < mpi_size; ++i)
      result += global_sum[i];
    result /= mpi_size; // Compute global average
    tf = MPI_Wtime();   // Measure time
    printf("Global average using non-blocked comm: %1.4f (Time taken: %1.6fms)\n", result, 1000*(tf-t0));
  }

  MPI_Finalize(); // Exit MPI
}
