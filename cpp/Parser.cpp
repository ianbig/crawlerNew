//
// Created by Ian Liu on 2020/4/7.
//

#include "Parser.h"


namespace pt = boost::property_tree;
std::map<std::string, int> stack;

Parser::Parser() {
    Size_OF_RecordQueue = sizeof(std::vector<struct MemoryStruct>);
    size_parser_master_cache = sizeof(std::vector<std::string>);
    whiteList.push_back("https:\\/\\/star.ettoday\\.net[\\/|\\w\\/.\\?|=]+");
    whiteList.push_back("https:\\/\\/www.ettoday\\.net[\\/|\\w\\/.\\?|=]+\\.htm[?|=|\\w]*");
    blackList.push_back(".*ettoday.*");
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
        curl_slist_free_all(plist);
        curl_easy_cleanup(fetcher);
    }
    curl_global_cleanup();
    /* ======================== */

    /* start to extract important infortmation: link, maintex, title*/
    if(chunk.buffer == NULL) return ERROR_NO_CONTENT_FETCH;
    extractLink(chunk.buffer);
    if(chunk.flag == true)
        mainTexExtraction(chunk.buffer, chunk.url);

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
    }
    return 0;
}

int Parser::mainTexExtraction(char *buffer, char *url) {
    TidyBuffer output = {0};
    TidyBuffer errbuf = {0};
    int rc = -1;
    bool ok;
    ctmbstr encoding = "utf8";


    // convert html to xhtml
    TidyDoc doc = tidyCreate(); // Initialize "document"
    ok = tidyOptSetBool(doc, TidyXmlOut, yes); // Convert to XHTML
    tidySetCharEncoding(doc,encoding);
    
    if(ok)
        rc = tidySetErrorBuffer(doc, &errbuf); // Capture diagnostics
    if(rc >= 0 ) {
        rc = tidyParseString(doc, buffer); // Parse the input
    }
    if ( rc >= 0 )
        rc = tidyCleanAndRepair( doc ); // Tidy it up!
    if(rc >= 0)
        rc = tidyRunDiagnostics(doc);
    if( rc > 1)
        rc = ( tidyOptSetBool(doc, TidyForceOutput, yes) ? rc : -1 ); // If error, force output.
    if ( rc >= 0 )
        rc = tidySaveBuffer( doc, &output );          // Pretty Print
    
   
    // parse DOM and compute textdensity (Ci / Ti)
    boost::property_tree::ptree tree;
    std::stringstream ss;
    ss << output.bp;

    try {
        boost::property_tree::xml_parser::read_xml(ss, tree);
    }

    catch(const boost::property_tree::xml_parser::xml_parser_error& ex) {
        std::cerr << ex.message() << std::endl;
    }

    // create new dom tree and extract content
    tree_node *root;
    root = create_dom_tree(tree, "root"); // root means on top of tree = first call of function
    DOM_tree droot(root);
    droot.contentExtraction();

    //std::cerr << droot.content_buffer << std::endl;
    
    // push information into record queue
    struct MemoryStruct tmp_chunk;

    tmp_chunk.size = strlen(buffer);
    tmp_chunk.buffer = new char[droot.content_buffer.length() + 1];
    memmove(tmp_chunk.buffer, droot.content_buffer.c_str(), droot.content_buffer.length());
    tmp_chunk.buffer[droot.content_buffer.length()] = 0;
    tmp_chunk.url = new char[strlen(url) + 1];
    memmove(tmp_chunk.url, url, strlen(url));
    tmp_chunk.url[strlen(url)] = 0;
    RecordQueue.push_back(tmp_chunk);

    Size_OF_RecordQueue += sizeof(struct MemoryStruct);
    
    // free up memory for maintex extraction

    delete_node(root);
    tidyBufFree( &output );
    tidyBufFree( &errbuf );
    tidyRelease( doc );

    return 0;
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
        fout << "@\n" << "@REC:" << std::endl << "@url:" << chunk.url << std::endl
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

size_t Parser::get_record_queue_size()  {
    return Size_OF_RecordQueue;
}

size_t Parser::get_record_amount() {
    return RecordQueue.size();
}

size_t Parser::get_master_parser_size() {
    return size_parser_master_cache;
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t real_size = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct*) userp;
    char *tmp = new char[mem->size];
    if(mem->buffer != NULL) {
        memmove(tmp, mem->buffer, mem->size);
        delete [] mem->buffer;
    }
    mem->buffer = new char[mem->size + real_size + 1];
    memmove(mem->buffer, tmp, mem->size);
    memmove(&(mem->buffer[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->buffer[mem->size] = 0;
    delete [] tmp;

    return real_size;
}

size_t findCaseInsensitive(std::string data, std::string toSearch) {
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
    return data.find(toSearch);
}