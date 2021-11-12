#ifndef WL_IRDA_KCOPE_H_
#define WL_IRDA_KCOPE_H_

#include "irda.h"


typedef void (* key_cope_pfn)(void *);
typedef struct key_cope_map
{
	uint8_t key_val;
	key_cope_pfn pfn;
}key_cope_map_t;

void key_callback_register(uint16_t key_val, key_cope_pfn pfn);
void key_callback_do(uint16_t key_val, void *op);


#endif

