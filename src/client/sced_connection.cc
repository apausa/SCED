/* "C" event display.
 * Client side connection and event handling. */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#include <event_buffer.h>
#include "sced_connection.h"

#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <sys/socket.h> /* for AF_INET */
#include <iostream>


//http://www.rhyolite.com/pipermail/dcc/2004/001986.html
#ifndef POLLRDNORM // should be defined in poll.h
# define POLLRDNORM     0x040           /* Normal data may be read.  */
# define POLLRDBAND     0x080           /* Priority data may be read.  */
# define POLLWRNORM     0x100           /* Writing now will not block.  */
# define POLLWRBAND     0x200           /* Priority data may be written.  */
#endif

static int ced_fd=-1; // CED connection socket

static unsigned short ced_port=7927; // port No of CED (assume localhost)
static char ced_host[30];

// Return 0 if can be connected, -1 otherwise.
int ced_connect(void){
  static time_t last_attempt=0;
  time_t ct;
  struct sockaddr_in addr;

  if(ced_fd>=0){
    return 0; // already connected;
  }
  time(&ct);
  if(ct-last_attempt<5){
    return -1; // don't try reconnect all the time
  }
  addr.sin_family=AF_INET;
  addr.sin_port=htons(ced_port);
  addr.sin_addr.s_addr=inet_addr(ced_host); 
  memset(&addr.sin_zero, 0, sizeof(addr.sin_zero)); //not nessesary because sin_zero is not used!

  ced_fd=socket(PF_INET,SOCK_STREAM,0);
  if(connect(ced_fd,(struct sockaddr *)&addr,sizeof(addr)) != 0){
    if(!last_attempt){
        perror("WARNING:CED: can't connect to CED");
    }
    time(&last_attempt);
    close(ced_fd);
    ced_fd=-1;
    return -1;
  }
  fprintf(stderr,"INFO:CED: connected to CED\n");
  return 0;
}


void ced_send_event(void){
  struct _phdr *hdr,draw_hdr;
  unsigned i,problem=0;
  int sent_sum;
  char *buf;
  int sent;
  ced_element *pe;

  if(ced_connect())
    return;
  for(i=0;i<eve.e_count && !problem;i++){
    pe=eve.e+i;
    if(!pe->count)
      continue;
    
    hdr=(struct _phdr *)(pe->b-HDR_SIZE); // !!! HERE is the trick :)
    hdr->type=i;
    hdr->size=HDR_SIZE+pe->count*pe->size;
    sent_sum=0;
    buf=(char *)hdr;
    while(sent_sum<hdr->size){
	    sent=write(ced_fd,buf+sent_sum,hdr->size-sent_sum);
        
	    if(sent<0){
            printf("send < 0\n");
	        problem=1;
	        break;
	    }
	    sent_sum+=sent;
    }
  }
  if(!problem){
    draw_hdr.size=HDR_SIZE;
    draw_hdr.type=DRAW_EVENT;
    if(write(ced_fd,&draw_hdr,HDR_SIZE)!=HDR_SIZE)
      problem=1;
  }
  if(problem){
    perror("WARNING:CED: can't send event, till next time...");
    close(ced_fd);
    ced_fd=-1;
  }
}


int ced_selected_id_noblock() {
  int id=-1 ;
  struct pollfd fds[1];
  fds[0].fd=ced_fd;
  fds[0].events = POLLRDNORM | POLLIN;
  if(poll(fds,1,0) > 0){
    if(recv(ced_fd, &id, sizeof(int) , 0 ) > 0){
        return id;
    }else{
        return -1;
    }
  }else{
   return -1;
  }
}

int ced_selected_id() {
  int id=-1 ;
  if(recv(ced_fd, &id, sizeof(int) , 0 ) > 0){
     return id;
  }else{
     return -1;
  }
}
// API
void ced_client_init(const char *hostname,unsigned short port){
  struct hostent *host = gethostbyname(hostname);
  snprintf(ced_host, 30, "%u.%u.%u.%u\n",(unsigned char)host->h_addr[0] ,(unsigned char)host->h_addr[1] ,(unsigned char)host->h_addr[2] ,(unsigned char)host->h_addr[3]); 

  ced_port=port;
  signal(SIGPIPE,SIG_IGN);
}

void ced_new_event(void){
  ced_reset();
}

void ced_draw_event(void){
  ced_send_event();
  ced_reset();
}
