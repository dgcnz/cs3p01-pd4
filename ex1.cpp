#include <cstdio>
#include <mpi.h>
#include <vector>

using namespace std;

int const ROUNDS = 18;

int main(int argc, char *argv[])
{
    int prank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2)
        MPI_Abort(MPI_COMM_WORLD, -1);

    double start, finish;

    int           k = 0;
    vector<float> a(1, 0.0);
    start = MPI_Wtime();
    while (k < ROUNDS)
    {
        if (k or prank)
        {
            MPI_Recv(&k,
                     1,
                     MPI_FLOAT,
                     1 - prank,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            a.resize(1 << k);
            MPI_Recv(a.data(),
                     1 << k,
                     MPI_FLOAT,
                     1 - prank,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }
        if (k == ROUNDS)
            break;
        a.resize(1 << ++k, prank);
        MPI_Send(&k, 1, MPI_FLOAT, 1 - prank, 0, MPI_COMM_WORLD);
        MPI_Send(a.data(), 1 << k, MPI_FLOAT, 1 - prank, 0, MPI_COMM_WORLD);
    }

    finish = MPI_Wtime();
    if (prank == 0)
        printf("TIME %lf\n", 1000 * (finish - start));

    MPI_Finalize();
    return 0;
}
