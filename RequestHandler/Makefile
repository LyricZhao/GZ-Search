WPATH = ../wSplit
PACKAGE = context.o database.o requestCore.o main.o
WORDPACK = $(WPATH)/CJKTokenizer.o $(WPATH)/Dictionary.o $(WPATH)/Word.o $(WPATH)/WordGraph.o
GPPFLAGS = -O2 -std=c++11 -fopenmp -I /usr/local/Cellar/berkeley-db/6.2.32/include -L/usr/local/Cellar/berkeley-db/6.2.32/lib -ldb_cxx
LIBFLAGS = -L/usr/local/lib -lzmq -fopenmp -liconv -I /usr/local/Cellar/berkeley-db/6.2.32/include -L/usr/local/Cellar/berkeley-db/6.2.32/lib -ldb_cxx
DEL = rm -rf
CXX = g++-7

default:
	make main

%.o: %.cpp %.h
	$(CXX) -c $(GPPFLAGS) $(LIBFLAGS) $*.cpp -o $*.o

main: $(PACKAGE) $(WORDPACK)
	$(CXX) $(WORDPACK) $(PACKAGE) $(LIBFLAGS) -o main

wSplit:
	make -C ../wSplit

run:
	make main
	./main

clean:
	-$(DEL) *.o
	-$(DEL) ./main
	-$(DEL) $(WPATH)/*.o

cr:
	make run

rebuild:
	make clean
	make

rerun:
	make rebuild
	make run
