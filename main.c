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

int key_value_comp( const key_value_pair l, size_t key )
{
    return (l.key_ < key ) ? -1 : ( key < l.key_ );
}

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
    size_t result = 0;
    mtx_lock( &node->mtx_ );
    array_bin_lower_bound_compare( node->dat_, key, &key_value_comp, result );
    key_value_pair *r = queue_emplace_at( &node->dat_, result );
    r->key_ =  key;
    r->val_ = *val;
    printf( "result: %ld\n", result );
    mtx_unlock( &node->mtx_ );

}

value *back_get( size_t key )
{
    backet_node *node = &g_backets[key % BACKETS_COUNT];
    size_t result = 0;
    value *res = NULL;
    mtx_lock( &node->mtx_ );
    array_bin_search_compare( node->dat_, key, &key_value_comp, result );
    if( result != array_lenght(node->dat_) ) {
        res = &array_at( node->dat_, result ).val_;
    }
    mtx_unlock( &node->mtx_ );
    return res;
}

int main( )
{
    int i;
    for( i=0; i<BACKETS_COUNT; ++i ) {
        cnd_init(&g_backets[i].cnd_);
        mtx_init(&g_backets[i].mtx_, mtx_plain);
        array_place_init( g_backets[i].dat_ );
    }

    value v;
    v.data = 1000;

    for( int i = 0; i<100; i++ ) {
        v.data = v.data + i;
        back_add( i, &v );
    }

    value * r = back_get( 56 );
    printf( "%d\n", r->data );
}
