#ifndef __ARRAY2D_H_
#define __ARRAY2D_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <jansson.h>



//Pointer to the values that are going to be stored in 1d array
typedef void *Array2d_value_t;

//Array struct
//stuct to represent the 2d array, stores the num rows and num cols, as well as a *pointer value which points to first row in data
//typedef struct Array2d_s Array2d_s;

typedef struct Array2d_s{
	unsigned int rows, cols;
	Array2d_value_t *pData;	
} Array2d_s;

//Array2d_s pointer type, points to 2d array
typedef Array2d_s *Array2d;


//method to deserialize json object
//takes a file ptr to read in the elements and a method to 
//deserialize the elements of our file
//
//returns a pointer to the first element in the array
Array2d Array2d_deserialize(FILE* in, void*(*deserializeElement)(json_t *));

//get number of rows
//
//returns an unsigned int which is the number of rows in the array
unsigned int Array2d_rows(Array2d pArray);

//get number of columns
//
//returns an unsigned int which is the number of columns in the array
unsigned int Array2d_columns(Array2d pArray);

//print the values in the array
//
//return nothing but prints the values in the array delimiting with ' ' character
void Array2d_map(Array2d pArray, void(*printElement)(void*));

//returns the value stored in the array at position (i,j)
Array2d_value_t Array2d_get(Array2d pArray, int i, int j);

//method to set value in 2d array with a particular value
//
//set the value at position (i,j) in 2d array pointed at by pArray with value "value"
//
//Returns true if the value was successfully set and false otherwise
bool Array2d_set(Array2d pArray, int i, int j, Array2d_value_t value);

//method to swap
//
//swap the value at position (i1,j1) of 2d array with value at position (i2,j2)
//
//Returns true if the values were successfully swapped and false otherwise
bool Array2d_swap(Array2d pArray, int i1, int j1, int i2, int j2);

//method to serialize
//
//serialize the elements in 2d array pointed at by pArray by printing them out to file pointed by out
//each element should be serialized by serialize element function passed in
void Array2d_serialize(Array2d pArray, FILE* out, json_t *(*serializeElement)(void *));

//method to destroy
//
//destroy (i.e. free) the elements in the 2d array pointed at by pArray using the destroyElement function to 
//free each individual element
//
//Returns true if the array was succesfully destroyed and false otherwise
bool Array2d_destroy(Array2d pArray, bool (*destroyElement)(void*)); 

#endif




