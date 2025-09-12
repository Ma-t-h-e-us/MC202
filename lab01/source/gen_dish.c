#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"

int main(int argc, char *argv[])
{
    // Read file name from arguments
	const char *file_name = get_file_name(argc, argv, "test_dish");

	// Set experiment seed
	srand(time(NULL));

	// Matrix size
	int M;

	// Read matrix size from user
	printf("Matrix length MxM (small integer): ");
	scanf("%d", &M);
	
    // Write random dish status to file
	write_random_dish_to_file(file_name, M);
	printf("File '%s' generated with success.\n", file_name);

	return 0;
}
