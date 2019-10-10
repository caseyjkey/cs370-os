#include <stdlib.h>

int main(int argc, char argv[]) {
	char filename[] = argv[1];
	
	// Get file descriptor and close read end
	int *fdPtr = atoi(argv[2]);
	close(*fdPtr[0])

	// Open input filename for reading
        FILE *fin = fopen(filename, "r");
        // Loop until there are no more input from the file
        int coded;
        // Store the coded input number into coded
        while(fscanf(fin, "%d", &coded) == 1 && i < 5776) {
                sprintf(sNum, "%d", coded);
	}
       
}
