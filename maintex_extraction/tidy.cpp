#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <map>

namespace pt = boost::property_tree;
std::map<std::string, int> stack;

void cleanNode(pt::ptree &root) {
    
}


void Tagcount(pt::ptree &root) {

    for(auto it = root.begin(); it != root.end(); it++) {
        if(stack.find(it->first) != stack.end()) {
            stack.find(it->first)->second += 1;
        }
        else {
            stack.insert(std::pair<std::string, int>(it->first, 1));
        }
        Tagcount(it->second);
    }
}

int main (int argc, char **argv) {
    std::ifstream fs;
    std::ofstream out;
    char *buf;

    fs.open("/Users/ianliu/Desktop/crawlerNew/maintex_extraction/CETD/BBC/original/99.htm", std::ios::in);
    out.open("/Users/ianliu/Desktop/crawlerNew/maintex_extraction/output/0.xhtml", std::ios::out);

    if(!fs.is_open()) {
        std::cerr << "ERROR: File not open" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(!out.is_open()) {
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
    
    /*
    if( rc >= 0 ) {
        if(rc > 0 ) std::cerr << "ERROR: " << errbuf.bp << std::endl;
        out << output.bp;
        out.close();
    }
    */

   
    // parse DOM and compute textdensity (Ci / Ti)
    boost::property_tree::ptree DOM_tree;
    std::stringstream ss;
    ss << output.bp;

    try {
        boost::property_tree::xml_parser::read_xml(ss, DOM_tree);
    }

    catch(const boost::property_tree::xml_parser::xml_parser_error& ex) {
        std::cerr << ex.message() << std::endl;
    }

    std::map<std::string, int>::iterator it;
    


    //clean script, comment, style first
    for(auto it = DOM_tree.begin(); it != DOM_tree.end(); it++)
    {
        Tagcount(it->second);
    }


    for(it = stack.begin(); it != stack.end(); it++ )
    {
        std::cout << " tag name: " << it->first << " count: " << it->second << std::endl;
    }

    // free error
    tidyBufFree( &output );
    tidyBufFree( &errbuf );
    tidyRelease( doc );

    return 0;
}