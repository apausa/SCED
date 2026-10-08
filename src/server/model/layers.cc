#include <cstdio>
#include <cstring>

#include <sced_types.h>

#include "layers.h"
#include "settings.h"

extern CEDsettings setting;

static char layerDescription[CED_MAX_LAYER][CED_MAX_LAYER_CHAR];

void toggle_layer(unsigned l){
    if(l > CED_MAX_LAYER-1){ return; }

    if(setting.layer[l]){
        setting.layer[l]=false;
    }else{
        setting.layer[l]=true;
    }

}

static void toggle_all(int first, int end){
    int anz=0;
    for(int i=first;i<end;i++){ //try to turn all layers on
        if(!setting.layer[i]){
           toggle_layer(i);
           anz++;
        }
    }
    if(anz == 0){ //turn all layers off
        for(int i=first;i<end;i++){
           toggle_layer(i);
        }
    }
}

void layers_toggle_all_data(void){
    toggle_all(0, NUMBER_DATA_LAYER);
}

void layers_toggle_all_detector(void){
    toggle_all(NUMBER_DATA_LAYER, NUMBER_DETECTOR_LAYER+NUMBER_DATA_LAYER);
}

void layers_show_all(void){
    for(int i=0;i<CED_MAX_LAYER;i++){
        setting.layer[i]=true; // turn all layers on
    }
}

void layer_set_description(int id, const char *text){
    if(id < 0 || id >= CED_MAX_LAYER){
        printf("Warning: Layer id out of range\n");
        return;
    }
    strncpy(layerDescription[id], text,CED_MAX_LAYER_CHAR-1);
}

const char *layer_description(int layer){
    if(layer < 0 || layer >= CED_MAX_LAYER){
        return "";
    }
    return layerDescription[layer];
}
