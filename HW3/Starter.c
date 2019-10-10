// Watch helpdesk video before asking for help

#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define IMAGE_HEIGHT 76
#define IMAGE_WIDTH 76

//static int findThreeValues(int coded, int &red, int &green, int &blue);

// This function spawns three child processes, one for each color
static int findThreeValues(char coded[], int *red, int *green, int *blue) {
		// Store color byte in decodedValue
		int decodedValue;
		pid_t cpid = fork();
		// If we are in child process, run the Red program with coded as an argument
		if(cpid == 0) {
			execlp("./Red", "Red", coded, NULL); //(char) coded, NULL);	
		}
		// If we are parent, wait for child to finish processing
		else if (cpid > 0) {
			printf("Starter: Forked process with ID %d.\n", cpid);
			printf("Starter: Waiting for process [%d].\n", cpid);
			// We get exit value
			wait(&decodedValue);
			// // Find the value returned from Red
			*red = WEXITSTATUS(decodedValue);
			printf("Starter: Child process %d returned %d.\n", cpid, *red);
		}
		else
			printf("ERROR");
		
		// Repeat above procedure for Green
		cpid = fork();
		if(cpid == 0) {
			execlp("./Green", "Green", coded, NULL);	
		}
		else if (cpid > 0) {
			printf("Starter: Forked process with ID %d.\n", cpid);
			printf("Starter: Waiting for process [%d].\n", cpid);
			wait(&decodedValue);
			*green = WEXITSTATUS(decodedValue);
			printf("Starter: Child process %d returned %d.\n", cpid, *green);
		}
		else
			printf("ERROR");
		

		// Repeat procedure for Blue
		cpid = fork();
		if(cpid == 0) {
			execlp("./Blue", "Blue", coded, NULL);	
		}
		else if (cpid > 0) {
			
			printf("Starter: Forked process with ID %d.\n", cpid);
			printf("Starter: Waiting for process [%d].\n", cpid);
			wait(&decodedValue);
			*blue = WEXITSTATUS(decodedValue);
			printf("Starter: Child process %d returned %d.\n", cpid, *blue);
		}
		else
			printf("ERROR");
	
		return 0;

}


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
	int result = pipe(pipefd);
	char pipeLocation[200];
	// Put write end of pipefd into pipeLocation 
	sprintf(pipeLocation,  "%d", pipefd[1]);
	
	// Execute child process to read file
	int cpid = fork();
	// Are we the child process?
	if(cpid == 0) {
		// If so, execute the FileReader
		execlp("./FileReader", "FileReader", filename, pipeLocation, NULL);
	}
	else {
		// Let's hang and wait for child to complete task
		wait();
	}	
	/*	
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
	*/
	return 0;
}


