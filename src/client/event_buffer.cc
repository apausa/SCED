/* "C" event display.
 * Event buffer. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <event_buffer.h>

#include <iostream>

ced_event eve = {0,0};

unsigned ced_register_element(unsigned item_size,ced_draw_cb draw_func){
  ced_element *pe;
  if(!(eve.e_count&0xf)){
    eve.e=(ced_element *) realloc(eve.e,(eve.e_count+0x10)*sizeof(ced_element));
  }

  pe=eve.e+eve.e_count;
  memset(pe,0,sizeof(*pe));
  pe->size=item_size;
  pe->draw=draw_func;
  return eve.e_count++;
}

void ced_reset(void){
  unsigned i;
  
  for(i=0;i<eve.e_count;i++){
    eve.e[i].count=0;
  }
}

void ced_buf_alloc(ced_element *pe,unsigned count){
  if(!pe->b){
    pe->b=(unsigned char *) malloc(count*pe->size+HDR_SIZE);
    if(pe->b==NULL){
        printf("ERROR: malloc failed!\n");
        exit(1);
    }
  }else{
    pe->b=(unsigned char *) realloc(pe->b-HDR_SIZE,count*pe->size+HDR_SIZE);
    if(pe->b==NULL){
        printf("ERROR: malloc failed!\n");
        exit(1);
    }
  }
  pe->b+=HDR_SIZE;
  pe->alloced=count;
}

void *ced_add(unsigned id){
  ced_element *pe;
  if(id >= eve.e_count){
    fprintf(stderr,"BUG:CED: attempt to access not registered element\n");
    return 0;
  }
  pe=eve.e+id;

  if(pe->count==pe->alloced){
    ced_buf_alloc(pe,pe->alloced+256);
  }

  return (pe->b+(pe->count++)*pe->size);
}
