WPATH = ../wSplit
WORDPACK = $(WPATH)/CJKTokenizer.o $(WPATH)/Dictionary.o $(WPATH)/Word.o $(WPATH)/WordGraph.o
PACKAGE = CoreHandler.o inputHandler.o prComputer.o networkRecv.o dataBaseWriter.o utility.o database.o graph.o $(WORDPACK)
GPPFLAGS = -O2 -std=c++11 -fopenmp -I /usr/local/Cellar/berkeley-db/6.2.32/include -L/usr/local/Cellar/berkeley-db/6.2.32/lib -ldb_cxx
LIBFLAGS = -L/usr/local/lib -lzmq -fopenmp -liconv -I /usr/local/Cellar/berkeley-db/6.2.32/include -L/usr/local/Cellar/berkeley-db/6.2.32/lib -ldb_cxx
DEL = rm -rf
CXX = g++-7

default:
	make CoreHandler

%.o: %.cpp %.h
	$(CXX) -c $(GPPFLAGS) $(LIBFLAGS) $*.cpp -o $*.o

CoreHandler: $(PACKAGE)
	$(CXX) $(PACKAGE) $(LIBFLAGS) -o CoreHandler

wSplit:
	make -C ../wSplit

run:
	make CoreHandler
	./CoreHandler

clean:
	-$(DEL) *.o
	-$(DEL) ./CoreHandler
	-$(DEL) ./check
	-$(DEL) $(WPATH)/*.o

cdb:
	-$(DEL) *.db

cr:
	make cdb
	make run

rebuild:
	make clean
	make

rerun:
	./kill.sh
	make rebuild
	make cr

mch: $(WORDPACK) check.o
	$(CXX) $(WORDPACK) utility.o check.o $(LIBFLAGS) -o check

runc:
	rm -rf check.o
	make mch
	./check
