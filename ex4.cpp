#include <iostream>
#include <mpi.h>
#include <math.h>

#define N 800
// #define N 3000

using namespace std;

int f_beg(int rank);
void initialize_A(double A[][N]);
void print_matrix(double A[][N]);
double infNorm(double A[][N]);

int size;
double A[N][N];

double start, finish;

int main(int argc, char **argv)
{
	int rank;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		initialize_A(A);

		// print_matrix(A);
	}

    start = MPI_Wtime();

	double norm = infNorm(A);

    finish = MPI_Wtime();

    MPI_Finalize();

	if (rank == 0) {
		// cout << "Norm: " << norm << endl;
		cout << "TIME " << 1000 * (finish - start) << endl;
	}

	return 0;
}

int f_beg(int rank) {
	return N * rank / size;
}

void initialize_A(double A[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			A[i][j] = i * j;
		}
	}
}

void print_matrix(double A[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << A[i][j] << ' ';
		}

		cout << endl;
	}
}

double infNorm(double A[][N]) {
	int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int i_beg = f_beg(rank), i_end = f_beg(rank + 1);

	// Distribute A from process 0 to all other processes
	if (rank == 0) {
		for (int i = 1; i < size; i++) {
			MPI_Send(A[f_beg(i)], N * (f_beg(i + 1) - f_beg(i)), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(A[i_beg], N * (i_end - i_beg), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}


    // start = MPI_Wtime();
	// Multiply
	double s, norm;
	norm = fabs(A[i_beg][0]);
	for (int i = i_beg; i < i_end; i++) {
		s = 0.0;

		for (int j = 0; j < N; j++) {
			s += fabs(A[i][j]);
		}

		if (s > norm) {
			norm = s;
		}
	}
    // finish = MPI_Wtime();

	// Uncomment to see which section of the matrix was copied by this process
	// cout << "RANK: " << rank << endl;
	// print_matrix(A);

	// Send global maximum norm to process 0
	double global_norm;

	MPI_Reduce(&norm, &global_norm, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	norm = global_norm;

	return norm;
}
