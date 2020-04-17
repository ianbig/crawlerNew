//
// Created by Ian Liu on 2020/4/7.
//

#include "master.h"

Master::Master() {
    line_length = 100;
}


int Master::start() {
    std::string tmp_url;
    size_t record_size = 0;
    size_t real_record_size = 0;
    bool first_crawel = true;

    while(record_size <= MAX_RECORD_SIZE) {
        Parser parser;
        std::ifstream fin;
        int file_check = 0;

        // commit seed to commitQueue
        if(first_crawel) {
            fin.open("./seed.log", std::ios::in);
            while(fin.getline(&(tmp_url[0]), line_length)) {
                commitQueue.push_back(tmp_url);
            }

            if(line_length == -1) {
                line_length = line_length * 2;
                commitQueue.clear();
                continue;
            }

            if(commitQueue.empty()) {
                std::cerr << "ERROR: Please feed URL in seed.log" << std::endl;
                return ERROR_SEED_NO_URL;
            }
            else first_crawel = false;
        }

        else {
            fin.open("./temp.log", std::ios::in);
            while(fin.getline(&(tmp_url[0]), line_length)) {
                commitQueue.push_back(tmp_url);
            }
        }

        while(! commitQueue.empty() && record_size <= MAX_RECORD_SIZE ) {
            tmp_url = commitQueue.front();
            parser.fetch(tmp_url.c_str());
            commitQueue.erase(commitQueue.begin());
            record_size = parser.get_record_queue_size();
        }

        real_record_size += record_size;
        if(!commitQueue.empty()) write_out_commit_queue("./DB/uncommit.log");
        parser.write_out_url("./DB/urlPool.log");
        parser.write_out_record("./ettoday.rec");

    }

}

// commit queue not write to file
// second crawel where to load file
// master_parser queue size need to consider as well

void Master::write_out_commit_queue(std::string filePath) {
    std::ofstream fout;
    std::string tmp;
    fout.open(filePath, std::ios::out);

    for(int i = 0; i < commitQueue.size(); i++) {
        tmp = commitQueue.at(i);
        fout << tmp << std::endl;
    }

    fout.close();
}