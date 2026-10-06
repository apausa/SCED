/* "C" event display.
 * Client side connection and event handling.
 *
 * Alexey Zhelezov, DESY/ITEP, 2005 */
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

//hauke
//#include <stropts.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <sys/socket.h> /* for AF_INET */
#include <iostream>


//http://www.rhyolite.com/pipermail/dcc/2004/001986.html
#ifndef POLLRDNORM //fg: should be defined in poll.h
# define POLLRDNORM     0x040           /* Normal data may be read.  */
# define POLLRDBAND     0x080           /* Priority data may be read.  */
# define POLLWRNORM     0x100           /* Writing now will not block.  */
# define POLLWRBAND     0x200           /* Priority data may be written.  */
#endif
//end hauke

static int ced_fd=-1; // CED connection socket

static unsigned short ced_port=7927; // port No of CED (assume localhost)
static char ced_host[30];

// Return 0 if can be connected, -1 otherwise.
/*static*/ int ced_connect(void){
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
    //printf("i=%i\n",i);
    pe=eve.e+i;
    if(!pe->count)
      continue;
    
    //unsigned hauke;
    //printf("size of unsigned %i\n", sizeof(hauke));
    hdr=(struct _phdr *)(pe->b-HDR_SIZE); // !!! HERE is the trick :)
    hdr->type=i;
    //printf("pe->count %i, pe->size %i\n",pe->count, pe->size);
    hdr->size=HDR_SIZE+pe->count*pe->size;
    sent_sum=0;
    //if(hdr->size > 10000000){printf("U P S!  This data set is realy big! (%f kB)(%i counts)\n",(hdr->size)/1024.0,pe->count);}
    buf=(char *)hdr;
    //printf("hdr->size=%i\n",hdr->size);
    while(sent_sum<hdr->size){
        //printf("sent_sum = %i, hdr->size=%i\n",sent_sum,hdr->size);
	    sent=write(ced_fd,buf+sent_sum,hdr->size-sent_sum);
        
        //printf("byte: %u\n", buf[sent_sum]);
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


//hauke
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


  //printf("ip: %s\n",  ced_host);
  //ced_host=host->h_addr;
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
