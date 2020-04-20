//
// Created by Ian Liu on 2020/4/7.
//

#include <gumbo.h>
#include "Parser.h"

Parser::Parser() {
    Size_OF_RecordQueue = sizeof(std::vector<std::string>);
    size_parser_master_cache = sizeof(std::vector<std::string>);
    whiteList.push_back("https:\\/\\/star.ettoday\\.net[\\/|\\w\\/.\\?|=]+");
    whiteList.push_back("https:\\/\\/www.ettoday\\.net[\\/|\\w\\/.\\?|=]+\\.htm[?|=|\\w]*");
    blackList.push_back(".*ettoday.*");
}

size_t Parser::get_record_queue_size()  {
    return Size_OF_RecordQueue;
}

size_t Parser::get_record_amount() {
    return RecordQueue.size();
}

size_t Parser::get_master_parser_size() {
    return size_parser_master_cache;
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
        if( !std::regex_match(url, std::regex(tmpList)) )
            return ERROR_BLACKLIST_BLOCK;
    }

    /* fetch the html into memory */
    curl_global_init(CURL_GLOBAL_SSL);
    fetcher = curl_easy_init();

    if(fetcher) {
        curl_easy_setopt(fetcher, CURLOPT_URL, url);
        curl_easy_setopt(fetcher, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(fetcher, CURLOPT_WRITEDATA, &chunk);
        curl_slist *plist = NULL;
        plist = curl_slist_append(plist, "USER_AGENT: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36 \
                                          (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36");
        curl_easy_setopt(fetcher, CURLOPT_HTTPHEADER, plist);
        curl_easy_perform(fetcher);
        curl_easy_cleanup(fetcher);
    }
    curl_global_cleanup();
    /* ======================== */

    /* start to extract important infortmation: link, maintex, title*/
    if(chunk.buffer == NULL) return ERROR_NO_CONTENT_FETCH;
    extractLink(chunk.buffer);
    if(chunk.flag == true)
        mainTexExtration(chunk.buffer, chunk.url);


    return FETCH_SUCCESS;
}

// test + extract link
size_t Parser::extractLink(char *buffer) {
    std::string pattern("<a.*href=\"([\\w | : |\\/ | . | \\% | ? | =]*)\"(.*)");
    std::regex reg(pattern);
    std::string strbuf(buffer);
    std::string url;
    std::smatch sm;
    std::string md5;
    auto result_start = std::sregex_iterator(strbuf.begin(),strbuf.end(),reg);
    auto result_end = std::sregex_iterator();

    for(auto it = result_start; it != result_end; ++it) {
        sm = *it;
        url = sm.str(1);

        if(std::regex_match(url, std::regex("\\/news[\\/|\\d|.|\\w|?|=]*"))) {
            url = "https://www.ettoday.net" + url;
        }
        else if(std::regex_match(url, std::regex("\\/\\/www\\.ettoday\\.net\\/news[\\/|\\d|.|\\w|?|=]*"))) {
            url = "https:" + url;
        }

        md5 = MD5(url).toStr();

        if( tb.query(md5) == false) {
            tb.insert(md5);
            Parser_Master_Cache.push_back(url);
            size_parser_master_cache += url.length();
        }
        //else std::cout << "ERROR: already in seendb" << std::endl;
    }
}

int Parser::mainTexExtration(char *buffer, char *url) {
    if(buffer == NULL)
        return ERROR_MAINTEX_NULL;
    GumboOutput *output = gumbo_parse(buffer);
    getMainTex(output->root, url);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

GumboNode* Parser::diveNode(GumboNode *child_root, const char *desiredValue) {
    GumboNode *child_next_level;
    GumboAttribute *value;

    for(int i = 0; i < child_root->v.element.children.length; i++) {
        child_next_level = (GumboNode*)child_root->v.element.children.data[i];
        if(child_next_level->v.element.tag == GUMBO_TAG_DIV || child_next_level->v.element.tag == GUMBO_TAG_H1) {
            if(child_next_level->type == GUMBO_NODE_ELEMENT && (value = gumbo_get_attribute(&child_next_level->v.element.attributes, "class") ) != NULL) {
                // std::cout << (char*)value->value << std::endl;
                if(strcmp((char*)value->value, desiredValue) == 0) {
                    //std::cout << (char*)value->value << std::endl;
                    return child_next_level;
                }
            }
        }
    }
    return NULL;
}

std::string Parser::cleantext(GumboNode* node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT &&
               node->v.element.tag != GUMBO_TAG_SCRIPT &&
               node->v.element.tag != GUMBO_TAG_STYLE ) {
        std::string contents = "";
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = cleantext((GumboNode*) children->data[i]);
            if (i != 0 && !text.empty()) {
                contents.append(" ");
            }
            contents.append(text);
        }
        return contents;
    } else {
        return "";
    }
}

// maintex && title
void Parser::textExtract(GumboNode *root, char *url) {
    std::string title;
    std::string context;
    GumboNode *level_ptr;
    GumboNode *text_parse;
    MemoryStruct chunk;

    for (int i = 0; i < root->v.element.children.length; i++) {
        level_ptr = (GumboNode *) root->v.element.children.data[i];
        if (level_ptr->v.element.tag == GUMBO_TAG_HEADER) {
            level_ptr = diveNode(level_ptr, "title");
            GumboNode *title_text = (GumboNode *) level_ptr->v.element.children.data[0];
            assert(title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE);
            title = title_text->v.text.text;
        }
    }

    text_parse = diveNode(root, "story");
    assert(text_parse != NULL);
    context = cleantext(text_parse);
    //std::cout << context << std::endl;


    chunk.buffer = new char[context.size() + 1];
    chunk.title = new char[title.size() + 1];
    chunk.url = new char[strlen(url) + 1];
    memmove(chunk.buffer, context.c_str(), context.size());
    chunk.buffer[context.size()] = 0;
    memmove(chunk.title, title.c_str(), title.size());
    chunk.title[title.size()] = 0;
    memmove(chunk.url, url ,strlen(url));
    chunk.url[strlen(url)] = 0;
    chunk.size = strlen(chunk.buffer);

    RecordQueue.push_back(chunk);
    Size_OF_RecordQueue += sizeof(chunk) + strlen(chunk.buffer);
}

void Parser:: getMainTex(GumboNode *root, char *url) {
    GumboVector *children = &root->v.element.children;
    GumboNode* body_root;
    GumboNode *level_ptr;
    GumboAttribute *value;
    GumboNode *c1_ptr;
    bool mainFlag = false; // main article in ettoday

    for(int i = 0; i < children->length; i++) {
        body_root = (GumboNode*)children->data[i];
        if(body_root->v.element.tag == GUMBO_TAG_BODY) {
            break;
        }
    }
    assert(body_root->v.element.tag == GUMBO_TAG_BODY);
    if(!std::regex_match(url,
            std::regex("https:\\/\\/sports\\.ettoday\\.net[\\/|\\w\\/.\\?|=]+")) ) {
        level_ptr = diveNode(body_root, "wrapper_box");
    }
    else level_ptr = body_root;
    if(level_ptr) level_ptr = diveNode(level_ptr, "wrapper");
    if(level_ptr) level_ptr = diveNode(level_ptr, "container_box");
    if(level_ptr) level_ptr = diveNode(level_ptr, "container");
    if(level_ptr) level_ptr = diveNode(level_ptr, "r1 clearfix");
    if(level_ptr) level_ptr = diveNode(level_ptr, "c1");

    if(level_ptr == NULL) return;

    value = gumbo_get_attribute(&level_ptr->v.element.attributes, "class");
    assert(strcmp((char*)value->value,"c1") == 0);
    if( std::regex_match(url,
                         std::regex("https:\\/\\/www\\.ettoday\\.net[\\/|\\w\\/.\\?|=]+") )
                         || std::regex_match(url, std::regex("https:\\/\\/sports\\.ettoday\\.net[\\/|\\w\\/.\\?|=]+")) ) {
        if(level_ptr) level_ptr = diveNode(level_ptr, "part_area_1");
        mainFlag = true;
        if(level_ptr == NULL)
            return;
        value = gumbo_get_attribute(&level_ptr->v.element.attributes, "class");
        assert(strcmp((char*)value->value,"part_area_1") == 0);
    }

    for(int i = 0; i < level_ptr->v.element.children.length; i++) {
        c1_ptr = (GumboNode*)level_ptr->v.element.children.data[i];
        if(c1_ptr->v.element.tag == GUMBO_TAG_ARTICLE && c1_ptr->type == GUMBO_NODE_ELEMENT) {
            if(mainFlag == false) c1_ptr = diveNode(c1_ptr, "module_1");

            else {
                c1_ptr = diveNode(c1_ptr, "subject_article");
            }
            textExtract(c1_ptr, url);
            break;
        }
    }
}

void Parser::write_out_record(std::string filePath) {

    if(RecordQueue.empty()) {
        std::cerr << "record queue is empty" << std::endl;
        return;
    }

    std::ofstream fout;
    struct MemoryStruct chunk;
    fout.open(filePath, std::ios::out | std::ios::app);
    for(int i = 0; i < RecordQueue.size(); i++) {
        chunk = RecordQueue.at(i);
        fout << "@\n" << "@REC:" << std::endl << "@url:" << chunk.url << std::endl <<"@title:" << chunk.title
        << std::endl << "@size:" << chunk.size << std::endl <<"@body:" << chunk.buffer << std::endl;
    }
    fout.close();
    std::cerr << "Record write out" << std::endl;
}

void Parser::write_out_url(std::string filePath) {
    std::ofstream fout;
    fout.open(filePath, std::ios::out);
    std::string tmp;
    for(int i = 0; i < Parser_Master_Cache.size(); i++ ) {
        tmp = Parser_Master_Cache.at(i);
        fout << tmp << std::endl;
    }
    fout.close();
    std::cerr << "write out url" << std::endl;
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