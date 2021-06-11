#include <mpi.h>  
#include <iostream>
#include <math.h>

int main(int argc, char **argv) {
  int mpi_rank, mpi_size;
  MPI_Status stat;
  MPI_Init(&argc, &argv);                   
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank); 
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  double t0 = 0.0, tf = 0.0;               

  int n, iter = 2;
  double local_sum, global_sum, result, error;
  const int max_iter = 6;
  const double pi = 3.141592653589793238462643;

  while (iter <= max_iter) {
    MPI_Barrier(MPI_COMM_WORLD); // barrera 
    local_sum = 0.0, global_sum = 0.0;
    result = 0.0, error = 0.0;

    if (mpi_rank == 0) {
      t0 = MPI_Wtime();  // tiempo inicial
      n = pow(10, iter); // cantidad de iteraciones
    }

    // Broadcast count
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Operate
    for (int i = (n/mpi_size * mpi_rank) + 1; i <= (n/mpi_size * (mpi_rank+1)); ++i)
    //for (int i = (n/mpi_size * mpi_rank); i <= (n/mpi_size * (mpi_rank+1)-1); ++i)
        local_sum += (4/(1 + pow((i - 0.5)/n, 2)));
//printf("suma local del proceso %d es %1.6f\n",mpi_rank, local_sum);
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (mpi_rank == 0) {
      result = global_sum / n; // resultado de PI 
      error = pi - result;     // error en calculo de PI
      tf = MPI_Wtime();        // tiempo de ejecucion
      printf("Valor de Pi con n = 10^%d: %1.6f, error: %e Tiempo: %1.6fms\n", iter, result, error, 1000*(tf-t0));
//	printf("%d %1.6f %d\n",n, 1000*(tf-t0), mpi_size);  

  }
    iter++;
  }

  MPI_Finalize(); // Exit MPI
}
