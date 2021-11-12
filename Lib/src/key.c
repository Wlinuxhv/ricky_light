#include "key.h"

const uint8_t key_const_value[] = {
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
    KEY_6, KEY_7, KEY_8, KEY_9, Plus100, Plus200,
    EQ_KEY, VOL_Mins, VOL_Plus, PLAY_PAUSE, NEXT_KEY,
    CH_Mins, CH_Midl, CH_Plus,
};
#define KEY_NUMBS 4//(sizeof(key_const_value)/key_const_value[0])


static key_cope_map_t Key_cope_map[KEY_NUMBS];

void key_callback_register(uint16_t key_val, key_cope_pfn pfn)
{
    uint8_t i = 0;
    for (; i < KEY_NUMBS; i++) {
        if (key_val == key_const_value[i])
            break;
    }
    if (i < KEY_NUMBS) {
        Key_cope_map[i].pfn = pfn;
    }
}

void key_callback_do(uint16_t key_val, void *op)
{

    uint8_t i = 0;
    if (key_val == 0)
        return;
    for (; i < KEY_NUMBS; i++) {
        if (key_val == key_const_value[i])
            break;
    }
    if (i < KEY_NUMBS) {
        Key_cope_map[i].pfn(op);
    }
}


