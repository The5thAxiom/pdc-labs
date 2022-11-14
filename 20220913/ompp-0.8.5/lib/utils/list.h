/* 
 * list.h  */
/* @OMPP_HEADER@ */


#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>

typedef struct lelem
{
  struct lelem *next;
  struct lelem *prev;
  
  /* the actual data payload */
  void *data;
}
lelem_t;


typedef struct
{
  lelem_t *head;
  lelem_t *tail;
  size_t  size;
}
dllist_t;


void list_init( dllist_t *l );

void list_add_back( dllist_t *l, lelem_t *elem );

void list_add_front( dllist_t *l, lelem_t *elem );

void list_add_after( dllist_t *l, lelem_t *pos, lelem_t *elem );

lelem_t* list_pop_front( dllist_t *l );

lelem_t* list_pop_back( dllist_t *l );

void list_remove( dllist_t *l, lelem_t *elem );


#endif /* LIST_H_INLCLUDED */
