#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	// Convert argument input to an integer
	int codedValue = atoi(argv[1]);
	
	// Get process id for output
	const pid_t pid = getpid();
	//char* outputPrefix = strcat("Red[", itoa(pid));	
	printf("Blue[%d]: Received coded value %d\n", pid, codedValue);

	// Decode value using bitshifting, output result
	int decodedValue = (codedValue) & 0xFF;
	printf("Blue[%d]: Decoded into %d\n", pid, decodedValue);
	
	// Return result to parent
	return decodedValue;
}
