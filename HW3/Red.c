#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	// Convert argument to an integer for processing, consider stol
	char *coded_values[5778];
        char *coded_value;
        coded_value = strtok(argv[1], " "); 
        int i = 0;  
        // Convert coded ints into a character array
        while(coded_value != NULL) {
                coded_values[i] = coded_value;
                i++;
                coded_value = strtok(NULL, " ");
        }
	
	int codedValue;        
	char decodedString[30];        
	char allDecodedStrings[60000];        
	// Get PID for console output
	const pid_t pid = getpid();
	for(int i = 0; i < 5777; i++) {                
		// Store the coded input number into codedValue
		codedValue = atoi(coded_values[1]);
		
		printf("Red[%d]: Received coded value %d\n", pid, codedValue);

		// Decode value with bitshifting, output the result
		int decodedValue = (codedValue >> 16) & 0xFF;
		printf("Red[%d]: Decoded into %d\n", pid, decodedValue);

		// Store decoded value into a string, add to all decoded value strings
		sprintf(decodedString, "%d ", decodedValue);                
		strcat(allDecodedStrings, decodedString);      
	}

	
	// Return 0 for success
	return 0;
}
