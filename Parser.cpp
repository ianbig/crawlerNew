//
// Created by Ian Liu on 2020/4/7.
//

#include "Parser.h"

Parser::Parser() {
    Size_OF_RecordQueue = 0;
    whiteList.push_back("https:\\/\\/[\\w]*.ettoday\\.net[\\/|\\w\\/.\\?|=]+");
    blackList.push_back("facebook");
    blackList.push_back("instagram");
}

// 0 means success fetch; -1 means blackList website not fetch
int Parser::fetch(const char *url) {
    CURL *fetcher = NULL;
    struct MemoryStruct chunk;
    std::string tmpList;
    // initialize chunk;

    chunk.url = new char[strlen(url) + 1];
    memmove(chunk.url, url, strlen(url));
    chunk.url[strlen(url)] = 0;

    // see weather to save this chunk
    for(int i = 0; i < whiteList.size(); i++) {
        tmpList = whiteList.at(i);
        if( std::regex_match(url, std::regex(tmpList)) ) chunk.flag = true;
    }

    // not fetch black List
    for(int i = 0; i < blackList.size(); i++) {
        tmpList = blackList.at(i);
        if( findCaseInsensitive(url, tmpList) != std::string::npos )
            return -1;
    }

    /* fetch the html into memory */
    curl_global_init(CURL_GLOBAL_SSL);
    fetcher = curl_easy_init();

    if(fetcher) {
        curl_easy_setopt(fetcher, CURLOPT_URL, url);
        curl_easy_setopt(fetcher, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(fetcher, CURLOPT_WRITEDATA, &chunk);
        curl_slist *plist = NULL;
        plist = curl_slist_append(plist, "USER_AGENT: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36\
                                          (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36");
        curl_easy_setopt(fetcher, CURLOPT_HTTPHEADER, plist);
        curl_easy_perform(fetcher);
        curl_easy_cleanup(fetcher);
    }
    curl_global_cleanup();
    /* ======================== */

    /* start to extract important infortmation: link, maintex, title*/
    extractLink(chunk.buffer);
   // mainTexExtration(chunk.buffer);
    int i = 0;

    while (i < Parser_Master_Cache.size()) {
        std::cout << Parser_Master_Cache.at(i) << std::endl;
        i++;
    }



    if(chunk.flag == true) {
        RecordQueue.push_back(chunk);
        Size_OF_RecordQueue += chunk.size;
        std::ofstream fs;
        fs.open("./test.html", std::ios::out);
        fs << chunk.buffer;
    }

    else {
        std::cout << chunk.url << std::endl;
    }
    return 0;
}

// test + extract link
size_t Parser::extractLink(char *buffer) {
    std::string pattern("<a.*href=\"([\\w | : |\\/ | . | \\% | ? | =]*)\"(.*)");
    std::regex reg(pattern);
    std::string strbuf(buffer);
    std::string url;
    std::smatch sm;
    auto result_start = std::sregex_iterator(strbuf.begin(),strbuf.end(),reg);
    auto result_end = std::sregex_iterator();

    for(auto it = result_start; it != result_end; ++it) {
        sm = *it;
        url = sm.str(1);

        if(std::regex_match(url, std::regex("\\/news[\\/|\\d|.|\\w|?|=]*"))) {
            url = "https://www.ettoday.net" + url;
        }

        if( tb.contain(url) < 0) {
            tb.insert(url);
            Parser_Master_Cache.push_back(url);
            std::cout << Parser_Master_Cache.back()<< std::endl;
        }
    }
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct*) userp;
    char *tmp = new char[mem->size];
    if(mem->buffer != NULL) {
        memmove(tmp, mem->buffer, mem->size);
        delete [] mem->buffer;
    }
    mem->buffer = new char[mem->size + realsize + 1];
    memmove(mem->buffer, tmp, mem->size);
    memmove(&(mem->buffer[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->buffer[mem->size] = 0;

    return realsize;
}

size_t findCaseInsensitive(std::string data, std::string toSearch) {
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
    return data.find(toSearch);
}