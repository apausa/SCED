/* "C" event display.
 * Server side event handling: the event on screen and the input from the client. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <event_buffer.h>
#include "event.h"

// NOT used in CED client
static ced_event ceve = {0,0}; // current event on screen

static void ced_event_copy(ced_event *trg){
  unsigned i;
  ced_element *pe;

  if(trg->e_count<eve.e_count){
    trg->e=(ced_element*) realloc(trg->e,eve.e_count*sizeof(ced_element));
  }


  for(i=0;i<eve.e_count;i++){
        pe=trg->e+i;
        if(i<trg->e_count){
            if(pe->alloced<eve.e[i].alloced) {
	          ced_buf_alloc(pe,eve.e[i].alloced);
            }
            pe->count=eve.e[i].count;

        }else{
            memcpy(pe,eve.e+i,sizeof(ced_element));
            if(pe->b){
	              pe->b=0;
	              ced_buf_alloc(pe,pe->alloced);
            }
        }
        if(pe->count){
            memcpy(pe->b,eve.e[i].b,pe->count*pe->size);
        }
  }
  trg->e_count=eve.e_count;
}

void ced_do_draw_event(void){
  unsigned int i,j;
  ced_element *pe;
  unsigned char *pdata;
  for(i=0;i<ceve.e_count;i++){
    pe=ceve.e+i;
    if(!pe->draw)
      continue;
    for(pdata=pe->b,j=0;j<pe->count;j++,pdata+=pe->size)
      (*(pe->draw))(pdata);
  }
}

int ced_process_input(void *data){
  struct _phdr *hdr = (_phdr*) data;
  unsigned count;
  ced_element *pe;
  
  if(!data){ // new client is connected
    ced_reset();
    return 0;
  }

  if(hdr->type == DRAW_EVENT){
    ced_event_copy(&ceve);
    ced_reset();
    return 1;
  }
  if(hdr->type>=eve.e_count){
    fprintf(stderr,"WARNING:CED: undefined element type (%u), ignored\n",
	    hdr->type);
    return 0;
  }
  pe=eve.e+hdr->type;
  if((hdr->size-HDR_SIZE)%pe->size){
    fprintf(stderr,"BUG:CED: size alignment is wrong for element %u\n", hdr->type);
    return 0;
  }
  count=(hdr->size-HDR_SIZE)/pe->size;
  if(!count)
    return 0;
  if(count>=pe->alloced)
    ced_buf_alloc(pe,count+256);
  memcpy(pe->b,hdr->b,count*pe->size);
  pe->count=count;
  return 0;
}
