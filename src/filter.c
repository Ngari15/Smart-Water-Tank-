#include "filter.h"

void filter_init(moving_average_t * filter,uint16_t initial_value){

    filter->sum = 0;
    filter->index = 0;

    for (uint8_t i=0; i <FILTER_WINDOW_SIZE; i++){
        filter-> samples[i]= initial_value;
        filter-> sum += initial_value;
    }

}

uint16_t filter_update(moving_average_t *filter, uint16_t new_sample){

    filter ->sum -= filter->samples[filter->index];
    filter ->samples[filter->index]= new_sample;
    filter ->sum += new_sample;
    filter ->index++;

    if(filter->index >= FILTER_WINDOW_SIZE)
    {
        filter->index= 0;
    }
    
    return(uint16_t)(filter->sum/ FILTER_WINDOW_SIZE);
}