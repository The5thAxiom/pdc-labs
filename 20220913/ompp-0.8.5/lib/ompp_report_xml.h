
#ifndef OMPP_REPORT_XML_H_INCLUDED
#define OMPP_REPORT_XML_H_INCLUDED

#define OMPP_XML_MET_TIME            (0)
#define OMPP_XML_MET_VISITS          (1)
#define OMPP_XML_MET_WORK            (2)
#define OMPP_XML_MET_OVHDS           (3)
#define OMPP_XML_MET_OVHD_SYNC       (4)
#define OMPP_XML_MET_OVHD_LIMPAR     (5)
#define OMPP_XML_MET_OVHD_IMBAL      (6)
#define OMPP_XML_MET_OVHD_MGMT       (7)
#define OMPP_XML_MET_MPITIME         (8)
#define OMPP_XML_MET_MPIDATAVOL      (9)
#define OMPP_XML_MET_MPIDATAIN      (10)
#define OMPP_XML_MET_MPIDATAOUT     (11)
#define OMPP_XML_MET_MPINCALLS      (12)
#define OMPP_XML_MET_MPINSEND       (13)
#define OMPP_XML_MET_MPINRECV       (14)
#define OMPP_XML_MET_MPINCOLL       (15)


void ompp_report_xml( FILE* f, 
		      dllist_t *rlist,
		      rstack_t *rstack );

#endif /* OMPP_REPORT_XML_H_INCLUDED */
