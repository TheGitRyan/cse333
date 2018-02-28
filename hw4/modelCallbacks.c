//includes the actual definitions for the callback functions to make the model work with our 2Darrya library from hw3
//since the model represents a 2d array of images and the images are indexed using integers the actual underlying
//2d model array will only store integers
//
//These callbacks are for serializing/deserializing the integers stored in that array
#include "modelCallbacks.h"

//deserialize element
void* modelCallback_deserializeElement(json_t *element){

        if(json_is_integer(element)) {
                int val = json_integer_value(element);
                return (void*)val;
        }
        printf("deserialization failed");
        return NULL;
}

//destroy an element
bool modelCallback_destroyElement(void* element) {
	//since integers are a simple data type
	//no freeing is necessary
        return true;
}

//serialize an element
json_t* modelCallback_serializeElement(void *element){
        return json_integer((intptr_t)element);
}

//print an element
void modelCallback_printElement(void *element) {
	printf("%ld ", (intptr_t)element);
}
