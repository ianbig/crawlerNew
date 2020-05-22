#include "crawler.h"

int main() {

    crawler master;
    timeval start, end;
    gettimeofday(&start,  NULL);
    master.start();
    gettimeofday(&end, NULL);
    std::cout << "Crawel Time: "<< (double) ( TV_PER_SEC * (end.tv_sec-start.tv_sec) + (end.tv_usec - start.tv_usec) ) / TV_PER_SEC;
}