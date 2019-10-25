#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	// Let gets that shared memory file descriptor
        int green_fd = shm_open(argv[argc-1], O_CREAT | O_RDWR, 0666);

        // Now let's point to it, need this to access the shm
        char *green_shmPtr = mmap(0, 60000, PROT_READ | PROT_WRITE, MAP_SHARED, green_fd, 0);

	
	// COnver command line arg to an integer
	int codedValue;
	char decodedString[30];
	char allDecodedStrings[60000];
	// Get process id for output
	const pid_t pid = getpid();

	for(int i = 0; i < 5776; i++) {
		codedValue = atoi(argv[i]);

		printf("Green[%d]: Received coded value %d\n", pid, codedValue);

		// Decode value using bitshifting, output result
		int decodedValue = (codedValue >> 8) & 0xFF;
		printf("Green[%d]: Decoded into %d\n", pid, decodedValue);
		
		// Store decoded value into a string, add to all decoded value strings
                sprintf(decodedString, "%d ", decodedValue);
                strcat(allDecodedStrings, decodedString);
	}

	// Remove last space and replace with string terminator
        char *lastSpace = strrchr(allDecodedStrings, ' ');
        *lastSpace = '\0';

        // Put our finished string into shared memory!
        memcpy(green_shmPtr, allDecodedStrings, 60000);

	// Return zero for success bruh
	return 0;
}
