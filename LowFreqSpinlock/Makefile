OBJECTS=main.c low_freq_spinlock.c util.c

CC=gcc

CFLAGS=#-g -Wall
LIBS=#

PROGRAM_NAME=lowfreq_spin



$(PROGRAM_NAME):$(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(OBJECTS) $(LIBS)

	@echo " "
	@echo "     ### Compilazione completata! ###     "
	@echo " "

clean:
	rm -f *.o
	rm -f core
