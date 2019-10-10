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
	$(GCC) -o Red $(LD_FLAGS) Red.c 

Blue: Blue.c 
	$(GCC) -o Blue $(LD_FLAGS) Blue.c

Green: Green.c
	$(GCC) -o Green $(LD_FLAGS) Green.c

FileReader:
	$(GCC) -o FileReader $(LD_FLAGS) FileReader.c

Starter: Starter.c
	$(GCC) -o Starter $(LD_FLAGS) Starter.c

build: Red Blue Green FileReader Starter

# Recompile C objects if headers change
${OBJS}:      ${HEADERS}

# Clean up the directory
clean:
	rm -f $(OBJS) *~ $(EXE)

package:
	tar -cvzf Casey-Key-HW3.tar *.c *.h *.txt Makefile
