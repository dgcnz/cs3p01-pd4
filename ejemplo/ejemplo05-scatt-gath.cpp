#include <mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv) {
   int size, rank;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int globaldata[size];
  int localdata;
  int i;

  if (rank == 0) {
          for (i=0; i<size; i++)
              globaldata[i] = 2*i+1;
	cout<<"Proceso "<<rank<<", tiene data: ";
          for (i=0; i<size; i++)
		  cout<<globaldata[i]<<" ";
	cout<<endl;    

  }
// Comunicacion
     MPI_Scatter(&globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);
// Computo
     cout<<"Proceso "<<rank<<" tiene data "<<localdata<<endl;
       localdata *= 2;
   cout<<"Proceso "<<rank<<" duplica la data a : "<<localdata<<endl;
// Comunicacion
   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Gather(&localdata, 1, MPI_INT, &globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);

   if (rank == 0) {
	   cout<<"Proceso "<<rank<<", tiene data: ";
           for (i=0; i<size; i++)
		   cout<<globaldata[i]<<" ";
	cout<<endl;
   }

   MPI_Finalize();
    return 0;
}












