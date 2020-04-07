

#include "Parser.h"
#include "HashTable.h"
#include "md5/src/md5.h"

int main() {
    HashTable tb;
    std::string md5[10];
    std::string newm[5];

    md5[0] = "https://github.com/nostrademons/gumbo-libxml";
    md5[1] = "https://www.ettoday.net/";
    md5[2] = "https://curl.haxx.se/libcurl/c/getinmemory.html";
    md5[3] = "https://alrightchiu.github.io/SecondRound/hash-tablechaining.html";
    md5[4] = "https://github.com/google/gumbo-parser/blob/master/examples/get_title.c";

    for(int i = 0; i < 5; i++) {
        newm[i] = MD5(md5[i]).toStr();
        tb.insert(newm[i]);
    }

    for(int i; i < 5; i++) {
        if(tb.contain(newm[i])) std::cout << "contained !!!" << std::endl;
    }

    md5[4] = "https://www.youtube.com/";
    newm[4] = MD5("https://www.youtube.com/").toStr();
    if(tb.contain(newm[4]) != -1) std::cout << tb.contain(newm[4]) << std::endl;
    else std::cout << "Not found" << std::endl;
}