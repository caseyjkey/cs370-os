# List of files
SRCS          = Starter.c FileReader.c Red.c Green.c Blue.c  
OBJS          = Starter.o FileReader.o Red.o Green.o Blue.o
# HEADERS       = 	  

EXE           = Starter FileReader Red Green Blue 
DEFINES       = -DDEBUG -DHALF
# LIB           = HW1.a

# Compiler and loader commands and flags
GCC             = gcc
GCC_FLAGS       = -g -std=gnu99 -Wall -c -lrt 
LD_FLAGS        = -g -std=gnu99 -Wall -lrt

# Compile .c files to .o files
.c.o:
	$(GCC) $(GCC_FLAGS) $(DEFINES) $<

# Target is the executable
Red: Red.c
	$(GCC) Red.c  -o Red $(LD_FLAGS) 

Blue: Blue.c 
	$(GCC) Blue.c -o Blue $(LD_FLAGS)

Green: Green.c
	$(GCC) Green.c -o Green $(LD_FLAGS)

FileReader:
	$(GCC) FileReader.c -o FileReader $(LD_FLAGS)

Starter: Starter.c
	$(GCC) Starter.c -o Starter $(LD_FLAGS)

build: Red Blue Green FileReader Starter

# Recompile C objects if headers change
${OBJS}:      ${HEADERS}

# Clean up the directory
clean:
	rm -f $(OBJS) *~ $(EXE)

package:
	tar -cvzf Casey-Key-HW3.tar *.c *.txt Makefile
