#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "errors.h"
#include "logfiles.h"
#include "tree.h"

Error_t MyTreeCtor(Tree* tree,
                   const char* name,
                   const unsigned line,
                   const char* file,
                   const char* func)
    {
    assert(tree != NULL);

    tree->root = nullptr;
    tree->size = 0;

    tree->name = name;
    tree->line = line;
    tree->file = file;
    tree->func = func;

    if (LogFileInit(&tree->txt_logfile, "txt_logfile", name, "html") == FileError)
        {
        perror("ERROR: cannot open txt_logfile");
        return FileError;
        }

    tree->dumps_count = 0;

    return Ok;
    }

Error_t TreeDtor(Tree* tree)
    {
    State_t state = TreeVerify(tree);
    if (IsTreeErrorState(state))
        {
        TreeDump(tree, state);
        }

    if (tree->root) DeleteNode(tree->root);
    tree->size = 0;

    fclose(tree->txt_logfile);

    return Ok;
    }

Node* NewNode(Node** node, Data_t data)
    {
    if (*node)
        {
        printf("Error: node already exist");
        return nullptr;
        }

    *node         = (Node*)  calloc(1, sizeof(Node));
    (*node)->data = (Data_t) calloc(strlen(data) + 1, sizeof(*data));
    strncpy((*node)->data, data, strlen(data));

    (*node)->left   = nullptr;
    (*node)->right  = nullptr;

    return *node;
    }

Error_t DeleteNode(Node* node)
    {
    assert(node != NULL);

    if (node->left)  DeleteNode(node->left);
    if (node->right) DeleteNode(node->right);
    free(node->data);
    free(node);

    return Ok;
    }

Node* EditNode(Node* node, Data_t data)
    {
    if (!node)
        {
        printf("Error: node not exist");
        return nullptr;
        }

    free(node->data);

    node->data = (Data_t) calloc(strlen(data) + 1, sizeof(*data));
    strncpy(node->data, data, strlen(data));

    return node;
    }

Error_t PreorderNode(const Node* node, FILE* file)
    {
    if (!node)
        {
        fprintf(file, "nil\n");
        return Ok;
        }

    fprintf(file, "(\n");
    fprintf(file, "%s\n", node->data);
    PreorderNode(node->left, file);
    PreorderNode(node->right, file);
    fprintf(file, ")\n");

    return Ok;
    }

Error_t PostorderNode(const Node* node, FILE* file)
    {
    if (!node)
        {
        fprintf(file, "nil\n");
        return Ok;
        }

    fprintf(file, "(\n");
    PostorderNode(node->left, file);
    PostorderNode(node->right, file);
    fprintf(file, "%s\n", node->data);
    fprintf(file, ")\n");

    return Ok;
    }

int NodesCount(const Node* node)
    {
    if (node == nullptr)
        {
        return 0;
        }

    return NodesCount(node->left) + NodesCount(node->right);
    }

Error_t InorderNode(const Node* node, FILE* file)
    {
    if (!node)
        {
        fprintf(file, "nil\n");
        return Ok;
        }

    fprintf(file, "(\n");
    InorderNode(node->left, file);
    fprintf(file, "%s\n", node->data);
    InorderNode(node->right, file);
    fprintf(file, ")\n");

    return Ok;
    }

State_t TreeVerify(const Tree *tree)
    {
    State_t state = 0;

    if (tree == NULL)
        {
        state |= TREE_NULLPTR;
        return state;
        }

    return state;
    }

bool IsTreeErrorState(const State_t state)
    {
    return state;
    }

Error_t MyTreeDump(Tree *tree,
                   State_t state,
                   const char* name,
                   const unsigned line,
                   const char* file,
                   const char* func)
    {
    assert(tree != NULL);
    assert(tree->txt_logfile != NULL);

    TreeDumpMessage(tree, state, name, line, file, func);

    FILE* img_file_ptr  = nullptr;
    char  img_file_name[LOG_FILE_NAME_LENGTH] = "";
    char  obj_name[LOG_FILE_NAME_LENGTH] = "";
    snprintf(obj_name, sizeof(obj_name), "%s%d", name, tree->dumps_count);

    if (LogFileInit(&img_file_ptr, "img_logfile", obj_name, "dot", img_file_name) == FileError)
        {
        printf("ERROR: cannot open img_logfile");
        return FileError;
        }

    TreeDumpImage(tree, img_file_ptr, name, line, file, func);

    fclose(img_file_ptr);

    char command[COMMAND_LENGTH] = "";
    snprintf(command, sizeof(command), "dot -v -Tpng %s -o logfiles/img_log%d.png", img_file_name, tree->dumps_count);
    system(command);

    ++tree->dumps_count;

    return Ok;
    }


Error_t TreeDumpMessage(const Tree *tree,
                        State_t state,
                        const char* name,
                        const unsigned line,
                        const char* file,
                        const char* func)
    {
    assert(tree != NULL);
    assert(tree->txt_logfile != NULL);

    fprintf(tree->txt_logfile, "<pre>\n\n");

    fprintf(tree->txt_logfile, "Tree[%p] '%s' from %s(%u) %s()\n", tree, tree->name, tree->file, tree->line, tree->func);
    fprintf(tree->txt_logfile, "\tcalled like '%s' from %s(%u) %s()\n\n",      name,       file,       line,       func);
    fprintf(tree->txt_logfile, "\ttree:");

    if (tree->root)
        {
        PreorderNode(tree->root, tree->txt_logfile);
        }
    else
        {
        fprintf(tree->txt_logfile, "\tnil\n\t");
        }

    fprintf(tree->txt_logfile, "\tsize = %ld\n\t}\n\n", tree->size);

    for(size_t bit = 0; bit < CHAR_BIT * sizeof(state); bit++)
        {
        if (state & 1 << bit)
            {
            fprintf(tree->txt_logfile, "%s\n", GetTreeErrorBitMsg(bit));
            }
        }

    fprintf(tree->txt_logfile, "</pre>\n\n");

    return Ok;
    }

const char* GetTreeErrorBitMsg(size_t bit)
    {
    static const int  ERROR_COUNT = sizeof(State_t) * CHAR_BIT;
    static const char * const ERROR_MESSAGE[ERROR_COUNT] = {
        "Tree is nullptr",
        "Tree fas wrong size",};

    return ERROR_MESSAGE[bit];
    }

Error_t TreeDumpImage(const Tree *tree,
                        FILE* fp,
                        const char* name,
                        const unsigned line,
                        const char* file,
                        const char* func)
    {
    assert(tree != NULL);
    assert(fp != NULL);

    fprintf(fp,                 "digraph MyTree\n{\n"
                                "\tbgcolor = darkgreen;\n"
                                "\tgraph [splines = splines];\n"
                                "\tnode  [width = 2, style = filled, color = wheat];\n"
                                "\tedge  [color = darkgreen, fontsize = 15];\n\n");

    //style

    fprintf(fp,                 "\tsubgraph cluster%d\n    {\n"
                                "\t\tbgcolor = yellowgreen;"
                                "\t\theight  = 20"
                                "\t\tstyle   = filled;\n"
                                "\t\tlabel   = \"Tree[%p] called like '%s' from %s(%u) %s()\";\n\n",
                                tree->dumps_count, tree, name, file, line, func);

    if (tree->root)
        {
        NodeDump(tree->root, fp);
        }
    else
        {
        fprintf(fp,  "\t\tnil [shape=oval, height = 1, label = \"nil\"];\n");
        }

    fprintf(fp, "\t}\n}\n");

    return Ok;
    }

Error_t NodeDump(Node *node, FILE *fp)
    {
    assert(node != NULL);
    assert(fp   != NULL);

    fprintf(fp,  "\t\t%ld [shape=oval, height = 1, label = \"%s\"];\n", node, node->data);

    if (node->left)
        {
        fprintf(fp, "\t\t%ld -> %ld [color = green];\n", node, node->left);
        NodeDump(node->left, fp);
        }

    if (node->right)
        {
        fprintf(fp, "\t\t%ld -> %ld [color = red];\n", node, node->right);
        NodeDump(node->right, fp);
        }

    return Ok;
    }
