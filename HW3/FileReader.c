#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	// Get file descriptor and close read end
	int fdPtr =atoi(argv[2]);

	// Open input filename for reading
        FILE *fin = fopen(argv[1], "r");
        // Loop until there are no more input from the file
        int coded;
	char codedString[30];
	char allCodedStrings[60000];
        // Store the coded input number into coded
        while(fscanf(fin, "%d", &coded) == 1) {
                sprintf(codedString, "%d ", coded);
		strcat(allCodedStrings, codedString);
	}
	write(fdPtr, allCodedStrings, strlen(allCodedStrings)+1);
	close(fdPtr);
	return 0;
}
