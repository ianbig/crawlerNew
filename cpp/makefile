CC = g++
OBJ = main.o crawler.o Parser.o DataBase.o tree.o
REF_FILE = md5/src/md5.cpp
CFLAG = -g
LCFLAG = -lcurl -lmysqlcppconn8 -ltidy
INCLUDE = mysql-connector-c++-8.0.20-linux-glibc2.12-x86-64bit/include/
LIB = mysql-connector-c++-8.0.20-linux-glibc2.12-x86-64bit/lib64
CXX_STANDARD = -std=c++11
all:crawler
crawler: $(OBJ)
	$(CC) -O0 -o ../crawler $(CXX_STANDARD) $(REF_FILE) $(OBJ)  $(CFLAG) -I$(HOME)/$(INCLUDE) -L$(HOME)/$(LIB) $(LCFLAG)  -D_GLIBCXX_USE_CXX11_ABI=0
DataBase.o: DataBase.cpp DataBase.h
	$(CC) -O0 -c $(CXX_STANDARD) -I$(HOME)/$(INCLUDE) DataBase.cpp -D_GLIBCXX_USE_CXX11_ABI=0
crawler.o: crawler.cpp crawler.h
	$(CC) -O0 -c $(CXX_STANDARD) -I$(HOME)/$(INCLUDE) crawler.cpp -D_GLIBCXX_USE_CXX11_ABI=0
Parser.o: Parser.h Parser.cpp
	$(CC) -O0 -c $(CXX_STANDARD) -I$(HOME)/$(INCLUDE)  Parser.cpp -D_GLIBCXX_USE_CXX11_ABI=0
tree.o: tree.h tree.cpp
	$(CC) -O0 -c $(CXX_STANDARD) -I$(HOME)/$(INCLUDE)  tree.cpp -D_GLIBCXX_USE_CXX11_ABI=0
main.o: main.cpp
	$(CC) -O0 -c $(CXX_STANDARD) -I$(HOME)/$(INCLUDE) main.cpp -D_GLIBCXX_USE_CXX11_ABI=0


.PHONY: clean
clean:
	rm *.o
