#include <mpi.h>  
#include <iostream>
using namespace std;

void T1(double a[],double *v){*v=a[1];cout<<"T1:"<<*v<<endl;}
void T2(double b[],double *w){*w=b[2];cout<<"T2:"<<*w<<endl;}
void T3(double b[],double *v){*v=b[3];cout<<"T3:"<<*v<<endl;}
void T4(double c[],double *w){*w=c[4];cout<<"T4:"<<*w<<endl;}
void T5(double c[],double *v){*v=c[5];cout<<"T5:"<<*v<<endl;}
void T6(double a[],double *w){*w=a[6];cout<<"T6:"<<*w<<endl;}

int main(int argc,char *argv[]) {
int N=10,p,rank;;
double a[N],b[N],c[N],v=0.0,w=0.0;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD,&p);     	
MPI_Comm_rank(MPI_COMM_WORLD,&rank);

for (int i=0;i<N;i++){
 a[i]=i;
 b[i]=2*i;
 c[i]=3*i;
}

     if (rank==0) {
       T1(a,&v);
	cout<<"T1: rank "<<rank<<"\t"<<"v "<<v<<endl;
       MPI_Send(&v, 1, MPI_DOUBLE, 1, 111, MPI_COMM_WORLD);
       MPI_Recv(&w, 1, MPI_DOUBLE, 2, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       T6(a,&w);
	cout<<"T6: rank "<<rank<<"\t"<<"w "<<w<<endl;
     } else if (rank==1) {
       T2(b,&w);
	cout<<"T2: rank "<<rank<<"\t"<<"w "<<w<<endl;
       MPI_Send(&w, 1, MPI_DOUBLE, 2, 111, MPI_COMM_WORLD);
       MPI_Recv(&v, 1, MPI_DOUBLE, 0, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       T3(b,&v);
	cout<<"T3: rank "<<rank<<"\t"<<"v "<<v<<endl;
       MPI_Send(&v, 1, MPI_DOUBLE, 2, 111, MPI_COMM_WORLD);
     } else { /* rank==2 */
       MPI_Recv(&w, 1, MPI_DOUBLE, 1, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       T4(c,&w);
	cout<<"T4: rank "<<rank<<"\t"<<"w "<<w<<endl;
       MPI_Send(&w, 1, MPI_DOUBLE, 0, 111, MPI_COMM_WORLD);
       MPI_Recv(&v, 1, MPI_DOUBLE, 1, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       T5(c,&v);
	cout<<"T5: rank "<<rank<<"\t"<<"v "<<v<<endl;
}

MPI_Finalize();  /* salida MPI */
  
}
