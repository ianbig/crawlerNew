CC = g++
OBJ = main.o crawler.o Parser.o DataBase.o
REF_FILE = md5/src/md5.cpp
CFLAG = -g
LCFLAG = -lcurl -lmysqlcppconn8
INCLUDE = /usr/local/include
LIB = /usr/local/lib
CXX_STANDARD = -std=c++11
all:crawler
crawler: $(OBJ)
	$(CC) -o ../crawler $(CXX_STANDARD) $(REF_FILE) $(OBJ)  $(CFLAG) -I$(INCLUDE) -L$(LIB) $(LCFLAG)  -D_GLIBCXX_USE_CXX11_ABI=0
DataBase.o: DataBase.cpp DataBase.h
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) DataBase.cpp -D_GLIBCXX_USE_CXX11_ABI=0
crawler.o: crawler.cpp crawler.h
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) crawler.cpp -D_GLIBCXX_USE_CXX11_ABI=0
Parser.o: Parser.h Parser.cpp
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE)  Parser.cpp -D_GLIBCXX_USE_CXX11_ABI=0
main.o: main.cpp
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) main.cpp -D_GLIBCXX_USE_CXX11_ABI=0


.PHONY: clean
clean:
	rm *.o
