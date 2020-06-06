#include "tree.h"

tree_node::tree_node() {
    char_count = 0;
    tag_count = 0;
    text_density = 0.0;
    child_size = 0;
    max_child_size = INITSIZE;
    children = new tree_node*[INITSIZE];
}

tree_node::tree_node(const tree_node &copy) {
    this->tagname = copy.tagname;
    this->char_count = copy.char_count;
    this->tag_count = copy.tag_count;
    this->text_density = copy.text_density;
    this->children = copy.children;
    std::cerr << "tree_node copy constructor is called" << std::endl;
}

tree_node& tree_node::operator=(const tree_node &rhs) {
    if(this == &rhs) return *this;
    this->tagname = rhs.tagname;
    this->char_count = rhs.char_count;
    this->tag_count = rhs.tag_count;
    this->text_density = rhs.text_density;
    this->children = rhs.children;
    std::cerr << "Tree Node Assignment Operator is called !!!!" << std::endl;
    return *this;
}

void tree_node::push_back(tree_node *node) {
    if(child_size + 1 > max_child_size) {
       max_child_size = child_size * 2;
       tree_node **tmp_tree = children;
       children = new tree_node*[max_child_size];
       for(int i = 0; i < child_size; i++ ) {
           children[i] = tmp_tree[i];
       }
       delete tmp_tree;
       std::cerr << "node " << tagname << " has been reallocate and old memory are deleted" 
       << std::endl;
    } // reallocate
    children[child_size] = node;
    child_size += 1;
    return;
}

DOM_tree::DOM_tree(tree_node *rroot) {
    this->root = rroot;
    content_buffer = "";
}

void DOM_tree::contentExtraction() {
    get_threshold(root);
    extract_maintex(root);
}

// Put the threshold value of body into threhold
// By doing this, I could save the memory space for tmp_threshold
// which is use as intermediate variable for passing value back from recursive
void DOM_tree::get_threshold(tree_node *node) {
    tree_node *tmp_node;
    if(tmp_node->tagname == "body") {
        threshold = node->text_density; 
        return;
    }
    for(int i = 0; i < node->child_size; i++) {
        tmp_node = node->children[i];
        get_threshold(tmp_node);
    }
    return;
}

void DOM_tree::extract_maintex(tree_node *node) {
    tree_node *tmp_node;
    std::string tagname = node->tagname;

    if(node->text_density < threshold) return;

    else {
        if(
            tagname == "p" || tagname == "strong" || tagname == "i" || tagname == "b" ||
            tagname == "em" || tagname == "mark" || tagname == "small" || tagname == "del" ||
            tagname ==  "ins" || tagname == "sub" || tagname == "sup" || tagname == "h1" || 
            tagname == "h2" || tagname == "h3" || tagname == "h4" || tagname == "h5" ||
            tagname == "h6") {
                content_buffer += node->value;
        }
        for(int i = 0; i < node->child_size; i++) {
            tmp_node = node->children[i];
            extract_maintex(tmp_node);
        }
    }
}


void DOM_tree::traverse(tree_node *node) {
    tree_node* tmp_child;
    std::cout << "Tag name: " << node->tagname << 
        " Text Density: " << node->text_density << 
        " Text Count: " << node->char_count << " Tag Count: " 
        << node->tag_count << std::endl;
    for(int i = 0; i < node->child_size; i++) {
        tmp_child = node->children[i];
        traverse(tmp_child);
    }
}

tree_node* create_dom_tree(pt::ptree node, std::string tagname) {
    tree_node *parent = new tree_node;
    tree_node *child;
    int tmp_char_count = 0;

    parent->tagname = tagname;

    if(
        tagname == "p" || tagname == "strong" || tagname == "i" || tagname == "b" ||
        tagname == "em" || tagname == "mark" || tagname == "small" || tagname == "del" ||
        tagname ==  "ins" || tagname == "sub" || tagname == "sup" || tagname == "h1" || 
        tagname == "h2" || tagname == "h3" || tagname == "h4" || tagname == "h5" ||
        tagname == "h6") {
            parent->value = node.data();
            tmp_char_count = node.data().length();
        }

    for(auto it = node.begin(); it != node.end(); it++) {
        if(
            it->first != "script" && it->first != "noscript" && 
            it->first != "style" && it->first != "<xmlcomment>"&&
            it->first != "<xmlattr>"  && it->first != "<xml:lang>" ) 
        {
            child = create_dom_tree(it->second, it->first); // second argument is to identify maintex's tag   
            parent->char_count += child->char_count + tmp_char_count;
            parent->tag_count += child->tag_count + 1; // include itself
            parent->push_back(child);
        }
        else continue;
    }

    // in general tag_count == 0 means hitting the bottom of tree; hence,
    // we add the char vlaue  and set tag_count = 1
    // if not doing this the leaf's (bottom of tree) value is unable to return back to parent
    // make the every node in tree have no character count
    if(parent->tag_count == 0) {
        parent->char_count += tmp_char_count;
        parent->tag_count = 1;
    }

    parent->text_density = (parent->char_count) / (parent->tag_count);
    
    return parent;
}