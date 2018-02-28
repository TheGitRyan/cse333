#ifndef __CALLBACKS_H_
#define __CALLBACKS_H_
#include <jansson.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

//deserialize an element
void* modelCallback_deserializeElement(json_t *element);

//destroy an element
bool modelCallback_destroyElement(void *element);

//serialize an element
json_t* modelCallback_serializeElement(void *element);

//print an element
void modelCallback_printElement(void *element);

#endif

