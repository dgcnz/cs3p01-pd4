#include<iostream>
#include<mpi.h>

double T1(double i){return i;}
double T2(double i){return i;}
double T3(double i){return i;}
double T4(double i){return i;}
double T5(double i){return i;}
double T6(double i){return i;}
double T7(double i){return i;}

void calculo(double i, double j, double k) {
  double a, b, c, d, result;
  int p,rank;
  MPI_Comm_size(MPI_COMM_WORLD,&p);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  /* primera fase */
  if (rank==0)
    a = T1(i);
  else if (rank==1)
    a = T2(j);
  else /* rank==2 */
    a = T3(k);

  MPI_Allreduce(&a, &b, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  c = T4(b);

  /* segunda fase */
  if (rank==0)
    d = T5(a/c);
  else if (rank==1)
    d = T6(a/c);
  else /* rank==2 */
    d = T7(a/c);

  MPI_Reduce(&d, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
  if (rank==0)
    std::cout << result << std::endl;
}

int main(int argc, char*argv[]) {
  MPI_Init(&argc, &argv);
  calculo(211, 422, 732);
  MPI_Finalize();
}
