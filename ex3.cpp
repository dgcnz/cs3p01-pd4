#include <iostream>
#include <mpi.h>

#define N 10

using namespace std;

int f_beg(int rank);
void initialize_A_v(int A[][N], int v[]);
void print_matrix(int A[][N]);
void print_vector(int v[]);

int size;
	
int main(int argc, char **argv)
{
    int rank;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int A[N][N], v[N], x[N];

	int i_beg = f_beg(rank), i_end = f_beg(rank + 1);

	// Distribute A and v from process 0 to all other processes
	if (rank == 0) {
		initialize_A_v(A, v);
		cout << "Matrix A:" << endl;
		print_matrix(A);
		cout << "Vector v:" << endl;
		print_vector(v);
	
		for (int i = 1; i < size; i++) {
			MPI_Send(A[f_beg(i)], N * (f_beg(i + 1) - f_beg(i)), MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(v, N, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(A[i_beg], N * (i_end - i_beg), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(v, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// Multiply
	for (int i = i_beg; i < i_end; i++) {
		x[i] = 0;

		for (int j = 0; j < N; j++) {
			x[i] += A[i][j] * v[j];
		}
	}
	
	// Uncomment to see which section of the matrix was copied by this process
	// cout << "RANK: " << rank << endl;
	// print_matrix(A);

	// Send x back from all processes to process 0
	if (rank != 0) {
		MPI_Send(x + i_beg, i_end - i_beg, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else {
		for (int i = 1; i < size; i++) {
			MPI_Recv(x + f_beg(i), f_beg(i + 1) - f_beg(i), MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

    MPI_Finalize();

	// Print result on the main process
	if (rank == 0) {
		cout << "Result vector x:" << endl;
		print_vector(x);
	}

	return 0;
}

int f_beg(int rank) {
	return N * rank / size;
}

void initialize_A_v(int A[][N], int v[]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			A[i][j] = i;
		}

		v[i] = i;
	}
}

void print_matrix(int A[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << A[i][j] << ' ';
		}

		cout << endl;
	}
}

void print_vector(int v[]) {
	for (int j = 0; j < N; j++) {
		cout << v[j] << endl;
	}
}
