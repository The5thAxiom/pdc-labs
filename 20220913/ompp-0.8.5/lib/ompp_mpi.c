/*
 * ompp_mpi.c */ 
/* @OMPP_HEADER@ */

#include <stdio.h>
#include <mpi.h>

#include "ompp.h"
#include "ompp_time.h"
#include "ompp_mpi.h"

int ompp_mpi_myrank;
int ompp_mpi_nprocs;


int     *ompp_mpi_bin;    /* number of bytes received P2P + collective*/
int     *ompp_mpi_bout;   /* number of bytes sent P2P + collective */
int     *ompp_mpi_nsend;  /* number of send calls */
int     *ompp_mpi_nrecv;  /* number of recv calls */
int     *ompp_mpi_ncoll;  /* number of collective calls */
double  *ompp_mpi_time;   /* time spent in MPI routines */


void ompp_mpi_initialize()
{ 
  int i;

  ompp_mpi_myrank=0;
  ompp_mpi_nprocs=0;

  ompp_mpi_bin   = (int*) malloc( sizeof(int)*ompp_max_threads );
  ompp_mpi_bout  = (int*) malloc( sizeof(int)*ompp_max_threads );
  ompp_mpi_nsend = (int*) malloc( sizeof(int)*ompp_max_threads );
  ompp_mpi_nrecv = (int*) malloc( sizeof(int)*ompp_max_threads );
  ompp_mpi_ncoll = (int*) malloc( sizeof(int)*ompp_max_threads );

  ompp_mpi_time = (double*) malloc( sizeof(double)*ompp_max_threads );

  for( i=0; i<ompp_max_threads; i++ )
    {
      ompp_mpi_bin[i]  = 0;
      ompp_mpi_bout[i] = 0;
      ompp_mpi_nsend[i] = 0;
      ompp_mpi_nrecv[i] = 0;
      ompp_mpi_ncoll[i] = 0;
      ompp_mpi_time[i]  = 0.0;
    }
}


void ompp_mpi_read( ompp_mpi_t *mpi, int tid )
{
  mpi->bin   = ompp_mpi_bin[tid];
  mpi->bout  = ompp_mpi_bout[tid];
  mpi->nsend = ompp_mpi_nsend[tid];
  mpi->nrecv = ompp_mpi_nrecv[tid];
  mpi->ncoll = ompp_mpi_ncoll[tid];
  mpi->time  = ompp_mpi_time[tid];
}


int MPI_Init(int *argc, char **argv[])
{
  int res;
  int tid;
  double tstart, tend;

  if( !(OMPP_IS_INITIALIZED) )
    {
      ompp_initialize();
    }
  
  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);

  res = PMPI_Init(argc, argv);

  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);

  PMPI_Comm_size( MPI_COMM_WORLD, &ompp_mpi_nprocs );
  PMPI_Comm_rank( MPI_COMM_WORLD, &ompp_mpi_myrank );

  return res;
}


int MPI_Finalize() 
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Finalize();
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}


int MPI_Send( void* buf,
	      int   count,
	      MPI_Datatype datatype,
	      int   dest,
	      int   tag,
	      MPI_Comm comm )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Send(buf, count, datatype, dest, tag, comm);
  OMPP_TIMESTAMP(tend);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  ompp_mpi_nsend[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}

int MPI_Isend( void* buf,
	       int   count,
	       MPI_Datatype datatype,
	       int   dest,
	       int   tag,
	       MPI_Comm comm,
	       MPI_Request *req)
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Isend(buf, count, datatype, dest, tag, comm, req);
  OMPP_TIMESTAMP(tend);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  ompp_mpi_nsend[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}



int MPI_Ssend( void* buf,
	      int   count,
	      MPI_Datatype datatype,
	      int   dest,
	      int   tag,
	      MPI_Comm comm )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);  
  res = PMPI_Ssend(buf, count, datatype, dest, tag, comm);
  OMPP_TIMESTAMP(tend);    

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  ompp_mpi_nsend[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);  

  return res;
}


int MPI_Bsend( void *buf, int count, 
	       MPI_Datatype datatype, int dest, 
	       int tag, MPI_Comm comm )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);  
  res = PMPI_Bsend(buf, count, datatype, dest, tag, comm);
  OMPP_TIMESTAMP(tend);    

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  ompp_mpi_nsend[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);    

  return res; 
}

int MPI_Rsend( void *buf, int count, 
	       MPI_Datatype datatype, int dest, 
               int tag, MPI_Comm comm )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);  
  res = PMPI_Rsend(buf, count, datatype, dest, tag, comm);
  OMPP_TIMESTAMP(tend);    

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  ompp_mpi_nsend[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}




int MPI_Ibsend( void *buf, int count, MPI_Datatype datatype, 
		int dest, int tag,
		MPI_Comm comm, MPI_Request *req )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Ibsend(buf, count, datatype, dest, tag, comm, req);
  OMPP_TIMESTAMP(tend);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  ompp_mpi_nsend[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}

int MPI_Issend( void *buf, int count, MPI_Datatype datatype, 
		int dest, int tag, 
		MPI_Comm comm, MPI_Request *req )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Issend(buf, count, datatype, dest, tag, comm, req);
  OMPP_TIMESTAMP(tend);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  ompp_mpi_nsend[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}

int MPI_Irsend( void *buf, int count, 
		MPI_Datatype datatype, int dest,
		int tag, MPI_Comm comm, MPI_Request *req )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Irsend(buf, count, datatype, dest, tag, comm, req);
  OMPP_TIMESTAMP(tend);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  ompp_mpi_nsend[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}



int MPI_Recv( void* buf, 
	      int   count, 
	      MPI_Datatype datatype, 
	      int   source,
	      int   tag, 
	      MPI_Comm comm, 
	      MPI_Status* status )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res =  PMPI_Recv(buf, count, datatype, source, tag, comm, status);
  OMPP_TIMESTAMP(tend);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;
  
  ompp_mpi_nrecv[tid]++;
  ompp_mpi_bin[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}


int MPI_Irecv( void* buf, 
	       int   count, 
	       MPI_Datatype datatype, 
	       int   source,
	       int   tag, 
	       MPI_Comm comm, 
	       MPI_Request* request )
{
  int dsize; 
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res =  PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
  OMPP_TIMESTAMP(tend);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;
  
  ompp_mpi_nrecv[tid]++;
  ompp_mpi_bin[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}


int MPI_Probe( int source, int tag, 
	       MPI_Comm comm, MPI_Status *status )
{
  int res; 
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Probe( source, tag, comm, status );
  OMPP_TIMESTAMP(tend);
  
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;  
}


int MPI_Barrier( MPI_Comm comm )
{
  int res; 
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Barrier( comm );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_ncoll[tid]++;
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;  
}


int MPI_Bcast( void *buf,
               int count,
               MPI_Datatype dtype,
               int root,
               MPI_Comm comm )
{
  int res;
  int dsize;
  int tid;
  int myrank;
  int commsize;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Bcast( buf, count, dtype, root, comm );
  OMPP_TIMESTAMP(tend);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  PMPI_Type_size(dtype, &dsize);
  dsize *= count;

  if( myrank==root )
    {
      ompp_mpi_bout[tid]+=dsize*(commsize-1);
    }
  else
    {
      ompp_mpi_bin[tid]+=dsize;
    }

  ompp_mpi_ncoll[tid]++;
  ompp_mpi_time[tid]+=(tend-tstart);
  return res;
}


int MPI_Scatter( void *sendbuf, int sendcnt,
		 MPI_Datatype sendtype,
		 void *recvbuf,
		 int recvcnt,
		 MPI_Datatype recvtype,
		 int root,
		 MPI_Comm comm )
{
  int res;
  int dsize;
  int myrank;
  int commsize;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Scatter( sendbuf, sendcnt, sendtype,
		      recvbuf, recvcnt, recvtype,
		      root, comm );
  OMPP_TIMESTAMP(tend);

  if( myrank==root )
    {
      PMPI_Type_size(sendtype, &dsize);
      dsize *= sendcnt;

      /* send to all, including self */
      ompp_mpi_bout[tid]+=dsize*commsize;
    }
  
  /* all receive, including root */
  PMPI_Type_size(recvtype, &dsize);
  dsize *= recvcnt;

  ompp_mpi_bin[tid]+=dsize;
  
  ompp_mpi_ncoll[tid]++;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}


int MPI_Scatterv(  void *sendbuf, int *sendcnts, 
		   int *displs, MPI_Datatype sendtype, 
		   void *recvbuf, int recvcnt,  
		   MPI_Datatype recvtype, 
		   int root, 
		   MPI_Comm comm )
{
  int i;
  int res;
  int dsize;
  int myrank;
  int commsize;
  int tid;
  int scount;
  double tstart, tend;

  
  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Scatterv( sendbuf, sendcnts, displs,
		       sendtype, recvbuf, recvcnt, 
		       recvtype, root, comm );
  OMPP_TIMESTAMP(tend);

  if( myrank==root )
    {
      PMPI_Type_size(sendtype, &dsize);
      
      scount=0;
      for( i=0; i<commsize; i++ )
	{
	  scount+=sendcnts[i];
	}
      dsize *= scount;

      /* send to all, including self */
      ompp_mpi_bout[tid]+=dsize;
    }
  
  /* all recv, including root */
  PMPI_Type_size(recvtype, &dsize);
  dsize *= recvcnt;
  
  ompp_mpi_ncoll[tid]++;
  ompp_mpi_bin[tid]+=dsize;
  
  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}


int MPI_Gather( void *sendbuf,
		int sendcnt,
		MPI_Datatype sendtype,
		void *recvbuf,
		int recvcnt,
		MPI_Datatype recvtype,
		int root,
		MPI_Comm comm )
{
  int res;
  int dsize;
  int myrank;
  int commsize;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Gather( sendbuf, sendcnt, sendtype,
		     recvbuf, recvcnt, recvtype,
		     root, comm );
  OMPP_TIMESTAMP(tend);

  if( myrank==root )
    {
      PMPI_Type_size(recvtype, &dsize);
      dsize *= recvcnt;

      /* receive from all, including self */
      ompp_mpi_bin[tid]+=dsize*commsize;
    }

  /* all send, including root */
  PMPI_Type_size(sendtype, &dsize);
  dsize *= sendcnt;
  
  ompp_mpi_ncoll[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}



int MPI_Gatherv( void *sendbuf, int sendcnt,
		 MPI_Datatype sendtype,
		 void *recvbuf, int *recvcnts,
		 int *displs, MPI_Datatype recvtype,
		 int root, MPI_Comm comm )
{
  int i;
  int res;
  int dsize;
  int rcount;
  int myrank;
  int commsize;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Gatherv( sendbuf, sendcnt, sendtype,
		      recvbuf, recvcnts, displs, 
		      recvtype, root, comm );
  OMPP_TIMESTAMP(tend);

  if( myrank==root )
    {
      PMPI_Type_size(recvtype, &dsize);

      rcount=0;
      for( i=0; i<commsize; i++ )
	{
	  rcount+=recvcnts[i];
	}
      dsize *= rcount;

      /* receive from all, including self */
      ompp_mpi_bin[tid]+=dsize;
    }

  /* all send, including root */
  PMPI_Type_size(sendtype, &dsize);
  dsize *= sendcnt;
  
  ompp_mpi_ncoll[tid]++;
  ompp_mpi_bout[tid]+=dsize;
  
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}

int MPI_Allgatherv( void *sendbuf, int sendcnt,
		    MPI_Datatype sendtype,
		    void *recvbuf, int *recvcnts,
		    int *displs, MPI_Datatype recvtype,
		    MPI_Comm comm )
{
  int i;
  int res;
  int dsize;
  int rcount;
  int myrank;
  int commsize;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Allgatherv( sendbuf, sendcnt, sendtype,
			 recvbuf, recvcnts, displs, 
			 recvtype, comm );
  OMPP_TIMESTAMP(tend);

  
  PMPI_Type_size(recvtype, &dsize);
  rcount=0;
  for( i=0; i<commsize; i++ )
    {
      rcount+=recvcnts[i];
    }
  dsize *= rcount;
  /* receive from all, including self */
  ompp_mpi_bin[tid]+=dsize;
  

  /* all send, including root */
  PMPI_Type_size(sendtype, &dsize);
  dsize *= sendcnt;
  
  ompp_mpi_ncoll[tid]++;
  ompp_mpi_bout[tid]+=dsize*commsize;
  
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}


int MPI_Allgather( void *sendbuf, int sendcnt, 
		   MPI_Datatype sendtype,
		   void *recvbuf, int recvcnt,
		   MPI_Datatype recvtype, 
                   MPI_Comm comm )
{
  int res;
  int ssize;
  int rsize;
  int myrank;
  int commsize;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Allgather( sendbuf, sendcnt, sendtype,
			recvbuf, recvcnt, recvtype,
			comm );
  OMPP_TIMESTAMP(tend);

  
  PMPI_Type_size(recvtype, &rsize);
  rsize *= recvcnt;
  
  ompp_mpi_bin[tid]+=rsize*commsize;
   
  PMPI_Type_size(sendtype, &ssize);
  ssize *= sendcnt;
  
  ompp_mpi_ncoll[tid]++;
  ompp_mpi_bout[tid]+=ssize*commsize;
  
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}





int MPI_Reduce( void *sendbuf,
		void *recvbuf,
		int count,
		MPI_Datatype datatype,
		MPI_Op op,
		int root,
		MPI_Comm comm )
{
  int res;
  int dsize;
  int tid;
  int myrank;
  int commsize;
  double tstart, tend;

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Reduce( sendbuf, recvbuf,
		     count, datatype, op, 
		     root, comm );
  OMPP_TIMESTAMP(tend);

  /* assume root receives from all others */
  /* all others send to root */
  if( myrank==root )
    {
      ompp_mpi_bin[tid]+=dsize*(commsize-1);  
    }
  else
    {
      ompp_mpi_bout[tid]+=dsize;  
    }

  ompp_mpi_ncoll[tid]++;
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}


int MPI_Reduce_scatter ( void *sbuf, void *rbuf, 
			 int *rcnts, MPI_Datatype dtype, 
			 MPI_Op op, MPI_Comm comm )
{
  int res;
  int tid;
  int myrank;
  int commsize;
  double tstart, tend;
  int dsize, rvol, svol;
  int i;

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Reduce_scatter( sbuf, rbuf,
			     rcnts, dtype, op, comm );
  OMPP_TIMESTAMP(tend);

  /* assume process i is the root for rcnts[i] 
     reduction operations and receives the data from 
     all other processes */
  
  PMPI_Type_size(dtype, &dsize);
  svol=rvol=0;
  for( i=0; i<commsize; i++ )
    {
      if( i==myrank )
	{
	  /* receive from all but not myself */
	  rvol = rcnts[i]*dsize*(commsize-1);
	}
      else
	{
	  /* send to all but not myself */
	  svol += rcnts[i]*dsize;
	}
    }

  ompp_mpi_bout[tid]+=svol;  
  ompp_mpi_bin[tid]+=rvol;  

  ompp_mpi_ncoll[tid]++;
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}



int MPI_Scan( void *sendbuf, void *recvbuf, 
	      int count, MPI_Datatype datatype,
	      MPI_Op op, MPI_Comm comm )
{
  int res;
  int dsize;
  int tid;
  int myrank;
  int commsize;
  double tstart, tend;

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Scan( sendbuf, recvbuf,
		   count, datatype, op, comm );
  OMPP_TIMESTAMP(tend);

  if( myrank>0 )
    {
      ompp_mpi_bin[tid]+=dsize;  
    }
  if( myrank<(commsize-1) )
    {
      ompp_mpi_bout[tid]+=dsize;  
    }

  ompp_mpi_ncoll[tid]++;  
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;  
}




int MPI_Allreduce( void *sendbuf,
		   void *recvbuf,
		   int count,
		   MPI_Datatype datatype,
		   MPI_Op op,
		   MPI_Comm comm )
{
  int res;
  int dsize;
  int tid;
  int myrank;
  int commsize;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Allreduce( sendbuf, recvbuf,
			count, datatype, op, 
			comm );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_bin[tid]+=dsize*(commsize-1);
  ompp_mpi_bout[tid]+=dsize*(commsize-1);

  ompp_mpi_ncoll[tid]++;

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}


int MPI_Alltoall( void *sendbuf,
		  int sendcount,
		  MPI_Datatype sendtype,
		  void *recvbuf,
		  int recvcount,
		  MPI_Datatype recvtype,
		  MPI_Comm comm )
{
  int res;
  int dsize, ssize, rsize;
  int myrank;
  int commsize;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  PMPI_Type_size(sendtype, &ssize);
  ssize *= sendcount;
  ssize *= commsize;

  PMPI_Type_size(recvtype, &rsize);
  rsize *= recvcount;
  rsize *= commsize;


  OMPP_TIMESTAMP(tstart);
  res = PMPI_Alltoall( sendbuf, sendcount, sendtype,
		       recvbuf, recvcount, recvtype,
		       comm );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_bin[tid]+=ssize;
  ompp_mpi_bout[tid]+=ssize;

  ompp_mpi_ncoll[tid]++;

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}


int MPI_Alltoallv( void *sbuf,
		   int *scounts,
		   int *sdisps,
		   MPI_Datatype stype,
		   void *rbuf,
		   int *rcounts,
		   int *rdisps,
		   MPI_Datatype rtype,
		   MPI_Comm comm )
{
  int res;
  int i;
  int dsize, ssize, rsize;
  int rcount, scount;
  int myrank;
  int commsize;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Comm_rank(comm, &myrank);
  PMPI_Comm_size(comm, &commsize);

  PMPI_Type_size(stype, &ssize);
  PMPI_Type_size(rtype, &rsize);
 
  rcount=0; scount=0;
  for( i=0; i<commsize; i++ )
    {
      rcount+=rcounts[i];
      scount+=scounts[i];
    }
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Alltoallv( sbuf, scounts, sdisps, stype,
			rbuf, rcounts, rdisps, rtype,
			comm );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_bin[tid]+=rcount*rsize;
  ompp_mpi_bout[tid]+=scount*ssize;
  ompp_mpi_ncoll[tid]++;

  ompp_mpi_time[tid]+=(tend-tstart);
 
  return res;
}


int MPI_Wait ( MPI_Request  *request,
	       MPI_Status   *status )
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Wait( request, status );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);

  return res;  
}


int MPI_Waitall( int count,
		 MPI_Request requests[],
		 MPI_Status statuses[] )
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Waitall( count, requests, statuses );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}


int MPI_Waitany( int count,
		 MPI_Request requests[],
		 int *index,
		 MPI_Status *status )
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Waitany( count, requests, index, status );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}


int MPI_Waitsome( int incount, 
		  MPI_Request requests[], 
		  int *outcount,
		  int indices[],
		  MPI_Status statuses[])
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Waitsome( incount, requests, outcount, indices, statuses );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}


int MPI_Sendrecv ( void *sendbuf, int sendcount, MPI_Datatype sendtype, 
		   int dest, int sendtag, 
		   void *recvbuf, int recvcount, MPI_Datatype recvtype, 
		   int source, int recvtag, 
		   MPI_Comm comm, MPI_Status *status )
{
  int res;
  int dsize, ssize, rsize;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Type_size(sendtype, &ssize);
  ssize *= sendcount;

  PMPI_Type_size(recvtype, &rsize);
  rsize *= recvcount;


  OMPP_TIMESTAMP(tstart);
  res = PMPI_Sendrecv( sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, 
		       recvcount, recvtype, source, recvtag, comm, status );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_nrecv[tid]++;
  ompp_mpi_nsend[tid]++;
  ompp_mpi_bin[tid]+=rsize;
  ompp_mpi_bout[tid]+=ssize;
  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}

int MPI_Sendrecv_replace( void *buf, int count, 
			  MPI_Datatype datatype, 
			  int dest, int sendtag, 
			  int source, int recvtag, 
			  MPI_Comm comm, 
			  MPI_Status *status )
{
  int res;
  int dsize;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  PMPI_Type_size(datatype, &dsize);
  dsize *= count;

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Sendrecv_replace( buf, count, datatype, dest,
			       sendtag, source, recvtag, 
			       comm, status );
  OMPP_TIMESTAMP(tend);


  ompp_mpi_nrecv[tid]++;
  ompp_mpi_nsend[tid]++;
  ompp_mpi_bin[tid]+=dsize;
  ompp_mpi_bout[tid]+=dsize;
  ompp_mpi_time[tid]+=(tend-tstart);

  return res; 
}


int MPI_Pack( void *inbuf, int incount, 
	      MPI_Datatype datatype,
	      void *outbuf, int outcount,
	      int *position, MPI_Comm comm )
{
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Pack( inbuf, incount, datatype, outbuf, outcount, position, comm );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}


int MPI_Unpack( void *inbuf, int insize, 
		int *position, void *outbuf, 
		int outcount, MPI_Datatype datatype, 
                MPI_Comm comm )
{
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Unpack( inbuf, insize, position, outbuf, outcount, datatype, comm );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}

int MPI_Comm_rank(MPI_Comm comm, int *rank)
{
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Comm_rank(comm, rank);
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;  
}


int MPI_Comm_size(MPI_Comm comm, int *size)
{
  int res;
  int tid;
  double tstart, tend;
  
  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Comm_size(comm, size);
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;  
}


int MPI_Test ( MPI_Request  *request,
	       int          *flag,
	       MPI_Status   *status )
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);

  OMPP_TIMESTAMP(tstart);
  res = PMPI_Test( request, flag, status );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);

  return res;  
}


int MPI_Testall( int count,
		 MPI_Request requests[],
		 int *flag,
		 MPI_Status statuses[] )
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Testall( count, requests, flag, statuses );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);

  return res;
}


int MPI_Testany( int count,
		 MPI_Request requests[],
		 int *index,
		 int *flag,
		 MPI_Status *status )
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Testany( count, requests, index, flag, status );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}


int MPI_Testsome( int incount, 
		  MPI_Request requests[], 
		  int *outcount,
		  int indices[],
		  MPI_Status statuses[])
{
  int res;
  int tid;
  double tstart, tend;

  OMPP_GET_THREAD_NUM(tid);
  
  OMPP_TIMESTAMP(tstart);
  res = PMPI_Testsome( incount, requests, outcount, indices, statuses );
  OMPP_TIMESTAMP(tend);

  ompp_mpi_time[tid]+=(tend-tstart);
  
  return res;
}
