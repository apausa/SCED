/* Command-line argument parsing for CED's event display server.
 * Split out of glced.cc's main(). */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>

#include "cli.h"
#include "ui/input.h"

void parseCliArgs(int argc, char *argv[]){
    int i;
    for(i=1;i<argc ; i++){

      if(!strcmp( argv[i] , "-world_size" ) ) {
        float w_size = atof(  argv[++i] )  ;
        printf( "  setting world size to  %f " , w_size ) ;
        mm.sf = 205.0/w_size;
      } else if(!strcmp( argv[i] , "-h" ) ||
         !strcmp( argv[i] , "--help" )||
         !strcmp( argv[i] , "-?" )
         ) {


      printf( "\n  CED event display server: \n\n"
          "   Usage:  glced [-world_size LENGTH] [-trust TRUSTED_HOST]\n\n"
          "        options:  \n"
          "              LENGTH:       Visible world-cube size in mm (default: 6000) \n"
          "              TRUSTED_HOST: Ip or name of the host who is allowed to connect to CED\n\n"
          "   Example: \n\n"
          "     ./bin/glced -world_size 1000. -trust 192.168.11.22 > /tmp/glced.log 2>&1 & \n\n"
          "    "
          "   Change port (before starting glced):"
              "         export CED_PORT=<portnumber>\n\n\n"
          "   To connect Marlin from a remote machine set variables CED_HOST=<this_host> and CED_PORT=<this_CED_PORT> on the machine where Marlin is started from\n\n"
          "   On this machine start glced with option: -trust <host_where_Marlin_is_started_from> to accept the connection from the remote host"
          "\n\n"
          ) ;

        exit(0) ;
      } else if(!strcmp(argv[i], "-trust")){
          i++;
          if(i >= argc){
              printf("wrong syntax!\n");
              exit(0);
          }

          struct hostent *host = gethostbyname(argv[i]);
          if (host != NULL){
              extern char trusted_hosts[50];
              snprintf(trusted_hosts, 50, "%u.%u.%u.%u",(unsigned char)host->h_addr[0] ,(unsigned char)host->h_addr[1] ,(unsigned char)host->h_addr[2] ,(unsigned char)host->h_addr[3]);
              printf("Trust ip: %s\n", trusted_hosts);
          } else {
              printf("ERROR: Host %s is unknown!\n", argv[i+1]);
          }
      }
    }
}
