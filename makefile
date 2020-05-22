CC = g++
OBJ = main.o crawler.o parser.o database.o
REF_FILE = md5/src/md5.cpp
CFLAG = -g -lmysqlcppconn8 -lcurl
INCLUDE = /usr/local/include
LIB = /usr/local/lib
CXX_STANDARD = -std=c++17

all:crawler
crawler: $(OBJ)
	$(CC) -L$(LIB) $(CFLAG) $(REF_FILE) $(OBJ) -g -o crawler
main.o: main.cpp crawler.h
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) main.cpp
database.o: DataBase.cpp DataBase.h
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) DataBase.cpp
parser.o: Parser.h Parser.cpp
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE)  Parser.cpp
crawler.o: crawler.cpp crawler.h
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) crawler.cpp

.PHONY: clean
clean:
	rm *.o