#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "errors.h"
#include "tree.h"
#include "stack.h"
#include "akinator.h"

Error_t StartAkinatorGame(const char* file_name)
    {
    assert(file_name != NULL);

    Tree tree = {};
    TreeCtor(&tree);

    if (ReadTree(&tree, file_name) == SyntaxError)
        {
        return SyntaxError;
        }

    size_t answer_length = LINE_LENGTH;
    char* answer = (char*) calloc(answer_length, sizeof(char));

    printf("Приветствую вас путник! Не хотите ли сыграть со мной игру?\n"
           "Вы можете ответить ДА или НЕТ (Д/Н): ");
    while (getline(&answer, &answer_length, stdin))
        {
        if (!strncmp(answer, "ДА\n", sizeof("ДА\n")) || !strncmp(answer, "Д\n", sizeof("Д\n")) || !strncmp(answer, "д\n", sizeof("д\n")))
            {
            break;
            }
        else if (!strncmp(answer, "НЕТ\n", sizeof("НЕТ\n")) || !strncmp(answer, "Н\n", sizeof("Н\n")) || !strncmp(answer, "н\n", sizeof("н\n")))
            {
            printf("Хорошего дня, путник!\n");
            TreeDtor(&tree);
            free(answer);
            return Ok;
            }
        else
            {
            printf("Я не смог разобрать ваших слов, повторите пожалуйста, ДА или НЕТ (Д/Н): ");
            }
        }

    printf("Отлично, я имею несколько навыков, чего изволите?\nЗагадать персонажа [у], дать определение [о], сравнить [с]: ");
    while (getline(&answer, &answer_length, stdin))
        {
        if (!strncmp(answer, "У\n", sizeof("У\n")) || !strncmp(answer, "у\n", sizeof("у\n")))
            {
            Ask(&tree);
            break;
            }
        else if (!strncmp(answer, "О\n", sizeof("О\n")) || !strncmp(answer, "о\n", sizeof("о\n")))
            {
            Learn(&tree);
            break;
            }
        else if (!strncmp(answer, "С\n", sizeof("О\n")) || !strncmp(answer, "с\n", sizeof("о\n")))
            {
            Compare(&tree);
            break;
            }
        else
            {
            printf("\nЯ не смог разобрать ваших слов, повторите пожалуйста, чего изволите?\n"
                     "Загадать персонажа [у], дать определение [о], сравнить [с]: ");
            }
        }

    Repeat(&tree);

    printf("Рад был с вами поиграть! Хорошего дня, путник!\n");

    free(answer);

    WriteTree(&tree, file_name);
    TreeDump(&tree, 0);

    TreeDtor(&tree);
    return Ok;
    }

Error_t Repeat(Tree* tree)
    {
    assert(tree != NULL);

    printf("\nНе хотите ли сыграть ещё?\n");

    size_t answer_length = LINE_LENGTH;
    char* answer = (char*) calloc(answer_length, sizeof(char));

    while (getline(&answer, &answer_length, stdin))
        {
        if (!strncmp(answer, "ДА\n", sizeof("ДА\n")) || !strncmp(answer, "Д\n", sizeof("Д\n")) || !strncmp(answer, "д\n", sizeof("д\n")))
            {
            break;
            }
        else if (!strncmp(answer, "НЕТ\n", sizeof("НЕТ\n")) || !strncmp(answer, "Н\n", sizeof("Н\n")) || !strncmp(answer, "н\n", sizeof("н\n")))
            {
            free(answer);
            return Ok;
            }
        else
            {
            printf("Я не смог разобрать ваших слов, повторите пожалуйста, ДА или НЕТ (Д/Н): ");
            }
        }

    printf("Чего изволите сейчас?\nЗагадать персонажа [у], дать определение [о], сравнить [с]: ");

    while (getline(&answer, &answer_length, stdin))
        {
        if (!strncmp(answer, "У\n", sizeof("У\n")) || !strncmp(answer, "у\n", sizeof("у\n")))
            {
            Ask(tree);
            break;
            }
        else if (!strncmp(answer, "О\n", sizeof("О\n")) || !strncmp(answer, "о\n", sizeof("о\n")))
            {
            Learn(tree);
            break;
            }
        else if (!strncmp(answer, "С\n", sizeof("О\n")) || !strncmp(answer, "с\n", sizeof("о\n")))
            {
            Compare(tree);
            break;
            }
        else
            {
            printf("\nЯ не смог разобрать ваших слов, повторите пожалуйста, чего изволите?\n"
                     "Загадать персонажа [у], дать определение [о], сравнить [с]: ");
            }
        }

    free(answer);

    Repeat(tree);

    return Ok;
    }

Error_t Ask(Tree* tree)
    {
    assert(tree != NULL);

    printf("\nЗамечательно, вы должны загадать персонажа, а я попытаюсь его угадать.\n"
             "Честно отвечайте на мои вопросы: ДА или НЕТ (Д/Н)\n");

    Node* curent = tree->root;

    size_t answer_length = LINE_LENGTH;
    char* answer = (char*) calloc(answer_length, sizeof(char));

    while (curent->left && curent->right)
        {
        printf("\n%s?\n", curent->data);
        while (getline(&answer, &answer_length, stdin))
            {
            if (!strncmp(answer, "ДА\n", sizeof("ДА\n")) || !strncmp(answer, "Д\n", sizeof("Д\n")) || !strncmp(answer, "д\n", sizeof("д\n")))
                {
                curent = curent->left;
                break;
                }
            else if (!strncmp(answer, "НЕТ\n", sizeof("НЕТ\n")) || !strncmp(answer, "Н\n", sizeof("Н\n")) || !strncmp(answer, "н\n", sizeof("н\n")))
                {
                curent = curent->right;
                break;
                }
            else
                {
                printf("Я не смог разобрать ваших слов, повторите пожалуйста, ДА или НЕТ (Д/Н): ");
                }
            }
        }

    printf("Так есть у меня один на примете. Это %s?\n", curent->data);
    while (getline(&answer, &answer_length, stdin))
        {
        if (!strncmp(answer, "ДА\n", sizeof("ДА\n")) || !strncmp(answer, "Д\n", sizeof("Д\n")) || !strncmp(answer, "д\n", sizeof("д\n")))
            {
            printf("Это было очень легко, я знаю про всех и каждого!\n");
            break;
            }
        else if (!strncmp(answer, "НЕТ\n", sizeof("НЕТ\n")) || !strncmp(answer, "Н\n", sizeof("Н\n")) || !strncmp(answer, "н\n", sizeof("н\n")))
            {
            printf("Удивительно, кто же это может быть?\n"
                   "Сдаюсь, так кого вы загадали?\n");
            getline(&answer, &answer_length, stdin);
            NewNode(&curent->left, answer);
            printf("Хмм, а чем же %s отличается от %s\n", answer, curent->data);
            getline(&answer, &answer_length, stdin);
            printf("Хорошо, я запомнил, теперь я знаю на одного персонажа больше\n");
            NewNode(&curent->right, curent->data);
            EditNode(curent, answer);
            break;
            }
        else
            {
            printf("Я не смог разобрать ваших слов, повторите пожалуйста, ДА или НЕТ (Д/Н): ");
            }
        }

    free(answer);

    return Ok;
    }

Error_t Learn(Tree* tree)
    {
    assert(tree != NULL);

    printf("\nЗамечательно, скажите, про кого вы хотите узнать?\n");

    size_t answer_length = LINE_LENGTH;
    char* answer = (char*) calloc(answer_length, sizeof(char));
    getline(&answer, &answer_length, stdin);

    Stack stack = {};
    StackCtor(&stack);

    if (Search(&tree->root, answer, &stack))
        {
        printf("Итак, ваш персонаж обладает следующими признаками:\n");
        Node** curent = StackPop(&stack);
        while (StackTop(&stack) != POISON)
            {
            Node** next = StackPop(&stack);
            if ((*curent)->left == *next)
                {
                printf("Верно, что %s\n", (*curent)->data);
                }
            else
                {
                printf("Неверно, что %s\n", (*curent)->data);
                }
            curent = next;
            }
        }
    else
        {
        printf("К сожалению, я не знаю такого персонажа. Возможно, вы допустили ошибку в его имени");
        }

    StackDtor(&stack);
    free(answer);

    return Ok;
    }

Error_t Compare(Tree* tree)
    {
    assert(tree != NULL);

    printf("\nЗамечательно, скажите, кого вы хотите сравнить?\n");

    printf("Первый: ");

    size_t first_ans_len = LINE_LENGTH;
    char*  first = (char*) calloc(first_ans_len, sizeof(char));
    getline(&first, &first_ans_len, stdin);

    Stack first_stk = {};
    StackCtor(&first_stk);

    if (!Search(&tree->root, first, &first_stk))
        {
        printf("К сожалению, я не знаю кто такой %s", first);

        StackDtor(&first_stk);
        free(first);

        return Ok;
        }

    printf("Второй: ");

    size_t second_ans_len = LINE_LENGTH;
    char*  second = (char*) calloc(second_ans_len, sizeof(char));
    getline(&second, &second_ans_len, stdin);

    Stack second_stk = {};
    StackCtor(&second_stk);

    if (!Search(&tree->root, second, &second_stk))
        {
        printf("К сожалению, я не знаю кто такой %s", second);
        StackDtor(&first_stk);
        free(first);
        StackDtor(&second_stk);
        free(second);
        return Ok;
        }

    printf("Итак, схожие свойства персонажей:\n");
    Node** curent_f = StackPop(&first_stk);
    Node** curent_s = StackPop(&second_stk);
    Node** next_f = nullptr;
    Node** next_s = nullptr;

    while (StackTop(&first_stk) != POISON && StackTop(&second_stk) != POISON)
        {
        if (curent_f != curent_s)
            {
            break;
            }
        next_f = StackPop(&first_stk);
        next_s = StackPop(&second_stk);
        if ((*curent_f)->left == *next_f && (*curent_s)->left == *next_s)
            {
            printf("Верно, что %s\n", (*curent_f)->data);
            }
        else if ((*curent_f)->right == *next_f && (*curent_s)->right == *next_s)
            {
            printf("Неверно, что %s\n", (*curent_f)->data);
            }
        else
            {
            break;
            }
        curent_f = next_f;
        curent_s = next_s;
        }

    printf("\nПервый отличается от второго тем, что:\n");
    do  {
        if ((*curent_f)->left == *next_f)
            {
            printf("Верно, что %s\n", (*curent_f)->data);
            }
        else
            {
            printf("Неверно, что %s\n", (*curent_f)->data);
            }
        curent_f = next_f;
        next_f = StackPop(&first_stk);
        } while (next_f != POISON);

    printf("\nВторой отличается от первого тем, что:\n");
    do  {
        if ((*curent_s)->left == *next_s)
            {
            printf("Верно, что %s\n", (*curent_s)->data);
            }
        else
            {
            printf("Неверно, что %s\n", (*curent_s)->data);
            }
        curent_s = next_s;
        next_s = StackPop(&second_stk);
        } while (next_s != POISON);

    StackDtor(&first_stk);
    free(first);
    StackDtor(&second_stk);
    free(second);

    return Ok;
    }

bool Search(Node** node, const char* obj, Stack* stack)
    {
    assert(node  != NULL);
    assert(obj   != NULL);
    assert(stack != NULL);

    if (!strncmp((*node)->data, obj, strlen(obj) - 1))
        {
        StackPush(stack, node);
        return true;
        }

    if ((*node)->left)
        {
        if (Search(&(*node)->left, obj, stack))
            {
            StackPush(stack, node);
            return true;
            }
        }

    if ((*node)->right)
        {
        if (Search(&(*node)->right, obj, stack))
            {
            StackPush(stack, node);
            return true;
            }
        }

    return false;
    }

Error_t ReadTree(Tree* tree, const char* file_name)
    {
    assert(tree != NULL);
    assert(file_name != NULL);

    FILE* fp = fopen(file_name, "r");
    if (fp == NULL)
        {
        perror("ERROR: cannot open file");
        return FileError;
        }

    Stack stack = {};
    StackCtor(&stack);

    char line[LINE_LENGTH] = "";
    char word[WORD_LENGTH] = "";
    fscanf(fp, "%s", word);
    if (strncmp(word, "(", sizeof("(")))
        {
        printf("Syntax error\n");
        StackDtor(&stack);
        return SyntaxError;
        }
    Node* curent = NewNode(&tree->root, "");
    StackPush(&stack, &curent->right);
    StackPush(&stack, &curent->left);

    while (fscanf(fp, "%s", word) != EOF)
        {
        if (!strncmp(word, "nil", sizeof("nil")))
            {
            if (StackTop(&stack) == &curent->left)
                {
                EditNode(curent, line);
                *line = '\0';
                }
            StackPop(&stack);
            }
        else if (!strncmp(word, "(", sizeof("(")))
            {
            if (StackTop(&stack) == &curent->left)
                {
                EditNode(curent, line);
                *line = '\0';
                }

            Node* parent = curent;
            curent = NewNode(StackPop(&stack), "");
            StackPush(&stack, &parent);
            StackPush(&stack, &curent->right);
            StackPush(&stack, &curent->left);
            }
        else if (!strncmp(word, ")", sizeof(")")))
            {
            if (StackTop(&stack) == POISON)
                {
                break;
                }
            if (*StackTop(&stack) == curent->left || *StackTop(&stack) == curent->right)
                {
                printf("Syntax error\n");
                StackDtor(&stack);
                TreeDtor(tree);
                return SyntaxError;
                }
            curent = *StackPop(&stack);
            }
        else
            {
            strncat(line, word, LINE_LENGTH - strlen(line));
            strncat(line, " ", LINE_LENGTH - strlen(line));
            }
        }

    StackDtor(&stack);
    return Ok;
    }

Error_t WriteTree(Tree* tree, const char* file_name)
    {
    assert(tree != NULL);
    assert(file_name != NULL);

    FILE* fp = fopen(file_name, "w");
    if (fp == NULL)
        {
        perror("ERROR: cannot open file");
        return FileError;
        }

    PreorderNode(tree->root, fp);

    fclose(fp);

    return Ok;
    }
