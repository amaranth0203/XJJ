#include <stdio.h>
#include <stdlib.h>
#include "XJJ_util.h"
int main(int argc, char *argv[])
{
#ifdef _DEBUG_
  printf( "server : %s\n" , get_CC_address( ) ) ;
#endif
  return 0;
}
