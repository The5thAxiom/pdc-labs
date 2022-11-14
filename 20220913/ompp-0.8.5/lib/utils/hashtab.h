/* 
 * hashtab.h   */
/* @OMPP_HEADER@ */

#ifndef HASHTAB_H_INCLUDED
#define HASHTAB_H_INCLUDED

/*
 * key type
 */
typedef void* hashtab_key_t;


/*
 * value type
 */
typedef void* hashtab_val_t;


/*
 * comparing keys
 */
#ifndef HASHTAB_COMP_KEYS
#define HASHTAB_COMP_KEYS(k1,k2) (k1==k2)
#endif

/* 
 * comparing values
 */
#ifndef HASHTAB_COMP_VALUES
#define HASHTAB_COMP_VALUES(v1,v2) (v1==v2)
#endif

/*
 * compute hash value 
 */

#ifndef HASHTAB_GET_HASH
#define HASHTAB_GET_HASH(k) ((unsigned long)(k))
#endif 

/*
 * null value
 */

/* #define HASHTAB_NULL_VALUE ((void*) 0) */

#define HASHTAB_NULL_VALUE ((void*)0)


struct hentry
{
  struct hentry* next;
  hashtab_key_t key;
  hashtab_val_t value;
};


struct hashtab
{
  unsigned int size;
  unsigned int n_entries;    /* number of entries */

  struct hentry* table;
};


/*
 * create a new hash table
 */ 
struct hashtab* 
hashtab_create(unsigned int size);


/*
 * destroy the table
 */
void hashtab_destroy(struct hashtab *htab);


/*
 * return number of entries
 */
unsigned int hashtab_count(struct hashtab *htab);


/*
 * insert key-value pair
 */
int hashtab_insert( struct hashtab *htab, 
		    hashtab_key_t key, hashtab_val_t val );


/*
 * remove the specified key-value pair
 */
int hashtab_remove( struct hashtab *htab, 
		    hashtab_key_t key );


/* 
 * search for key
 */
hashtab_val_t hashtab_search( struct hashtab *htab, 
			      hashtab_key_t key );


#endif /* HASHTAB_H_INCLUDED */
