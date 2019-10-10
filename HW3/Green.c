#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	// COnver command line arg to an integer
	int codedValue = atoi(argv[1]);
	
	// Get process id for output
	const pid_t pid = getpid();
	//char* outputPrefix = strcat("Red[", itoa(pid));	
	printf("Green[%d]: Received coded value %d\n", pid, codedValue);

	// Decode value using bitshifting, output result
	int decodedValue = (codedValue >> 8) & 0xFF;
	printf("Green[%d]: Decoded into %d\n", pid, decodedValue);
	
	// Return value to parent
	return decodedValue;
}
