#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc,char *argv[])
{
	int rank, size,x;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	cout<<"Peekaboo! desde "<< rank<<" de "<<size<<" x: "<<x<<endl;
//	printf("Peekaboo! desde %d de %d\n", rank, size);
	MPI_Finalize();
}

