SRC = sigperf1.c sigshooter1.c
PRG = sigperf1 sigshooter1

all: ${PRG}

sigperf2:
	cc sigperf1.c -o sigperf1

sigshooter2:
	cc sigshooter1.c -o sigshooter1

man:
	xterm -e less sigs.text

clean:
	/bin/rm -f core sigperf1 sigshooter1