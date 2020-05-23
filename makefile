CC = g++
OBJ = main.o crawler.o Parser.o DataBase.o
REF_FILE = md5/src/md5.cpp
CFLAG = -g -lmysqlcppconn8 -lcurl
INCLUDE = /usr/include
LIB = /home/ianliu/mysql-connector-c++-8.0.20-linux-glibc2.12-x86-64bit/lib64
CXX_STANDARD = -std=c++17

all:crawler
crawler: $(OBJ)
	$(CC) -L$(LIB) $(CFLAG) $(REF_FILE) $(OBJ) -o crawler
main.o: main.cpp
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) main.cpp
DataBase.o: DataBase.cpp DataBase.h
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) DataBase.cpp
Parser.o: Parser.h Parser.cpp
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE)  Parser.cpp
crawler.o: crawler.cpp crawler.h
	$(CC) -c $(CXX_STANDARD) -I$(INCLUDE) crawler.cpp

.PHONY: clean
clean:
	rm *.o
