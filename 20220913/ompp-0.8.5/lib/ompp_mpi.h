
#ifndef OMPP_MPI_H_INCLUDED
#define OMPP_MPI_H_INCLUDED

typedef struct
{
  double time;
  unsigned nsend;
  unsigned nrecv;
  unsigned ncoll;
  unsigned bout;
  unsigned bin;
} 
ompp_mpi_t; 

void ompp_mpi_read( ompp_mpi_t *mpi, int tid );

void ompp_mpi_initialize();

/*
#define OMPP_MPI_TIME   (0)
#define OMPP_MPI_NSEND  (1)
#define OMPP_MPI_BSEND  (2)
#define OMPP_MPI_NRECV  (3)
#define OMPP_MPI_BRECV  (4)
*/

#endif /* OMPP_MPI_H_INCLUDED */
