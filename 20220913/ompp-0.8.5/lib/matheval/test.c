#include "matheval.h"

int main( int argc, char* argv[] )
{
  void *eval;

  char *names[] = {"a", "b"};
  double values[2]= {2.3, 5.6};

  double res;

  eval=evaluator_create("a+2*b");
  
  if( !eval )
    {
      printf("error in evaluator!\n");
      return 1;
    }

  res = evaluator_evaluate(eval, 2, names, values );
  printf( "*** %f ***\n", res);
  

  //  evaluator_destroy(eval);
}
