#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	for(int i = 0; i < 12; i++)
		printf(" yo %s\n", argv[1]);
	int codedValue;        
	char decodedString[30];        
	char allDecodedStrings[60000];        
	// Get PID for console output
	const pid_t pid = getpid();
	/*
	for(int i = 0; i < 5777; i++) {                
		// Store the coded input number into codedValue
		codedValue = atoi(argv[argc][i]);
		
		printf("Red[%d]: Received coded value %d\n", pid, codedValue);

		// Decode value with bitshifting, output the result
		int decodedValue = (codedValue >> 16) & 0xFF;
		printf("Red[%d]: Decoded into %d\n", pid, decodedValue);

		// Store decoded value into a string, add to all decoded value strings
		sprintf(decodedString, "%d ", decodedValue);                
		strcat(allDecodedStrings, decodedString);      
	}

	*/
	// Return 0 for success
	return 0;
}
