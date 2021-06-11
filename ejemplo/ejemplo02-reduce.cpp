/*  MPI_Reduce  */

#include <mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
       int   opcion, i, stop=0;
        int   me, numprocs,res;
        int data;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    srand(me);
        data=rand()%10+1;
	printf("Proceso %d, data: %d\n",me,data);

    MPI_Barrier(MPI_COMM_WORLD);

    while (!stop) {
        if (me==0) {
            printf("\nEscoja operacion:\n\n");
            printf("1 maximo global \n2 minimo global\n");
            printf("3 suma global\n4 producto global\n0 salida\n");
            printf("Resultado: ");
            if (scanf("%d", &opcion)!=1) {
                opcion=0;
            }
        }

	MPI_Bcast(&opcion,1,MPI_INT,0,MPI_COMM_WORLD);

        switch (opcion) {
        case 1: MPI_Reduce(&data, &res, 1, MPI_INT, MPI_MAX,
                           0, MPI_COMM_WORLD);
                break;
        case 2: MPI_Reduce(&data, &res, 1, MPI_INT, MPI_MIN,
                           0, MPI_COMM_WORLD);
                break;
        case 3: MPI_Reduce(&data, &res, 1, MPI_INT, MPI_SUM,
                           0, MPI_COMM_WORLD);
                break;
        case 4: MPI_Reduce(&data, &res, 1, MPI_INT, MPI_PROD				,0, MPI_COMM_WORLD);
                break;
        default: stop=1;
        }

	 if (me==0 && (!stop)) {
            printf("Proceso %d: Resultado: ", me);
                printf("%d \n", res);
        }
    }
    MPI_Finalize();
}
