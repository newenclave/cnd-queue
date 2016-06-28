#include <stdio.h>
#include "arrays.h"


#define BACKETS_COUNT 17

typedef struct  _value {

} value;

typedef struct _key_value_pair {
    size_t key_;
    value  val_;
} key_value_pair;

array_define_custom_type( key_value_pair, queue );

typedef struct _backet_node {

} backet_node;

int main( )
{

}
