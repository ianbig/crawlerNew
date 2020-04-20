//
// Created by Ian Liu on 2020/4/7.
//

#include "master.h"

Master::Master() {
}

Master::~Master() {
    char *tmp;
    for(int i = 0; i < commitQueue.size(); i++) {
        tmp = commitQueue.at(i);
        delete [] tmp;
    }
}


int Master::start() {
    char *tmp_url;
    size_t real_record_size = 0;
    FILE *fstream = NULL;
    char *line = NULL;
    char *copy_line = NULL;
    size_t get_line_size = 0;
    bool record_size_not_exceed = true;
    timeval start, end;
    int parser_check;

    std::cout << "=========start craweling==========" << std::endl;

    while(real_record_size < MAX_RECORD) {
        // the reason for use the outer loop is to automatically refresh queue in parser
        Parser parser;
        int file_check = 0;
        size_t line_size = 0;
        size_t record_size = 0;
        size_t master_parser_size = 0;

        fstream = fopen("./DB/seed.log", "r");
        while ((file_check = getline(&line, &get_line_size, fstream) ) != -1) {
            // allocate memory
            line_size = strlen(line);
            copy_line = new char[line_size + 1];
            copy_line[line_size] = 0;
            memmove(copy_line, line, line_size);
            commitQueue.push_back(copy_line);
            free(line);
            line = NULL;
        }
        fclose(fstream);

        if(!record_size_not_exceed) {
            fstream = fopen("./DB/uncommit.log", "r");
            while ((file_check = getline(&line, &get_line_size, fstream) ) != -1) {
                // allocate memory
                line_size = strlen(line);
                copy_line = new char[line_size + 1];
                copy_line[line_size] = 0;
                memmove(copy_line, line, line_size);
                commitQueue.push_back(copy_line);
                free(line);
                line = NULL;
            }
            fclose(fstream);
        }

        if(commitQueue.empty()) {
            std::cerr << "ERROR: seed.log is empty" << std::endl;
            return  ERROR_SEED_EMPTY;
        }

        while(! commitQueue.empty() &&  ( record_size_not_exceed = (record_size <= MAX_RECORD_SIZE && master_parser_size <= MAX_URL_CAPACITY) ) ){
            tmp_url = commitQueue.front();
            tmp_url = strtok(tmp_url,"\n");
            if(tmp_url == NULL) {
                delete [] tmp_url;
                commitQueue.erase(commitQueue.begin());
                continue;
            }
            gettimeofday(&start, NULL);
            gettimeofday(&end, NULL);

            while(  (double) ( TV_PER_SEC * (end.tv_sec-start.tv_sec) + (end.tv_usec - start.tv_usec) ) / TV_PER_SEC  < FLOW_CONTROL ) {
                gettimeofday(&end, NULL);
            } // flow control
            std::cout << tmp_url << std::endl;
            parser_check = parser.fetch(tmp_url);
            delete [] tmp_url;
            commitQueue.erase(commitQueue.begin());
            record_size = parser.get_record_queue_size();
            master_parser_size = parser.get_master_parser_size();
        }

        real_record_size += parser.get_record_amount();
        if(record_size_not_exceed == false ) write_out_commit_queue("./DB/uncommit.log");
        parser.write_out_url("./DB/seed.log");
        parser.write_out_record("./ettoday.rec");

    }
    std::cout << "====== craweling finish =====" << std::endl;
    std::cout << "Current Stored Record: " << real_record_size << std::endl;
}


void Master::write_out_commit_queue(std::string filePath) {
    std::cerr << "Write out to uncommit Queue" << std::endl;
    std::ofstream fout;
    char *tmp;
    fout.open(filePath, std::ios::out);

    for(int i = 0; i < commitQueue.size(); i++) {
        tmp = commitQueue.front();
        tmp = strtok(tmp,"\n");
        if(tmp) fout << tmp << std::endl;
        delete [] tmp;
        commitQueue.erase(commitQueue.begin());
    }

    fout.close();
}