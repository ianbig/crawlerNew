#include "gumbo.h"
#include <sstream>
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::map<std::string, int> hash;

void tagCount( GumboNode *node ) {
    std::string tagname = gumbo_normalized_tagname(node->v.element.tag);
    std::map<std::string, int>::iterator it;

    if(node->type == GUMBO_NODE_ELEMENT &&
    node->v.element.tag != GUMBO_TAG_SCRIPT &&  node->v.element.tag != GUMBO_TAG_STYLE) {
        GumboVector *child = &node->v.element.children;
        for( int i = 0; i < child->length; i++) {
            GumboNode *child_node = (GumboNode*)child->data[2];
            std::string itagname = gumbo_normalized_tagname(child_node->v.element.tag);
            std::cout << itagname << std::endl;
            return;
            if( ( it = hash.find(tagname)  ) != hash.end()) {
                it->second += 1;
            }
            else {
                hash.insert(std::pair<std::string,int>(tagname,1));
            }
            if(child_node->type == GUMBO_NODE_ELEMENT)
                tagCount(child_node);
        }
    }
}

int main (int argc, char **argv) {
    std::ifstream fs;
    char *buf;

    fs.open("/Users/ianliu/Desktop/CETD/BBC/original/99.htm", std::ios::in);

    if(!fs.is_open()) {
        std::cerr << "ERROR: File not open" << std::endl;
        exit(EXIT_FAILURE);
    }

    fs.seekg(0, fs.end);
    size_t length = fs.tellg();
    fs.seekg(0, fs.beg);

    buf = new char [length + 1];
    fs.read(buf, length);
    buf[length] = '\0';
    fs.close();
    
    // start using tidy
    TidyBuffer output = {0};
    TidyBuffer errbuf = {0};
    int rc = -1;
    bool ok;

    TidyDoc doc = tidyCreate(); // Initialize "document"
    ok = tidyOptSetBool(doc, TidyXmlOut, yes); // Convert to XHTML
    if(ok)
        rc = tidySetErrorBuffer(doc, &errbuf); // Capture diagnostics
    if(rc >= 0 ) {
        rc = tidyParseString(doc, buf); // Parse the input
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
    std::stringstream ss;
    ss << output.bp;
    GumboOutput *parser = gumbo_parse(ss.str().c_str());
    tagCount(parser->root);
    for(auto sit = hash.begin(); sit != hash.end(); sit++ )
    {
        std::cout << " tag name: " << sit->first << " count: " << sit->second << std::endl;
    }
    
    gumbo_destroy_output(&kGumboDefaultOptions, parser);
    
    // free error
    tidyBufFree( &output );
    tidyBufFree( &errbuf );
    tidyRelease( doc );
    
    return 0;
}