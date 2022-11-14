
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <mpi.h>

#if defined (F_UPPER) && defined (F_SFX_USC)

#define MPI_Init_F        MPI_INIT_
#define MPI_Finalize_F    MPI_FINALIZE_
#define MPI_Comm_rank_F   MPI_COMM_RANK_
#define MPI_Comm_size_F   MPI_COMM_SIZE_

#define MPI_Send_F        MPI_SEND_
#define MPI_Ssend_F       MPI_SSEND_
#define MPI_Rsend_F       MPI_RSEND_
#define MPI_Bsend_F       MPI_BSEND_
#define MPI_Isend_F       MPI_ISEND_
#define MPI_Issend_F      MPI_ISSEND_
#define MPI_Irsend_F      MPI_IRSEND_
#define MPI_Ibsend_F      MPI_IBSEND_

#define MPI_Recv_F              MPI_RECV_
#define MPI_Irecv_F             MPI_IRECV_
#define MPI_Sendrecv_F          MPI_SENDRECV_
#define MPI_Sendrecv_replace_F  MPI_SENDRECV_REPLACE_

#define MPI_Bcast_F       MPI_BCAST_
#define MPI_Barrier_F     MPI_BARRIER_
#define MPI_Gather_F      MPI_GATHER_
#define MPI_Gatherv_F     MPI_GATHERV_
#define MPI_Scatter_F     MPI_SCATTER_
#define MPI_Scatterv_F    MPI_SCATTERV_
#define MPI_Reduce_F      MPI_REDUCE_
#define MPI_Allreduce_F   MPI_ALLREDUCE_
#define MPI_Allgather_F   MPI_ALLGATHER_
#define MPI_Allgatherv_F  MPI_ALLGATHERV_
#define MPI_Alltoall_F    MPI_ALLTOALL_
#define MPI_Alltoallv_F   MPI_ALLTOALLV_

#define MPI_Reduce_scatter_F MPI_REDUCE_SCATTER_
#define MPI_Scan_F           MPI_SCAN_

#define MPI_Wait_F        MPI_WAIT_
#define MPI_Waitall_F     MPI_WAITALL_
#define MPI_Waitany_F     MPI_WAITANY_
#define MPI_Waitsome_F    MPI_WAITSOME_
#define MPI_Test_F        MPI_TEST_
#define MPI_Testall_F     MPI_TESTALL_
#define MPI_Testany_F     MPI_TESTANY_
#define MPI_Testsome_F    MPI_TESTSOME_

#endif /* defined (F_UPPER) && defined (F_SFX_USC) */



#if defined (F_UPPER) && defined (F_SFX_2USC)

#define MPI_Init_F        MPI_INIT__
#define MPI_Finalize_F    MPI_FINALIZE__
#define MPI_Comm_rank_F   MPI_COMM_RANK__
#define MPI_Comm_size_F   MPI_COMM_SIZE__

#define MPI_Send_F        MPI_SEND__
#define MPI_Ssend_F       MPI_SSEND__
#define MPI_Rsend_F       MPI_RSEND__
#define MPI_Bsend_F       MPI_BSEND__
#define MPI_Isend_F       MPI_ISEND__
#define MPI_Issend_F      MPI_ISSEND__
#define MPI_Irsend_F      MPI_IRSEND__
#define MPI_Ibsend_F      MPI_IBSEND__

#define MPI_Recv_F              MPI_RECV__
#define MPI_Irecv_F             MPI_IRECV__
#define MPI_Sendrecv_F          MPI_SENDRECV__
#define MPI_Sendrecv_replace_F  MPI_SENDRECV_REPLACE__

#define MPI_Barrier_F     MPI_BARRIER__
#define MPI_Bcast_F       MPI_BCAST__
#define MPI_Gather_F      MPI_GATHER__
#define MPI_Gatherv_F     MPI_GATHERV__
#define MPI_Scatter_F     MPI_SCATTER__
#define MPI_Scatterv_F    MPI_SCATTERV__
#define MPI_Reduce_F      MPI_REDUCE__
#define MPI_Allreduce_F   MPI_ALLREDUCE__
#define MPI_Allgather_F   MPI_ALLGATHER_
#define MPI_Allgatherv_F  MPI_ALLGATHERV_

#define MPI_Alltoall_F    MPI_ALLTOALL__
#define MPI_Alltoallv_F   MPI_ALLTOALLV__

#define MPI_Reduce_scatter_F MPI_REDUCE_SCATTER__
#define MPI_Scan_F           MPI_SCAN__

#define MPI_Wait_F        MPI_WAIT__
#define MPI_Waitall_F     MPI_WAITALL__
#define MPI_Waitany_F     MPI_WAITANY__
#define MPI_Waitsome_F    MPI_WAITSOME__
#define MPI_Test_F        MPI_TEST__
#define MPI_Testall_F     MPI_TESTALL__
#define MPI_Testany_F     MPI_TESTANY__
#define MPI_Testsome_F    MPI_TESTSOME__



#endif /* defined (F_UPPER) && defined (F_SFX_2USC) */



#if defined (F_LOWER) && defined (F_SFX_USC)

#define MPI_Init_F        mpi_init_
#define MPI_Finalize_F    mpi_finalize_
#define MPI_Comm_rank_F   mpi_comm_rank_
#define MPI_Comm_size_F   mpi_comm_size_

#define MPI_Send_F        mpi_send_
#define MPI_Ssend_F       mpi_ssend_
#define MPI_Rsend_F       mpi_rsend_
#define MPI_Bsend_F       mpi_bsend_
#define MPI_Isend_F       mpi_isend_
#define MPI_Issend_F      mpi_issend_
#define MPI_Irsend_F      mpi_irsend_
#define MPI_Ibsend_F      mpi_ibsend_

#define MPI_Recv_F              mpi_recv_
#define MPI_Irecv_F             mpi_irecv_
#define MPI_Sendrecv_F          mpi_sendrecv_
#define MPI_Sendrecv_replace_F  mpi_sendrecv_replace_

#define MPI_Barrier_F     mpi_barrier_
#define MPI_Bcast_F       mpi_bcast_
#define MPI_Gather_F      mpi_gather_
#define MPI_Gatherv_F     mpi_gatherv_
#define MPI_Scatter_F     mpi_scatter_
#define MPI_Scatterv_F    mpi_scatterv_
#define MPI_Reduce_F      mpi_reduce_
#define MPI_Allreduce_F   mpi_allreduce_
#define MPI_Allgather_F   mpi_allgather_
#define MPI_Allgatherv_F  mpi_allgatherv_

#define MPI_Alltoall_F    mpi_alltoall_
#define MPI_Alltoallv_F   mpi_alltoallv_

#define MPI_Reduce_scatter_F mpi_reduce_scatter_
#define MPI_Scan_F           mpi_scan_

#define MPI_Wait_F        mpi_wait_
#define MPI_Waitall_F     mpi_waitall_
#define MPI_Waitany_F     mpi_waitany_
#define MPI_Waitsome_F    mpi_waitsome_
#define MPI_Test_F        mpi_test_
#define MPI_Testall_F     mpi_testall_
#define MPI_Testany_F     mpi_testany_
#define MPI_Testsome_F    mpi_testsome_


#endif /* defined (F_LOWER) && defined (F_SFX_USC) */



#if defined (F_LOWER) && defined (F_SFX_2USC)

#define MPI_Init_F        mpi_init__
#define MPI_Finalize_F    mpi_finalize__
#define MPI_Comm_rank_F   mpi_comm_rank__
#define MPI_Comm_size_F   mpi_comm_size__

#define MPI_Send_F        mpi_send__
#define MPI_Ssend_F       mpi_ssend__
#define MPI_Rsend_F       mpi_rsend__
#define MPI_Bsend_F       mpi_bsend__
#define MPI_Isend_F       mpi_isend__
#define MPI_Issend_F      mpi_issend__
#define MPI_Irsend_F      mpi_irsend__
#define MPI_Ibsend_F      mpi_ibsend__

#define MPI_Recv_F              mpi_recv__
#define MPI_Irecv_F             mpi_irecv__
#define MPI_Sendrecv_F          mpi_sendrecv__
#define MPI_Sendrecv_replace_F  mpi_sendrecv_replace__

#define MPI_Barrier_F     mpi_barrier__
#define MPI_Bcast_F       mpi_bcast__
#define MPI_Gather_F      mpi_gather__
#define MPI_Gatherv_F     mpi_gatherv__
#define MPI_Scatter_F     mpi_scatter__
#define MPI_Scatterv_F    mpi_scatterv__
#define MPI_Reduce_F      mpi_reduce__
#define MPI_Allreduce_F   mpi_allreduce__
#define MPI_Allgather_F   mpi_allgather__
#define MPI_Allgatherv_F  mpi_allgatherv__

#define MPI_Alltoall_F    mpi_alltoall__
#define MPI_Alltoallv_F   mpi_alltoallv__

#define MPI_Reduce_scatter_F mpi_reduce_scatter__
#define MPI_Scan_F           mpi_scan__

#define MPI_Wait_F        mpi_wait__
#define MPI_Waitall_F     mpi_waitall__
#define MPI_Waitany_F     mpi_waitany__
#define MPI_Waitsome_F    mpi_waitsome__
#define MPI_Test_F        mpi_test__
#define MPI_Testall_F     mpi_testall__
#define MPI_Testany_F     mpi_testany__
#define MPI_Testsome_F    mpi_testsome__

#endif /* defined (F_LOWER) && defined (F_SFX_2USC) */

void MPI_Init_F( int *ierr )
{
  int argc;
  char **argv[32];
  static char *name="./foo";

  argc=1;
  argv[0]=&name;
  MPI_Init(&argc, argv);
}


void MPI_Finalize_F( int *ierr )
{
  *ierr = MPI_Finalize();
}



void MPI_Comm_rank_F(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Comm_rank(MPI_Comm_f2c(*comm),rank);
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Comm_size_F(MPI_Fint *comm, MPI_Fint *osize, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Comm_size(MPI_Comm_f2c(*comm),osize);
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Send_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Send(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*tag,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Ssend_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Ssend(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*tag,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Rsend_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Rsend(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*tag,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Bsend_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Bsend(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*tag,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Isend_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *req, MPI_Fint *info)
{
#if 1    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Isend(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*tag,MPI_Comm_f2c(*comm),&creq);
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 1    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Issend_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *req, MPI_Fint *info)
{
#if 1    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Issend(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*tag,MPI_Comm_f2c(*comm),&creq);
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 1    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Irsend_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *req, MPI_Fint *info)
{
#if 1    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Irsend(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*tag,MPI_Comm_f2c(*comm),&creq);
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 1    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Ibsend_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype,
		  MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, 
		  MPI_Fint *req, MPI_Fint *info)
{
#if 1    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Ibsend(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*tag,MPI_Comm_f2c(*comm),&creq);
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 1    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}



void MPI_Recv_F(void *rbuf, MPI_Fint *rcount, MPI_Fint *rtype, MPI_Fint *src, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 1   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Recv(rbuf,*rcount,MPI_Type_f2c(*rtype),*src,*tag,MPI_Comm_f2c(*comm),&cstat);
  
#if 1   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Irecv_F(void *rbuf, MPI_Fint *rcount, MPI_Fint *rtype, MPI_Fint *src, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *req, MPI_Fint *info)
{
#if 1    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Irecv(rbuf,*rcount,MPI_Type_f2c(*rtype),*src,*tag,MPI_Comm_f2c(*comm),&creq);
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 1    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Sendrecv_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest,   MPI_Fint *stag, void *rbuf, MPI_Fint *rcount, MPI_Fint *rtype, MPI_Fint *src, MPI_Fint *rtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 1   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Sendrecv(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*stag,rbuf,*rcount,MPI_Type_f2c(*rtype),*src,*rtag,MPI_Comm_f2c(*comm),&cstat);
  
#if 1   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Sendrecv_replace_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *dest, MPI_Fint *stag, MPI_Fint *src, MPI_Fint *rtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 1   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Sendrecv_replace(sbuf,*scount,MPI_Type_f2c(*stype),*dest,*stag,*src,*rtag,MPI_Comm_f2c(*comm),&cstat);
  
#if 1   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}



void MPI_Bcast_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Bcast(sbuf,*scount,MPI_Type_f2c(*stype),*root,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Barrier_F(MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Barrier(MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Gather_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, void *rbuf, MPI_Fint *rcount, MPI_Fint *rtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Gather(sbuf,*scount,MPI_Type_f2c(*stype),rbuf,*rcount,MPI_Type_f2c(*rtype),*root,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Gatherv_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, void *rbuf, MPI_Fint *rcounts, MPI_Fint *rdisp, MPI_Fint *rtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Gatherv(sbuf,*scount,MPI_Type_f2c(*stype),rbuf,rcounts,rdisp,MPI_Type_f2c(*rtype),*root,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Scatter_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, void *rbuf, MPI_Fint *rcount, MPI_Fint *rtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Scatter(sbuf,*scount,MPI_Type_f2c(*stype),rbuf,*rcount,MPI_Type_f2c(*rtype),*root,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Scatterv_F(void *sbuf, MPI_Fint *scounts, MPI_Fint *sdisp, MPI_Fint *stype, void *rbuf, MPI_Fint *rcount, MPI_Fint *rtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Scatterv(sbuf,scounts,sdisp,MPI_Type_f2c(*stype),rbuf,*rcount,MPI_Type_f2c(*rtype),*root,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Reduce_F(void *sbuf, void *rbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Reduce(sbuf,rbuf,*scount,MPI_Type_f2c(*stype),MPI_Op_f2c(*op),*root,MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Allreduce_F(void *sbuf, void *rbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Allreduce(sbuf,rbuf,*scount,MPI_Type_f2c(*stype),MPI_Op_f2c(*op),MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}



void MPI_Allgather_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, void *rbuf, MPI_Fint *rcount, MPI_Fint *rtype, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Allgather(sbuf,*scount,MPI_Type_f2c(*stype),rbuf,*rcount,MPI_Type_f2c(*rtype),MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Allgatherv_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, void *rbuf, MPI_Fint *rcounts, MPI_Fint *rdisp, MPI_Fint *rtype, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Allgatherv(sbuf,*scount,MPI_Type_f2c(*stype),rbuf,rcounts,rdisp,MPI_Type_f2c(*rtype),MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}

void MPI_Alltoall_F(void *sbuf, MPI_Fint *scount, MPI_Fint *stype, void *rbuf, MPI_Fint *rcount, MPI_Fint *rtype, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Alltoall(sbuf,*scount,MPI_Type_f2c(*stype),rbuf,*rcount,MPI_Type_f2c(*rtype),MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Alltoallv_F(void *sbuf, MPI_Fint *scounts, MPI_Fint *sdisp, MPI_Fint *stype, void *rbuf, MPI_Fint *rcounts, MPI_Fint *rdisp, MPI_Fint *rtype, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Alltoallv(sbuf,scounts,sdisp,MPI_Type_f2c(*stype),rbuf,rcounts,rdisp,MPI_Type_f2c(*rtype),MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Reduce_scatter_F(void *sbuf, void *rbuf, MPI_Fint *scounts, MPI_Fint *stype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Reduce_scatter(sbuf,rbuf,scounts,MPI_Type_f2c(*stype),MPI_Op_f2c(*op),MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


void MPI_Scan_F(void *sbuf, void *rbuf, MPI_Fint *scount, MPI_Fint *stype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *info)
{
#if 0    /* HAVE _CREQ */ 
  MPI_Request creq; 
#endif
#if 0   /* HAVE _CSTAT */ 
  MPI_Status  cstat; 
#endif

  *info=MPI_Scan(sbuf,rbuf,*scount,MPI_Type_f2c(*stype),MPI_Op_f2c(*op),MPI_Comm_f2c(*comm));
  
#if 0   /* HAVE _CSTAT */ 
  if (*info==MPI_SUCCESS) 
    MPI_Status_c2f(&cstat, status);
#endif

#if 0    /* HAVE _CREQ */ 
  if( *info==MPI_SUCCESS )
    *req=MPI_Request_c2f(creq);
#endif
}


static MPI_Request* alloc_requests(int count) 
{
  static MPI_Request *req_array = 0;
  static int req_array_size = 0;
  
  if (req_array_size == 0) 
    {
      req_array = (MPI_Request*) malloc(count * sizeof(MPI_Request));
      req_array_size = count;
    }
  else if (count > req_array_size) 
    {
      req_array = (MPI_Request*) realloc(req_array, count * sizeof(MPI_Request));
      req_array_size = count;
    }

  return req_array;
}


static MPI_Status* alloc_statuses(int count) 
{
  static MPI_Status *stat_array = 0;
  static int stat_array_size = 0;

  if (stat_array_size == 0) 
    {
      /* -- never used: initialize -- */
      stat_array = (MPI_Status*) malloc(count * sizeof(MPI_Status));
      stat_array_size = count;
    } 
  else if (count > stat_array_size) 
    {
      /* -- not enough room: expand -- */
      stat_array = (MPI_Status*) realloc(stat_array, count * sizeof(MPI_Status));
      stat_array_size = count;
    }
  return stat_array;
}



void MPI_Test_F(MPI_Fint *request, MPI_Fint *flag,
		MPI_Fint *status, MPI_Fint *info)
{
  MPI_Request creq;
  MPI_Status  cstat;

  creq = MPI_Request_f2c(*request);
  *info = MPI_Test(&creq, flag, &cstat);
  if( *info != MPI_SUCCESS ) return;
			       
  *request = MPI_Request_c2f(creq);

  if(flag) 
    MPI_Status_c2f(&cstat, status);
}

void MPI_Wait_F(MPI_Fint *request, 
		MPI_Fint *status, MPI_Fint *info)
{
  MPI_Request creq;
  MPI_Status  cstat;

  creq = MPI_Request_f2c(*request);
  *info = MPI_Wait(&creq, &cstat);
  *request = MPI_Request_c2f(creq);

  if(*info==MPI_SUCCESS )
    MPI_Status_c2f(&cstat, status);
}

void MPI_Testany_F(MPI_Fint *num, MPI_Fint *requests, 
		   MPI_Fint *indx, MPI_Fint *flag, 
		   MPI_Fint *status, MPI_Fint *info)
{
  int i;
  MPI_Request *creq;
  MPI_Status cstat;

  if(*num > 0 ) {
    creq = alloc_requests(*num);

    for( i=0; i<*num; i++ ) {
      creq[i] = MPI_Request_f2c(requests[i]);
    }
  }

  *info = MPI_Testany(*num, creq, indx, flag, &cstat);

  if( *info==MPI_SUCCESS )
    {
      if( *flag && *indx>=0 ) {
	requests[*indx] = MPI_Request_c2f(creq[*indx]);
	(*indx)++;
      }
      MPI_Status_c2f(&cstat, status);
    }
}

void MPI_Waitany_F(MPI_Fint *num, 
		   MPI_Fint *requests, MPI_Fint *indx, 
		   MPI_Fint *status, MPI_Fint *info)
{
  int i;
  MPI_Request *creq;
  MPI_Status cstat;
  
  if(*num > 0 ) {
    creq = alloc_requests(*num);

    for( i=0; i<*num; i++ ) {
      creq[i] = MPI_Request_f2c(requests[i]);
    }
  }

  *info = MPI_Waitany(*info, creq, indx, &cstat);

  if( *info==MPI_SUCCESS )
    {
      if( *indx>=0 ) {
	requests[*indx] = MPI_Request_c2f(creq[*indx]);
	(*indx)++;
      }
      MPI_Status_c2f(&cstat, status);
    }
}


void MPI_Testall_F(MPI_Fint *num, MPI_Fint *requests, 
		   MPI_Fint *flag, MPI_Fint *statuses, 
		   MPI_Fint *info)
{
  int i;
  MPI_Request *creq;
  MPI_Status *cstat;

  if(*num > 0 ) {
    creq = alloc_requests(*num);
    cstat = alloc_statuses(*num);

    for( i=0; i<*num; i++ ) {
      creq[i] = MPI_Request_f2c(requests[i]);
    }
  }

  *info = MPI_Testall(*num, creq, flag, cstat);

  for( i=0; i<*num; i++ ) {
    requests[i] = MPI_Request_c2f(creq[i]);
  }
  if( *info==MPI_SUCCESS && *flag ) {
    for( i=0; i<*num; i++ ) {
      MPI_Status_c2f( &(cstat[i]), &(statuses[i]));
    } 
  }  
}

void MPI_Waitall_F(MPI_Fint *num, 
		   MPI_Fint *requests, MPI_Fint *statuses, 
		   MPI_Fint *info)
{
  int i;
  MPI_Request *creq;
  MPI_Status *cstat;

  if(*num > 0 ) {
    creq = alloc_requests(*num);
    cstat = alloc_statuses(*num);

    for( i=0; i<*num; i++ ) {
      creq[i] = MPI_Request_f2c(requests[i]);
    }
  }

  *info = MPI_Waitall(*num, creq, cstat);
  
  for( i=0; i<*num; i++ ) {
    requests[i] = MPI_Request_c2f(creq[i]);
  }
  if( *info==MPI_SUCCESS ) {
    for( i=0; i<*num; i++ ) {
      MPI_Status_c2f( &(cstat[i]), &(statuses[i]));
    } 
  }  
}


void MPI_Testsome_F(MPI_Fint *inum, MPI_Fint *requests, 
		    MPI_Fint *onum, MPI_Fint *indices, 
		    MPI_Fint *statuses, MPI_Fint *info)
{
  int i, j, found;
  MPI_Request *creq;
  MPI_Status *cstat;

  if(*inum > 0 ) {
    creq = alloc_requests(*inum);
    cstat = alloc_statuses(*inum);

    for( i=0; i<*inum; i++ ) {
      creq[i] = MPI_Request_f2c(requests[i]);
    }
  }
  
  *info = MPI_Testsome(*inum, creq, onum, indices, cstat);
  
  if( *info==MPI_SUCCESS )
    {
      for (i=0; i<*inum; i++) {
	if (i < *onum) {
	  requests[indices[i]] = MPI_Request_c2f(creq[indices[i]]);
	} else {
	  found = j = 0;
	  while ( (!found) && (j<*onum) ) {
	    if (indices[j++] == i) found = 1;
	  }
	  if (!found) requests[i] = MPI_Request_c2f(creq[i]);
	}
      }

      for (i=0; i<*onum; i++) {
	MPI_Status_c2f(&cstat[i], &(statuses[i]));
	/* See the description of testsome in the standard;
	   the Fortran index ranges are from 1, not zero */
	if (indices[i] >= 0) indices[i]++;
      }
    }
}


void MPI_Waitsome_F(MPI_Fint *inum, MPI_Fint *requests, 
		    MPI_Fint *onum, MPI_Fint *indices, 
		    MPI_Fint *statuses, MPI_Fint *info)
{
  int i, j, found;
  MPI_Request *creq;
  MPI_Status *cstat;

  if(*inum > 0 ) {
    creq = alloc_requests(*inum);
    cstat = alloc_statuses(*inum);

    for( i=0; i<*inum; i++ ) {
      creq[i] = MPI_Request_f2c(requests[i]);
    }
  }
  
  *info = MPI_Waitsome(*inum, creq, onum, indices, cstat);

  if( *info==MPI_SUCCESS )
    {
      for (i=0; i<*inum; i++) {
	if (i < *onum) {
	  requests[indices[i]] = MPI_Request_c2f(creq[indices[i]]);
	} else {
	  found = j = 0;
	  while ( (!found) && (j<*onum) ) {
	    if (indices[j++] == i) found = 1;
	  }
	  if (!found) requests[i] = MPI_Request_c2f(creq[i]);
	}
      }

      for (i=0; i<*onum; i++) {
	MPI_Status_c2f(&cstat[i], &(statuses[i]));
	/* See the description of testsome in the standard;
	   the Fortran index ranges are from 1, not zero */
	if (indices[i] >= 0) indices[i]++;
      }
    }
}
