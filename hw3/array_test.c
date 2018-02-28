#include <stdlib.h>
#include <stdio.h>
#include "libs/array2d.h"
#include "modelCallbacks.h"

int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("Please provide an input and output file.\n");
		return 1;
	}
	
	printf("reading in array.\n");
	FILE* in = fopen(argv[1], "r");	
	Array2d array = Array2d_deserialize(in, modelCallback_deserializeElement);
	fclose(in);

	Array2d_map(array, modelCallback_printElement);
	
	printf("\nwriting array.\n");
	FILE* out = fopen(argv[2], "w+");
	Array2d_serialize(array, out, modelCallback_serializeElement);
	fclose(out);
	
	printf("destroying array.\n");
	Array2d_destroy(array, modelCallback_destroyElement);
	return 0;	
}
