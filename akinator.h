#ifndef AKINATOR_H
#define AKINATOR_H

const int WORD_LENGTH = 20;
const int LINE_LENGTH = 60;

Error_t StartAkinatorGame(const char* file_name);

Error_t Repeat(Tree* tree);

Error_t Ask(Tree* tree);
Error_t Learn(Tree* tree);
Error_t Compare(Tree* tree);

bool Search(Node** node, const char* obj, Stack* stack);

Error_t  ReadTree(Tree* tree, const char* file_name);
Error_t WriteTree(Tree* tree, const char* file_name);

#endif //AKINATOR_H
