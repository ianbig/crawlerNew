# ifndef TREE_H
# define TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

#define INITSIZE 10

namespace pt = boost::property_tree;

class DOM_tree;

class tree_node {
    private:
        size_t child_size;
        size_t max_child_size;
        tree_node **children;
    public:
        std::string tagname;
        double char_count;
        double tag_count;
        double text_density;
        std::string value;
        tree_node();
        tree_node(const tree_node &copy);
        tree_node& operator= (const tree_node &rhs);
        void push_back(tree_node *node);
    friend class DOM_tree;
    };

class DOM_tree {
    private:
        //double threshold;
    public:
        tree_node *root;
        // create tree and delete not necessary node e.g.
        // script
        // no script
        // <xmlcomment>
        // <xmlattr>
        // <xml:lang>
        DOM_tree(tree_node *rroot);
        //~DOM_tree();
        void contentExtraction();
        void traverse(tree_node *node);
        void get_threshold(tree_node *node); // have memory bug
        void extract_maintex(tree_node *node);
        double threshold;
        std::string content_buffer;
        
};

// dom tree is built recursievely
// ptree is recursively parse
// and the corresponding node is built according to current ptree node
// comment, syle, script is removed from dom_tree since most news article do not need javascript to show content
tree_node* create_dom_tree(pt::ptree node, std::string tagname);
//void delete_tree();


# endif