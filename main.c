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

int key_value_comp( const key_value_pair *l, size_t key )
{
//    static int i= 0;
//    printf("compare %d\n", i++);
    return (l->key_ < key ) ? -1 : ( key < l->key_ );
}

array_define_custom_type( key_value_pair, queue );

typedef struct _backet_node {
    mtx_t mtx_;
    queue dat_;
} backet_node;

backet_node g_backets[BACKETS_COUNT];

void back_add( size_t key, value *val )
{
    backet_node *node = &g_backets[key % BACKETS_COUNT];
    mtx_lock( &node->mtx_ );
    key_value_pair *r = queue_emplace_back( &node->dat_ );
    r->key_ =  key;
    r->val_ = *val;
    //printf( "result: %ld\n", result );
    mtx_unlock( &node->mtx_ );

}

value *back_get( size_t key )
{
    backet_node *node = &g_backets[key % BACKETS_COUNT];
    size_t result = 0;
    value *res = NULL;
    mtx_lock( &node->mtx_ );
    printf( "arr_len = %ld\n", node->dat_.len_ );
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
        mtx_init(&g_backets[i].mtx_, mtx_plain);
        array_place_init( g_backets[i].dat_ );
        queue_reserve( &g_backets[i].dat_, 8 );
    }

    value v;
    v.data = 1000;

    for( size_t i = 0; i<1000000; i++ ) {
        v.data = v.data + i;
        back_add( i, &v );
    }

    value * r = back_get( 117 );
    if( r ) {
        printf( "%d\n", r->data );
    }

    for( i=0; i<BACKETS_COUNT; ++i ) {
        printf( "arr_len[%i] = %ld\n", i, g_backets[i].dat_.len_ );
        mtx_destroy( &g_backets[i].mtx_ );
        queue_free(  &g_backets[i].dat_ );
    }

}
