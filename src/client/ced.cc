/* "C" event display.
 * Event buffer and server side input processing.
 *
*ik
 * Alexey Zhelezov, DESY/ITEP, 2005 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ced.h>

#include <iostream>

//static ced_event eve = {0,0};

ced_event eve = {0,0};

// NOT used in CED client
static ced_event ceve = {0,0}; // current event on screen

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
   // if( eve.e[i].alloced > 0){
   //     eve.e[i].alloced=0; //hauke: 15.12.11
   //     //free(eve.e[i].b-HDR_SIZE);
   // }
  }
}

static void ced_buf_alloc(ced_element *pe,unsigned count){
  if(!pe->b){

    //std::cout << "malloc  requestet: " << count*pe->size+HDR_SIZE << "bytes" << std::endl;
    pe->b=(unsigned char *) malloc(count*pe->size+HDR_SIZE);
    //printf("malloc: ask for NEW %lu bytes pointer: %p\n ", count*pe->size+HDR_SIZE, pe->b); //hauke
    if(pe->b==NULL){ //hauke
        printf("ERROR: malloc failed!\n");
        exit(1);
    }
  }else{
    //free(pe->b-HDR_SIZE);
    //pe->b=(unsigned char *) malloc(count*pe->size+HDR_SIZE);

    //std::cout << "realloc requestet: " << count*pe->size+HDR_SIZE << "bytes" << std::endl;
    pe->b=(unsigned char *) realloc(pe->b-HDR_SIZE,count*pe->size+HDR_SIZE);

    //printf("malloc: ask for %lu bytes, pointer: %p\n", count*pe->size+HDR_SIZE,pe->b);//hauke
    if(pe->b==NULL){ //hauke
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

static void ced_event_copy(ced_event *trg){
  unsigned i;
  ced_element *pe;
  //std::cout << "trg->e_count: " << trg->e_count << std::endl;
  //std::cout << "eve.e_count: " << eve.e_count << std::endl;

  //eve.e_count = 0;
  if(trg->e_count<eve.e_count){
    //free(trg->e);
    trg->e=(ced_element*) realloc(trg->e,eve.e_count*sizeof(ced_element));

    //trg->e=(ced_element*) malloc(eve.e_count*sizeof(ced_element));
  }


  for(i=0;i<eve.e_count;i++){
        pe=trg->e+i;
        if(i<trg->e_count){
            //if(pe->alloced > 0){
            //  free(pe->b);
            //  pe->b=NULL;
            //  pe->alloced=0;
            //}


            //if(pe->alloced > 0){
            //    std::cout << "try to free" << std::endl;
            //    free(pe->b-HDR_SIZE);
            //    pe->alloced = 0;
            //    std::cout << "finished" << std::endl;
            //}

            if(pe->alloced<eve.e[i].alloced) {
	          ced_buf_alloc(pe,eve.e[i].alloced);
                //std::cout << "test1 " << std::endl;
            }
            pe->count=eve.e[i].count;

        }else{
            memcpy(pe,eve.e+i,sizeof(ced_element));
            if(pe->b){
	              pe->b=0;
                  //  std::cout << "test2 " << std::endl;
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
    //printf("ceve.e_count: %i\n", ceve.e_count);
    //for(i=ceve.e_count-1; i >=0;i--){ //quick hack, change order so that the detector is drawn at last
    //printf("i = %i\n", i);

    pe=ceve.e+i;
    if(!pe->draw)
      continue;
    for(pdata=pe->b,j=0;j<pe->count;j++,pdata+=pe->size)
      (*(pe->draw))(pdata);
  }
}

int ced_process_input(void *data){
  struct _phdr{
    unsigned size;
    unsigned type;
    unsigned char b[4];
  } *hdr = (_phdr*) data;
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
