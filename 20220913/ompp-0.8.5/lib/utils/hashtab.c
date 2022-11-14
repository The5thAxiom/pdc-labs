/* 
 * hashtab.c  */
/* @OMPP_HEADER@ */


#include <stdlib.h>
#include "hashtab.h"

struct hashtab* hashtab_create(unsigned int size)
{
  int i;
  struct hashtab *tab;

  tab = (struct hashtab *) malloc( sizeof(struct hashtab) );
  if( !tab )
    return 0;

  tab->n_entries=0;
  tab->size=size;

  tab->table=(struct hentry*) malloc( sizeof(struct hentry)*tab->size );

  for( i=0; i<size; i++ )
  {
   tab->table[i].value=HASHTAB_NULL_VALUE;
    tab->table[i].next=0;
  }

  if( !(tab->table) )
  {
    free(tab);
    return 0;
  }
  return tab;
}

void hashtab_destroy(struct hashtab *tab)
{
  int i;
  struct hentry *entry, *temp;

  for( i=0; i<tab->size; i++ )
  {
    entry=tab->table[i].next;

    while( entry!=0 )
    {
      temp=entry->next;
      free( entry );
      entry=temp;
    }
  }

  free( tab->table );
  free( tab );
}


unsigned int hashtab_count( struct hashtab *htab )
{
  return htab->n_entries;
}



int hashtab_insert( struct hashtab *htab, hashtab_key_t key, hashtab_val_t val )
{
  struct hentry *entry;
  unsigned long hash;
  unsigned long index;

  hash = (unsigned long) HASHTAB_GET_HASH(key);
  index=hash%(htab->size);

  if( HASHTAB_COMP_VALUES( val, HASHTAB_NULL_VALUE) )
    return 0;

  /* store in table */
  if( htab->table[index].value==HASHTAB_NULL_VALUE )
  {
    htab->table[index].key=key;
    htab->table[index].value=val;
    return 1;
  }

  /* else make new entry  */
  entry=(struct hentry*) malloc( sizeof(struct hentry) );
  if( !entry )
    return 0;
  entry->key=key;
  entry->value=val;
  entry->next=htab->table[index].next;
  htab->table[index].next=entry;

  return 1;
}



hashtab_val_t hashtab_search( struct hashtab *htab, hashtab_key_t key )
{
  struct hentry *entry;
  unsigned long hash;
  unsigned long index;

  hash = (unsigned long) HASHTAB_GET_HASH(key);
  index=hash%(htab->size);


  entry=&(htab->table[index]);

  while(entry )
  {
    if( HASHTAB_COMP_KEYS(entry->key, key) && 
      !HASHTAB_COMP_VALUES( entry->value, HASHTAB_NULL_VALUE) )
    {
      return entry->value;
    }

    entry=entry->next;
  }

  return HASHTAB_NULL_VALUE;
}


int hashtab_remove( struct hashtab *htab, hashtab_key_t key )
{
  struct hentry *entry, *temp;
  unsigned long hash;
  unsigned long index;

  hash = (unsigned long) HASHTAB_GET_HASH(key);
  index=hash%(htab->size);

  
  /* in-table ? */
  if( HASHTAB_COMP_KEYS(htab->table[index].key, key) && 
      !HASHTAB_COMP_VALUES( htab->table[index].value, HASHTAB_NULL_VALUE) )
  {
    /* mark empty */
    htab->table[index].value=HASHTAB_NULL_VALUE;
    return 1;
  }

  temp=&(htab->table[index]);
  entry=temp->next;

  while(entry)
  {
    if( HASHTAB_COMP_KEYS(entry->key, key) )
    {
      temp->next=entry->next;
      free(entry);
      /* success */
      return 1;
    }

    temp=entry;
    entry=temp->next;
  }

  /* not found */
  return 0;
}


/*
int main( int argc, char *argv[] )
{
  struct hashtab *mytab;
  hashtab_key_t key;


  mytab=hashtab_create( 1000 );
  
  hashtab_insert( mytab, key, mytab );
  hashtab_insert( mytab, key, mytab );
  hashtab_insert( mytab, key, mytab );
  hashtab_insert( mytab, key, mytab );


  printf("search %X \n", hashtab_search( mytab, key) );

  printf("%d \n", hashtab_remove( mytab, key) );
  printf("search %X \n", hashtab_search( mytab, key) );

  printf("%d \n", hashtab_remove( mytab, key) );
  printf("search %X \n", hashtab_search( mytab, key) );
  
  printf("%d \n", hashtab_remove( mytab, key) );
  printf("search %X \n", hashtab_search( mytab, key) );

  printf("%d \n", hashtab_remove( mytab, key) );
  printf("search %X \n", hashtab_search( mytab, key) );

  printf("%d \n", hashtab_remove( mytab, key) );
  printf("%d \n", hashtab_remove( mytab, key) );

  hashtab_destroy(mytab); 
}
*/
