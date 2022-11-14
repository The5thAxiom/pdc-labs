

#ifndef OMPP_REPORT_CUBE_H_INCLUDED
#define OMPP_REPORT_CUBE_H_INCLUDED

#define OMPP_CUBE_MAX_REGIONS       200

#define OMPP_CUBE_MET_TIME            (0)
#define OMPP_CUBE_MET_VISITS          (1)
#define OMPP_CUBE_MET_WORK            (2)
#define OMPP_CUBE_MET_OVHDS           (3)
#define OMPP_CUBE_MET_OVHD_SYNC       (4)
#define OMPP_CUBE_MET_OVHD_LIMPAR     (5)
#define OMPP_CUBE_MET_OVHD_IMBAL      (6)
#define OMPP_CUBE_MET_OVHD_MGMT       (7)
#define OMPP_CUBE_MET_MPITIME         (8)
#define OMPP_CUBE_MET_MPIDATAVOL      (9)
#define OMPP_CUBE_MET_MPIDATAIN      (10)
#define OMPP_CUBE_MET_MPIDATAOUT     (11)
#define OMPP_CUBE_MET_MPINCALLS      (12)
#define OMPP_CUBE_MET_MPINSEND       (13)
#define OMPP_CUBE_MET_MPINRECV       (14)
#define OMPP_CUBE_MET_MPINCOLL       (15)

#define OMPP_CUBE_NUM_METRICS        (16)



#define OMPP_CUBE_DEFINE_CNODES         (1)
#define OMPP_CUBE_SET_METRICS        (1<<1)

void ompp_report_cube3( FILE* f, 
			dllist_t *rlist,
			rstack_t *rstack );

#endif /* OMPP_REPORT_CUBE_H_INCLUDED */
