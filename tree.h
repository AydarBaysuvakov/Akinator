#ifndef TREE_H
#define TREE_H

#define TREE_DEFN_ARGS const char* /* name */, const unsigned /* line */, \
                       const char* /* file */, const char*    /* func */
#define TREE_PASS_ARGS __LINE__, __FILE__, __FUNCTION__
#define TreeCtor(stk)         MyTreeCtor((stk),           #stk, TREE_PASS_ARGS)
#define TreeDump(stk, stk_st) MyTreeDump((stk), (stk_st), #stk, TREE_PASS_ARGS)

typedef char* Data_t;

struct Node
    {
    Data_t data;
    Node* left;
    Node* right;
    };

struct Tree
    {
    Node* root;
    size_t size;

    const char* name;
    unsigned    line;
    const char* file;
    const char* func;

    FILE* txt_logfile;
    int   dumps_count;
    };

enum ListErrorBit
    {
    TREE_NULLPTR               = 1 << 0,
    WRONG_SIZE                 = 1 << 1
    };

Error_t MyTreeCtor(Tree* tree, TREE_DEFN_ARGS);

Error_t TreeDtor(Tree* tree);

Node* NewNode(Node** node, Data_t data);

Error_t DeleteNode(Node* node);

Node* EditNode(Node* node, Data_t data);

Error_t PreorderNode(const Node* node, FILE* file);

Error_t PostorderNode(const Node* node, FILE* file);

Error_t InorderNode(const Node* node, FILE* file);

State_t TreeVerify(const Tree *tree);

bool IsTreeErrorState(const State_t state);

Error_t MyTreeDump(Tree *tree, State_t state, TREE_DEFN_ARGS);

Error_t TreeDumpMessage(const Tree *tree, State_t state, TREE_DEFN_ARGS);

const char* GetTreeErrorBitMsg(size_t bit);

Error_t TreeDumpImage(const Tree *tree, FILE* fp, TREE_DEFN_ARGS);

Error_t NodeDump(Node *node, FILE *fp);

#endif //TREE_H
