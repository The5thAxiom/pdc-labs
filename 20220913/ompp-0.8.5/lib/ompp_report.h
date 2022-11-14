/*
 * ompp_report.h  */
/* @OMPP_HEADER@ */

#ifndef OMPP_REPORT_H_INCLUDED
#define OMPP_REPORT_H_INCLUDED

#include "utils/list.h"
#include "ompp_overheads.h"
#include "ompp_properties.h"

void print_report(void);

void print_header( FILE *f );

void print_region_list( FILE *f,
			dllist_t * list );

void print_region_stats( FILE *f,
			 dllist_t * list );

void print_overhead_report( FILE *f,
			    overheads_t *ovhds, unsigned npara );

void print_properties_report( FILE *f,
			      ompp_perfprop_t *props, unsigned nprop );




#endif // OMPP_REPORT_H_INCLUDED
