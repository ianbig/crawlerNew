#include "master.h"

int main() {

    Master master;
    timeval start, end;
    gettimeofday(&start,  NULL);
    master.start();
    gettimeofday(&end, NULL);
    std::cout << (double) ( TV_PER_SEC * (end.tv_sec-start.tv_sec) + (end.tv_usec - start.tv_usec) ) / TV_PER_SEC;
}

// stars
// www
// current craweled amount: 2281 / 1216 1132 1200 117 1159(946) 1191(973) 1068(956.623)
// memory leak: clang-10 compiler not compatible 
// convert to json
// cython to use solr pyhon api