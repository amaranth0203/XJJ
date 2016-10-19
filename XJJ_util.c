#include "XJJ_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int socket_connect( char* host , in_port_t port ) {
  struct hostent *hp ;
  struct sockaddr_in addr ;
  int on = 1 , sock ;

  if( ( hp = gethostbyname( host ) ) == NULL ) {
#ifdef _DEBUG_
    herror( "gethostbyname" ) ;
#endif
    exit( 1 ) ;
  }
  bcopy( hp->h_addr , &addr.sin_addr , hp->h_length ) ;
  addr.sin_port = htons( port ) ;
  addr.sin_family = AF_INET ;
  sock = socket( PF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
  setsockopt( sock , IPPROTO_TCP , TCP_NODELAY , ( const char* )&on , sizeof( int ) ) ;
  if( sock == -1 ) {
#ifdef _DEBUG_
    perror( "setsockopt" ) ;
#endif
    exit( 1 ) ;
  }
  if( connect( sock , ( struct sockaddr * )&addr , sizeof( struct sockaddr_in ) ) == -1 ) {
#ifdef _DEBUG_
    perror( "connect" ) ;
#endif
    exit( 1 ) ;
  }
  return sock ;
}

#define BUFFER_SIZE 4
#define HOST "my.csdn.net"
#define PORT (80)
#define CONTENT \
"GET /echofocus HTTP/1.1\r\n"\
"Host: my.csdn.net\r\n\r\n"

const char* get_CC_address( ) {
  char* address ;
  address = ( char* )malloc( 128 * sizeof( char ) ) ;
  memset( address , 0 , 128 ) ;
  int found[6] = { 0 , 0 , 0 , 0 , 0 , 0 } ;
  int fd , i ;
  char buffer[BUFFER_SIZE] ;
  fd = socket_connect( HOST , PORT ) ;
  write( fd , CONTENT , strlen( CONTENT ) ) ;
  bzero( buffer , BUFFER_SIZE ) ;
  while( read( fd , buffer , BUFFER_SIZE ) != 0 ) {
    if( found[0] && found[1] && found[2] && found[3] && found[4] && found[5] ) {
      strcat( address , buffer ) ;
      if( strlen( address ) > 22 ) break ;//length of wassupxxx.xxx.xxx.xxx
      continue ;
    }
    for( i = 0 ; i < BUFFER_SIZE ; i ++ ) {
      if( buffer[i] == 'w' && !found[0] ) {
        found[0] = 1 ;
      }
      else if( buffer[i] == 'a' && found[0] && !found[1] ) {
        found[1] = 1 ;
      }
      else if( buffer[i] == 's' && found[1] && !found[2] ) {
        found[2] = 1 ;
      }
      else if( buffer[i] == 's' && found[2] && !found[3] ) {
        found[3] = 1 ;
      }
      else if( buffer[i] == 'u' && found[3] && !found[4] ) {
        found[4] = 1 ;
      }
      else if( buffer[i] == 'p' && found[4] && !found[5] ) {
        found[5] = 1 ;
      }
      else {
        found[0] = found[1] = found[2] = found[3] = found[4] = found[5] ;
      }
    }
    if( found[0] && found[1] && found[2] && found[3] && found[4] && found[5] ) {
      strcat( address , buffer ) ;
    }
    bzero( buffer , BUFFER_SIZE ) ;
  }
  shutdown( fd , SHUT_RDWR ) ;
  close( fd ) ;
  /* remove front string */
  while( address[0] < '0' || address[0] > '9' ) {
    memmove( address , address + 1 , strlen( address ) ) ;
  }
  /* remove tail string */
  i = 0 ;
  while( address[i] == '.' || ( address[i] >= '0' && address[i] <= '9' ) ) {
    i ++ ;
  }
  address[i] = '\0' ;
  return address ;
}
