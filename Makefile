EXE=fincore
CFLAGS+=-std=gnu99

default: $(EXE)

$(EXE): $(EXE).c
	$(CC) $(CFLAGS) -o $(EXE) $(EXE).c

clean:
	rm -rf *~ $(EXE)
