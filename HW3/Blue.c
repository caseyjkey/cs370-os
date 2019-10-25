#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	// Let gets that shared memory file descriptor
        int blue_fd = shm_open(argv[argc-1], O_CREAT | O_RDWR, 0666);

        // Now let's point to it, need this to access the shm
        char *blue_shmPtr = mmap(0, 60000, PROT_READ | PROT_WRITE, MAP_SHARED, blue_fd, 0);

	// For converting argument input to an integer
	int codedValue;
	char decodedString[30];
        char allDecodedStrings[60000];
	// Get process id for output
	const pid_t pid = getpid();
	for(int i = 0; i < 5776; i++) {
                // Store the coded input number into codedValue
                codedValue = atoi(argv[i]);

		printf("Blue[%d]: Received coded value %d\n", pid, codedValue);

		// Decode value using bitshifting, output result
		int decodedValue = (codedValue) & 0xFF;
		printf("Blue[%d]: Decoded into %d\n", pid, decodedValue);

		// Store decoded value into a string, add to all decoded value strings
                sprintf(decodedString, "%d ", decodedValue);
                strcat(allDecodedStrings, decodedString);
        }

	// Remove last space and replace with string terminator
        char *lastSpace = strrchr(allDecodedStrings, ' ');
        *lastSpace = '\0';

        // Put our finished string into shared memory!
        memcpy(blue_shmPtr, allDecodedStrings, 60000);
	
	// Return zero for SUCCESS
	return 0;
}
