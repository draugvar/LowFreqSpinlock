LOW=low_test.c ../LowFreqSpinlock/low_freq_spinlock.c ../LowFreqModule/ioctl.c
NORMAL=normal_test.c ../Utility/normal_spinlock.c

CC=gcc

CFLAGS=#-g -Wall
LIBS=-lpthread

LOW_TEST=lowfreq
NORMAL_TEST=normal


$(LOW_TEST):$(LOW)
	$(CC) $(CFLAGS) -o $(LOW_TEST) $(LOW) $(LIBS)

	@echo " "
	@echo "     ### Compilazione completata! ###     "
	@echo " "

$(NORMAL_TEST):$(NORMAL)
	$(CC) $(CFLAGS) -o $(NORMAL_TEST) $(NORMAL) $(LIBS)

	@echo " "
	@echo "     ### Compilazione completata! ###     "
	@echo " "

clean:
	rm -f *.o
	rm -f core
