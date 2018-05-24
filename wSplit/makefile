CC = g++-7
CXXFLAGS = -O3 -liconv

CJKTokenizer.o: CJKTokenizer.cpp CJKTokenizer.h Dictionary.h Word.h TST.h TSTNode.h WordGraph.h
	$(CC) $(CXXFLAGS) -c CJKTokenizer.cpp -o CJKTokenizer.o

Dictionary.o: Dictionary.cpp Dictionary.h TST.h TSTNode.h Word.h
	$(CC) $(CXXFLAGS) -c Dictionary.cpp -o Dictionary.o

Word.o: Word.cpp Word.h
	$(CC) $(CXXFLAGS) -c Word.cpp -o Word.o

WordGraph.o: WordGraph.cpp WordGraph.h Word.h Dictionary.h
	$(CC) $(CXXFLAGS) -c WordGraph.cpp -o WordGraph.o

main.o: main.cpp CJKTokenizer.h Dictionary.h
	$(CC) $(CXXFLAGS) -c main.cpp -o main.o

clean:
	rm -rf ./main *.o

reb:
	make clean
	make

run:
	make reb
	./main