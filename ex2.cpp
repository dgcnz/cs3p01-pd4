#include <mpi.h>
#include <stdio.h>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    int m = stoi(argv[1]); // trips
    int msg = 0;           // 0-length message
    double start_time;
    double end_time;

    int n;             // nodes
    int pid;           // processor id
    MPI_Status status; // mpi status

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    start_time = MPI_Wtime();

    if (n > 1)
    {
        for (int i = 0; i < m; ++i)
        {
            if (pid == 0)
            {
                MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                //printf("pid 0 sends to pid 1\n");
                MPI_Recv(&msg, 1, MPI_INT, n - 1, 0, MPI_COMM_WORLD, &status);
                //printf("pid 0 receives from pid %d\n", n - 1);
            }
            else
            {
                MPI_Recv(&msg, 1, MPI_INT, pid - 1, 0, MPI_COMM_WORLD, &status);
                //printf("pid %d receives from pid %d\n", pid, pid - 1);
                MPI_Send(&msg, 1, MPI_INT, (pid + 1) % n, 0, MPI_COMM_WORLD);
                //printf("pid %d sends to pid %d\n", pid, (pid + 1) % n);
            }
        }
    }
    else
    {
        printf("n should be greater than 1\n");
    }

    end_time = MPI_Wtime();
    if (pid == 0)
    {
        printf("%lf\n", 1000 * (end_time - start_time));
    }

    MPI_Finalize();
}
