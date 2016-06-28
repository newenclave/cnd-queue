#include <stdio.h>
#include "arrays.h"
#include "tinycthread.h"

#define BACKETS_COUNT 17

typedef struct  _value {
    int data;
} value;

typedef struct _key_value_pair {
    size_t key_;
    value  val_;
} key_value_pair;

array_define_custom_type( key_value_pair, queue );

typedef struct _backet_node {
    mtx_t mtx_;
    cnd_t cnd_;
    queue dat_;
} backet_node;

backet_node g_backets[BACKETS_COUNT];

void back_add( size_t key, value *val )
{
    backet_node *node = &g_backets[key % BACKETS_COUNT];
    mtx_lock( &node->mtx_ );
    //array_bin_upper_bound( )
    mtx_unlock( &node->mtx_ );
}

int main( )
{

    int i;
    for( i=0; i<BACKETS_COUNT; ++i ) {
        cnd_init(&g_backets[i].cnd_);
        mtx_init(&g_backets[i].mtx_, mtx_plain);
        array_place_init( g_backets[i].dat_ );
    }

}
