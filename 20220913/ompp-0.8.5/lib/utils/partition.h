/* 
 * partition.h */
/* @OMPP_HEADER@ */


#ifndef PARTITION_H_INCLUDED
#define PARTITION_H_INCLUDED

int find_partitions( int *value, int *partition, int n );
int format_partition( char *str, int *partition, int part, int n );
int get_representative( int *partition, int part, int n );

#endif /* PARTITION_H_INCLUDED */
