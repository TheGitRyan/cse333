#include "array2d.h"

//deserialize a 2d array of Array2d values
Array2d Array2d_deserialize(FILE* in, void* (*deserializeElement)(json_t *)) {
	json_t *array_info;
	json_error_t error;
	array_info = json_loadf(in, 0, &error); //load json object from file

	//make sure the json object storing our array has correct number of vals
	if(json_object_size(array_info) != (size_t)3){
		return NULL;
	}

	//read the number of rows from the json object
	json_t *num_rows_json = json_object_get(array_info, "rows");
	int num_rows = json_integer_value(num_rows_json);

	//read the number of cosl from the json object
	json_t *num_cols_json = json_object_get(array_info, "columns");
	int num_cols = json_integer_value(num_cols_json);
	
	//get the data field in the json object which stores the values to be read
	json_t *array_data = json_object_get(array_info, "data");

	//allocate new struct
	Array2d array = (Array2d)malloc(sizeof(Array2d_s));
	if(array == NULL) {
		printf("err out of memory");
		return NULL;
	}

	//assign fields of struct, new array that stores (num_rows * num_cols) values of type Array2d_value_t
 	array->pData = (Array2d_value_t*)malloc(sizeof(Array2d_value_t) * num_rows * num_cols);
	array->rows = num_rows;
	array->cols = num_cols;

	for(int i = 0; i < num_rows; i++) { 
		for(int j = 0; j < num_cols; j++){
			int offset = i * num_cols + j;
			json_t *element = json_array_get(array_data, (size_t)offset);
			*(array->pData + offset) = (Array2d_value_t)deserializeElement(element);
		}
	}

	//free parent json object
	json_decref(array_info);
	
	return array;
}

//returns the number of rows in this array
unsigned int Array2d_rows(Array2d pArray){
	return pArray->rows;
}

//return the number or cols in this array
unsigned int Array2d_columns(Array2d pArray){
	return pArray->cols;
}

//print the elements in our array
void Array2d_map(Array2d pArray, void (*elementPrint)(void*)){
	for(int i = 0; i < pArray->rows; i++){
		for(int j = 0; j < pArray->cols; j++) {
			int offset = i * pArray->cols + j;
			elementPrint(*(pArray->pData + offset));
		}
	}
}

//get the value stored in the array at position (i,j)
Array2d_value_t Array2d_get(Array2d pArray, int i, int j) {
	if(0 <= i && i < pArray->rows && 0 <= j && j < pArray->cols) {
		int offset = i * pArray->cols + j;
		return *(pArray->pData + offset);
	} 
	return NULL;
}

//set value at position (i,j) in array
bool Array2d_set(Array2d pArray, int i, int j, Array2d_value_t value) {
	if(0 <= i && i < pArray->rows && 0 <= j && j < pArray->cols){
		int offset = i * pArray->cols + j;
		*(pArray->pData + offset) = value;
		return true;
	} 
	return false;
}

void swap(Array2d_value_t *a, Array2d_value_t *b) {
	Array2d_value_t temp = *b;
	*b = *a;
	*a = temp;
}

//swap (i1,j1) and (i2,j2)
bool Array2d_swap(Array2d pArray, int i1, int j1, int i2, int j2){
	//check whether any indices out of bounds
	if(!(0 <= i1 && i1 < pArray->rows && 0 <= j1 && j1 < pArray->cols) ||
	   !(0 <= i2 && i2 < pArray->rows && 0 <= j2 && j2 < pArray->rows)){
		return false;	
	}
	
	//get the offsets of (i1, j1), (i2, j2) and get pointers to the two vals
	int offset1 = i1 * pArray->cols + j1;
	int offset2 = i2 * pArray->cols + j2;
	Array2d_value_t *pValOne = pArray->pData + offset1;
	Array2d_value_t *pValTwo = pArray->pData + offset2;

	swap(pValOne, pValTwo);
	return true;
}

void Array2d_serialize(Array2d pArray, FILE* out, json_t*(*serializeElement)(void *)) {
	json_t *array_info =  json_object();	
	//set rows and cols in array_info json object
	json_object_set_new(array_info, "rows", json_integer(pArray->rows));
	json_object_set_new(array_info, "columns", json_integer(pArray->cols));

	json_t *array_data = json_array();
	
	//get the elements in our 2d array struct and append them into our json_array
	for(int i = 0; i < pArray->rows; i++) {
		for(int j = 0; j < pArray->cols; j++) {
			int offset = i * pArray->cols + j;
			Array2d_value_t *pVal = pArray->pData + offset;
			json_t *serialElement = serializeElement((void*)*pVal);
			json_array_append_new(array_data, serialElement);
		}
	}
	//add data into array info json object
	json_object_set_new(array_info, "data", array_data);
	json_dumpf(array_info, out, 0); //output to file

	//free parent json object
	json_decref(array_info);
	
}

bool Array2d_destroy(Array2d pArray, bool (*destroyElement)(void *)) {
	if(pArray == NULL) { return false; }

	bool success = true;
	//free all values in array
	//necessary for the case when values in our array are pointers to larger objects
	for(int i = 0; i < pArray->rows; i++)	{
		for(int j = 0; j < pArray->cols; j++) {
			int offset = i * pArray->cols + j;
			Array2d_value_t val = *(pArray->pData + offset);
			success &= destroyElement((void *)val);
		}
	}
	//free malloced array space
	free(pArray->pData);
	//free rest of struc
	free(pArray);
	return success;
}




