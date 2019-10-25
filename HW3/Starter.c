// Watch helpdesk video before asking for help

#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define IMAGE_HEIGHT 76
#define IMAGE_WIDTH 76


int main(int argc, const char *argv[]) {
	// Check if a filename has been passed as argument
	char filename[50];
	if(argc == 2) {
		strcpy(filename, argv[1]);
	}
	// If not, use default value
	else { 
		strcpy(filename, "coded_image_1.txt");
	}
	
	// Create output filename given the input filename
	char outputFileName[30]= "";
	char extension[12] = "_output.ppm";
	strcpy(outputFileName, filename);
	char* pos = strchr(outputFileName,'.');
	*pos = '\0';
	strcat(outputFileName,extension);

	// Create pipe
	int pipefd[2];
	pipe(pipefd);
	char pipeLocation[200];
	// Put write end of pipefd into pipeLocation 
	sprintf(pipeLocation, "%d", pipefd[1]);
	
	// Execute child process to read file
	int cpid = fork();
	int decoded;
	char codedString[60000];
	// Are we the child process?
	if(cpid == 0) {
		// If so, execute the FileReader
		execlp("./FileReader", "FileReader", filename, pipeLocation, NULL);
		exit(0);
	}
	else {
		printf("Starter: Forked process with ID %d.\n", cpid);
		// Let's hang and wait for child to complete task
		printf("Starter: Waiting for process [%d].\n", cpid);
		wait(&decoded);
	
		// 3. Close writing end, then read the child's work, close reading end
		close(pipefd[1]);
		read(pipefd[0], codedString, 60000);
		close(pipefd[0]);
	}	
	
	char *coded_values[5778];
	char *coded_value;	
	coded_value = strtok(codedString, " ");
	int i = 0;
	// Convert coded ints into a character array
	while(coded_value != NULL) {
		coded_values[i] = coded_value;
		i++;
		coded_value = strtok(NULL, " ");
	}

	int red_fd = shm_open("Shared_mem_red", O_CREAT | O_RDWR, 0666);
	printf("Shared_mem_red: %d\n", red_fd);
	int green_fd = shm_open("Shared_mem_green", O_CREAT | O_RDWR, 0666);
	printf("Shared_mem_green: %d\n", green_fd);
	int blue_fd = shm_open("Shared_mem_blue", O_CREAT | O_RDWR, 0666);
	printf("Shared_mem_blue: %d\n", blue_fd);
	
	// Set the size of the shared memory segments
	int size = 60000;
	ftruncate(red_fd, size);
	ftruncate(green_fd, size);
	ftruncate(blue_fd, size);
	
	// Create pointers to access the shared memory segments
	char *red_shmPtr = mmap(0, size, PROT_READ, MAP_SHARED, red_fd, 0); 
	char *green_shmPtr = mmap(0, size, PROT_READ, MAP_SHARED, green_fd, 0); 
	char *blue_shmPtr = mmap(0, size, PROT_READ, MAP_SHARED, blue_fd, 0); 

	pid_t cpids[3];
	coded_values[5777] = NULL;
	for(int j = 0;  j < 3; j++) {
		if(j == 0) { // This is the red process
			// Write shm name to array
			coded_values[5776] = "Shared_mem_red";	
			
			//Create child process and record pid for waiting later
			cpids[j] = fork();
			// If we are the child, execute new program to decode
			if(cpids[j] == 0)
				execvp("./Red", coded_values);	
		}
		else if(j == 1) { // Execute green process
			coded_values[5776] = "Shared_mem_green";	
			cpids[j] = fork();
			if(cpids[j] == 0)
				execvp("./Green", coded_values);	
		}
		else { // Execute blue process
			coded_values[5776] = "Shared_mem_blue";
			cpids[j] = fork();
			if(cpids[j] == 0)
				execvp("./Blue", coded_values);	
		}
		printf("Starter: Forked process with ID %d.\n", cpids[j]);
		printf("Starter: Waiting for process [%d].\n", cpids[j]);
			
	}

	// Extract the values into respective arrays
	int status;
	char* decodedString;
	int RED_VALUE[5776], GREEN_VALUE[5776], BLUE_VALUE[5776];

	for(int j = 0; j < 3; j++) {


		if(j == 0) {
			waitpid(cpids[j], &status, WCONTINUED);
			decodedString = strtok(red_shmPtr, " ");
			for(int i = 0; decodedString != NULL; ++i) { 
				RED_VALUE[i] = atoi(decodedString);
				decodedString = strtok(NULL, " ");
			}
		}
		else if(j == 1) {
			waitpid(cpids[j], &status, WCONTINUED);
			decodedString = strtok(green_shmPtr, " ");  
                        for(int i = 0; decodedString != NULL; ++i) {
                                GREEN_VALUE[i] = atoi(decodedString);
                                decodedString = strtok(NULL, " ");
                        }
		}
		else {
			waitpid(cpids[j], &status, WCONTINUED);
			decodedString = strtok(blue_shmPtr, " ");
                        for(int i = 0; decodedString != NULL; ++i) {
                                BLUE_VALUE[i] = atoi(decodedString);
                                decodedString = strtok(NULL, " ");
                        }
		}
	
	}

	// Unlink shared memory, as is tradition
	shm_unlink("Shared_mem_red");
	shm_unlink("Shared_mem_green");
	shm_unlink("Shared_mem_blue");
	
	// Output color bytes to ppm file
        // First, open the file for writing as binary with fopen
        FILE *fout = fopen(outputFileName, "wb");
        // Write magic number to first line of file
        fprintf(fout, "P6\n%i %i 255\n", IMAGE_HEIGHT, IMAGE_WIDTH);
        // Write out all bytes to the file
        for (int i=0; i<IMAGE_HEIGHT*IMAGE_WIDTH; i++){
                fputc(RED_VALUE[i], fout); // 0 .. 255 
                fputc(GREEN_VALUE[i], fout); // 0 .. 255 
                fputc(BLUE_VALUE[i], fout); // 0 .. 255 
        }
        // Close the picture file
        fclose(fout);
        printf("Starter: %s file written and closed.\n", outputFileName);

	return 0;
}


