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


void convertArrToString(char** strings){
	int coded;
        char codedString[30];    
        char allCodedStrings[60000]; 
        // Store the coded input number into coded         
	for(int i = 0; i < 5778; i++) {
		strcat(allCodedStrings, strcat(strings[i], " "));
        }
	printf("result: %s", allCodedStrings);
	strings[0] = allCodedStrings;
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
		// Let's hang and wait for child to complete task
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
	int green_fd = shm_open("Shared_mem_red", O_CREAT | O_RDWR, 0666);
	int blue_fd = shm_open("Shared_mem_red", O_CREAT | O_RDWR, 0666);

	int size = 60000;
	result = ftruncate(red_fd, size);
	result = ftruncate(green_fd, size);
	result = ftruncate(blue_fd, size);
	
	int *red_shmPtr = (int *)mmap(0, size, PROT_READ, MAP_SHARED, red_fd, 0); 
	int *green_shmPtr = (int *)mmap(0, size, PROT_READ, MAP_SHARED, green_fd, 0); 
	int *blue_shmPtr = (int *)mmap(0, size, PROT_READ, MAP_SHARED, blue_fd, 0); 

	pid_t cpids[3];

	for(int j = 0;  j < 3; j++) {
		if(j == 0) {
			// Write shm name to array
			coded_values[5776] = "Shared_mem_red";	

			//Create child process and record pid for waiting later
			cpids[j] = fork();
			// If we are the child, execute new program to decode
			printf("before: %s", coded_values[0]);
			convertArrToString(coded_values);
			if(cpids[j] == 0)
				execlp("./Red", "Red", codedString[0], NULL);	
			else
				wait(&result);//break;
		}
		else if(j == 1) {
			coded_values[5776] = "Shared_mem_green";
			cpids[j] = fork();
			if(cpids[j] == 0)
				execlp("./Green", "Green", coded_values, NULL);	
			else
				break;
		}
		else {
			coded_values[5776] = "Shared_mem_blue";
			cpids[j] = fork();
			if(cpids[j] == 0)
				execlp("./Blue", "Blue", coded_values, NULL);	
			else
				break;
		}
			
	}
	
	int status;
	for(int j = 0; j < 3; j++) {
		result = waitpid(cpids[j], &status, WCONTINUED);
	}

	result = shm_unlink("Shared_mem_red");
	result = shm_unlink("Shared_mem_green");
	result = shm_unlink("Shared_mem_blue");
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
			printf("Starter: Waiting for process [%d].\n", cpid);
			wait(&decodedValue);
			*blue = WEXITSTATUS(decodedValue);
			printf("Starter: Child process %d returned %d.\n", cpid, *blue);
		}
		else
			printf("ERROR");
	
		return 0;
}
