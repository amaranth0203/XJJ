#include <stdio.h>
#include <stdlib.h>
#include "XJJ_util.h"
int main(int argc, char *argv[])
{
  serv_listen( "127.0.0.1" , 48523 ) ;
  return 0;
}
