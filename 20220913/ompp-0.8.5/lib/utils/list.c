/* 
 * list.c  */
/* @OMPP_HEADER@ */

#include "list.h"

void list_init( dllist_t *l )
{
  l->head=0; 
  l->tail=0;
  l->size=0;
}


void list_add_back( dllist_t *l, lelem_t *elem )
{
  lelem_t *temp;
  
  if( l->size==0 )
    {
      l->head=elem;
      l->tail=elem;
      elem->next=0;
      elem->prev=0;
    }
  else
    {
      temp=l->tail;
      
      l->tail=elem;
      elem->next=0;
      elem->prev=temp;
      temp->next=elem;
    }
  
  ++(l->size);
}


void list_add_front( dllist_t *l, lelem_t *elem )
{
  lelem_t *temp;
  
  if( l->size==0 )
    {
      l->head=elem;
      l->tail=elem;
      elem->next=0;
      elem->prev=0;
    }
  else
    {
      temp=l->head;

      l->head=elem;
      elem->next=temp;
      elem->prev=0;
      temp->prev=elem;
    }

  ++(l->size);
}


void list_add_after( dllist_t *l, lelem_t *pos, lelem_t *elem )
{
  lelem_t *temp;
  
  if( pos==l->tail )
    {
      list_add_back(l, elem);
      return;
    }
  
  temp=pos->next;

  pos->next=elem;
  elem->next=temp;
  elem->prev=pos;
  
  ++(l->size);
}


void list_remove( dllist_t *l, lelem_t *elem )
{
  lelem_t *p;

  if( elem==0 )
    return;
  
  p=elem->prev;
  
  if( p!=0 )
    {
      p->next=elem->next;
    }
  
  if( elem->next!=0 )
    {
      (elem->next)->prev=p;
    }
  
  if( l->head==elem )
    {
      l->head=elem->next;
    }
  
  if( l->tail==elem )
    {
      l->tail=elem->prev;
    }
  
  --(l->size);
}


lelem_t* list_pop_front( dllist_t *l )
{
  lelem_t *elem;
  
  if( l->size==0 )
    {
      return 0;
    }
  
  elem=l->head;
  
  l->head=elem->next;
  if( l->head==0 )
    l->tail=0;

  --(l->size);
  
  return elem;
}


lelem_t* list_pop_back( dllist_t *l )
{
  lelem_t *elem;
  
  if( l->size==0 )
    {
      return 0;
    }
  
  elem=l->tail;
  
  l->tail=elem->prev;
  if( l->tail==0 )
    {
      l->head=0;
    }
  
  --(l->size);
  
  return elem;
}
