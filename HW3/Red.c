#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	// Convert argument to an integer for processing
	int codedValue = atoi(argv[1]);
	
	// Get PID for console output
	const pid_t pid = getpid();
	//char* outputPrefix = strcat("Red[", itoa(pid));	
	printf("Red[%d]: Received coded value %d\n", pid, codedValue);

	// Decode value with bitshifting, output the result
	int decodedValue = (codedValue >> 16) & 0xFF;
	printf("Red[%d]: Decoded into %d\n", pid, decodedValue);
	
	// Return decodedValue for parent
	return decodedValue;
}
