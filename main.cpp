

#include "Parser.h"
#include "gumbo-parser/src/gumbo.h"

int main() {
    GumboOutput* output = gumbo_parse("<h1>Hello, World!</h1>");
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}