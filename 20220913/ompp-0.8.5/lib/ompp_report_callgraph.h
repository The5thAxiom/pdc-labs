/*
 * ompp_report_callgraph.h
 * 
 * The callgraph related portions of ompP's
 * profiling report  */
/* @OMPP_HEADER@ */


#ifndef OMPP_REPORT_CALLGRAPH_INCLUDED
#define OMPP_REPORT_CALLGRAPH_INCLUDED

#include <stdio.h>
#include <assert.h>
#include "ompp.h"
#include "ompp_rstack.h"
#include "ompp_regstat.h"
#include "ompp_region.h"


#define OMPP_MAX_CALLGRAPH_DEPTH 32
/* indentation must not be less than 2 */
#define OMPP_CALLGRAPH_STRFMT "%3s"


/* -- called by the report generator -- */

void print_callgraph( FILE *f, rstack_t *st );

void print_callgraph_profiles( FILE *f, rstack_t *st );


/* -- used internally  -- */

void print_callgraph_header( FILE *f );
void print_callgraph_footer( FILE *f );
void print_callgraph_node( FILE *f, rnode_t *node, int levl );

void print_callgraph_profiles_header( FILE *f );
void print_callgraph_profiles_footer( FILE *f );
void print_callgraph_profiles_node( FILE *f, rnode_t *node, int levl );

void print_flowinfo( FILE *f, rstack_t *st );
void print_flowinfo_levels( FILE *f, rstack_t *st );

#endif /* OMPP_REPORT_CALLGRAPH_INCLUDED */
