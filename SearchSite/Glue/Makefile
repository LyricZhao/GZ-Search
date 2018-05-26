WPATH = ../../wSplit
PACKAGE = context.o database.o requestCore.o CXXHandler.o CXXToPython.o
WORDPACK = $(WPATH)/CJKTokenizer.o $(WPATH)/Dictionary.o $(WPATH)/Word.o $(WPATH)/WordGraph.o
GPPFLAGS = -O2 -std=c++11 -fopenmp -I /usr/local/Cellar/berkeley-db/6.2.32/include -L/usr/local/Cellar/berkeley-db/6.2.32/lib -ldb_cxx
LIBFLAGS = -L/usr/local/lib -lzmq -fopenmp -liconv -I /usr/local/Cellar/berkeley-db/6.2.32/include -L/usr/local/Cellar/berkeley-db/6.2.32/lib -ldb_cxx
DEL = rm -rf
CXX = g++-7
LIB = CXXHandler.so

All: $(LIB)
$(LIB): $(PACKAGE) $(WORDPACK)
	g++-7 -fPIC -shared $(LIBFLAGS) $(PACKAGE) $(WORDPACK) -o $(LIB)

%.o: %.cpp $(HEADERS)
	g++-7 -c -fPIC $(GPPFLAGS) $< -o $@

clean:
	-rm -rf *.o
	-rm -rf $(WPATH)/*.o
