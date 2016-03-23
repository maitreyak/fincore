EXE=fincore

default: $(EXE)

$(EXE): $(EXE).c
	$(CC) $(CFPLAGS) -o $(EXE) $(EXE).c 
