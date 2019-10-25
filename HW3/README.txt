1. Name the function that is used to create a pipe. Which ends denotes the read and the write
ends of a pipe? (2 points)
	pipe(int pipefd[2]); read end is at pipefd[0] and write end is pipefd[1]
2. Name the function used to map files or devices in to memory? (1 point)
	mmap()
3. Name the function used to open a shared memory object? What does it return? (2 points)
	shm_open returns -1 for an error or a file descriptor on success
