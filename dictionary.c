// Implements a dictionary's functionality

// Hash Function can be changed by adding double the amount of size for
// The hashing table, remember when David said you can double it by hashing
// To aa, ab, ac, ad, etc instead of having a table of size 26 with a, b, c
// I think I want to make a table of trees.

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// AVL IMPLEMENTATION!!!
// Figure out how to calculate the heights of the left and right subtrees of
// Each node.

// So far I am creating three height variables. height left
// height right and regular height.
// Everytime the add node recurses left it adds to heightLeft.
// Everytime it adds to the right it adds to heightRight

// I was thinking. Since we do not know wether or not
// It will add to the height until the last node it put
// into the tree. I wanted to create a global variable
// To keep track of height then update from then.
// Maybe traverse down the tree and add or subtract
// from the other nodes as needed.

// 8/18/2024
// Look at the roatation on the board how do you do that rotation?
// Ask chat gpt. Look at videos. ETC
// GLOBAL VARIABLE TO KEEP TRACK OF DICTIONARY SIZE
uint32_t sizer = 0;

// TODO: Choose number of buckets in hash table
const unsigned int N = 702;

// Hash table
node *table[N] = {NULL};

int strgcmp(const char *str1, const char *str2)
{
    // GO THROUGH NODE TREE AND DO A BINARY SEARCH FOR EQUIVALENT WORD ELSE ITS SPELLED INCORRECTLY
    while (*str1 && (toupper(*str1) == toupper(*str2)))
    {
        str1++;
        str2++;
    }
    return *(unsigned char *) str1 - *(unsigned char *) str2;
}

int height(treenode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return node->height;
}

int balance(treenode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return height(node->right) - height(node->left);
}

int max(int x, int y)
{
    if (x > y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

treenode *rightRotate(treenode *node)
{
    // printf("ROTATE RIGHT\n");
    treenode *node2 = node->left;
    treenode *node3 = node2->right;

    node2->right = node;
    node->left = node3;

    node->height = max(height(node->left), height(node->right)) + 1;
    node2->height = max(height(node2->left), height(node2->right)) + 1;

    return node2;
}

treenode *leftRotate(treenode *node)
{
    // printf("ROTATE LEFT\n");
    treenode *node2 = node->right;
    treenode *node3 = node2->left;

    node2->left = node;
    node->right = node3;

    node->height = max(height(node->left), height(node->right)) + 1;
    node2->height = max(height(node2->left), height(node2->right)) + 1;

    return node2;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    if (strlen(word) < 2 && toupper(word[0]) == 'A')
        return true;

    int n = hash(word);

    if (table[n] == NULL)
    {
        // printf("Visited ! \n%s\n", word);
        // getchar();
        return false;
    }

    // Search for word
    return binary_search(table[n]->next, word);

    return false;
}

bool binary_search(treenode *ptr, const char *word)
{
    //  IF A POINTER IN THE TABLE ARRAY IS NULL. IT MAY INDICATE A LOAD FAILURE
    if (ptr == NULL)
    {
        // printf("Something is very wrong!");
        return false;
    }

    int strval = strcasecmp(word, ptr->string);

    // GO THROUGH NODE TREE AND DO A BINARY SEARCH FOR EQUIVALENT WORD ELSE ITS SPELLED INCORRECTLY
    if (strval > 0)
    {
        if (ptr->right == NULL)
        {
            // printf("reached right BINARY SEARCH\n");
            return false;
        }
        else
        {
            return binary_search(ptr->right, word);
        }
    }
    else if (strval < 0)
    {
        if (ptr->left == NULL)
        {
            // printf("reached left BINARY SEARCH\n");
            return false;
        }
        else
        {
            return binary_search(ptr->left, word);
        }
    }
    else
    {
        return true;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    if (strlen(word) < 2)
    {
        return (toupper(word[0]) - 'A');
    }

    return (((toupper(word[0]) - 'A') * 26) + 1) + toupper(word[1]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    FILE *ptr = fopen(dictionary, "r");

    // IF FILE CANNOT BE FOUND RETURN CAN'T OPEN FILE
    if (ptr == NULL)
    {
        // printf("Could not open file.\n");
        return false;
    }

    char c[LENGTH + 2];

    treenode *list = NULL;

    // GET ONE WORD IN TXT FILE ONE LINE AT A TIME
    while (fgets(c, sizeof(c), ptr) != 0)
    {

        // RECORD EVEN ONE LETTER WORDS FOR CS50 WILL NEED TO GET RETURNED
        // THE FULL SIZE OF THE DICTIONARY


        if (isalpha(c[0]))
        {
            sizer++;
        }


        treenode *n = (treenode *) malloc(sizeof(treenode));

        // IF SYSTEM CAN'T ALLOCATE SPACE RETURN FAIL
        if (n == NULL)
        {
            fclose(ptr);
            free(n);
            return false;
        }

        n->string = (char *) malloc(sizeof(char) * (strlen(c)));

        // IF SYSTEM CAN'T ALLOCATE SPACE RETURN FAIL
        if (n->string == NULL)
        {
            free(n);
            fclose(ptr);
            return false;
        }

        // COPY THE STRING INCLUDING THE APOSTROPHE
        strncpy(n->string, c, strlen(c) - 1);

        n->string[strlen(c) - 1] = '\0';
        n->height = 1;
        n->right = NULL;
        n->left = NULL;

        // IF TABLE POSITION OF FIRST TWO LETTERS DO NOT HAVE A TREE,
        // HAVE POINTER AT TABLE POSITION POINT TO TREE ROOT
        unsigned int index = hash(n->string);

        if (table[index] == NULL)
        {
            // printf("adding table node\n");
            table[index] = (node *) malloc(sizeof(node));

            if (table[index] == NULL)
            {
                free(table[index]);
                fclose(ptr);
                return false;
            }

            table[index]->next = n;

            if (strlen(n->string) == 1)
            {
                table[index]->word[0] = n->string[0];
            }
            else
            {
                table[index]->word[0] = n->string[0];
                table[index]->word[1] = n->string[1];
            }
        }
        else // ADD A NODE TO THE AVL TREE
        {
            // printf("%s\n", n->string);
            table[index]->next = add_node(table[index]->next, n);
        }
    }

    fclose(ptr);
    return true;
}

// -------------------------------------------------------------------------------------------------

treenode *checkHeight(treenode *node, treenode *n)
{
    // RIGHT ROTATE / LEFT SIDE UNBALANCED
    if (node->left != NULL && height(node->right) - height(node->left) < -1 &&
        strcmp(n->string, node->left->string) < 0)
    {
        return rightRotate(node);
    }

    // LEFT ROTATE / RIGHT SIDE UNBALANCED
    if (node->right != NULL && height(node->right) - height(node->left) > 1 &&
        strcmp(n->string, node->right->string) > 0)
    {
        return leftRotate(node);
    }

    // RIGHT-LEFT ROTATE / LEFT SIDE UNBALANCED
    if (node->left != NULL && height(node->right) - height(node->left) < -1 &&
        strcmp(n->string, node->left->string) > 0)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // LEFT-RIGHT ROTATE / RIGHT SIDE UNBALANCED
    if (node->right != NULL && height(node->right) - height(node->left) > 1 &&
        strcmp(n->string, node->right->string) < 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Strcmp uses the same system of comparision you do. If values dont match and the corresponding
// index is lower in the first word than the second word then it returns a negative number if its
// higher it returns a positive number. If the first word is smaller than the second it returns a
// negative value. If the first word is larger then is returns a positive value. If even it returns
// a 0. So you can just do strcmp and then deviate between right and left.
// -------------------------------------------------------------------------------------------------
treenode *add_node(treenode *list, treenode *n)
{
    if (strcmp(n->string, list->string) == 0)
    {
        return list;
    }

    // printf("inside tree\n");

    // printf("n \'string\' %s, \'list string\' %s\n", n->string, list->string);
    int stringC = strcmp(n->string, list->string);

    if (stringC >
        0) // IF N POINTER STRING VALUE AT INDEX J IS GREATER THAN // LISTS VALUE AT THE SAME INDEX
    {
        if (list->right == NULL)
        {
            list->right = n;
            list->height = max(height(list->right), height(list->left)) + 1;
            // printf("this would be correct RIGHT\n");
            return list;
        }
        else
        {
            // printf("going right\n");

            treenode *returnedValue = add_node(list->right, n);

            // printf("going back\n");

            list->height = max(height(list->right), height(list->left)) + 1;

            if (strcmp(returnedValue->string, list->right->string) != 0)
            {
                list->right = returnedValue;
                list->height = max(height(list->right), height(list->left)) + 1;
                return list;
            }

            return checkHeight(list, n);
        }
    }
    else if (stringC < 0) // IF N POINTER STRING VALUE AT INDEX J IS GREATER THAN LIST
    {
        if (list->left == NULL)
        {
            list->left = n;
            list->height = max(height(list->right), height(list->left)) + 1;
            // printf("this would be correct LEFT\n");
            return list;
        }
        else
        {
            // printf("going left\n");
            treenode *returnedValue = add_node(list->left, n);

            list->height = max(height(list->right), height(list->left)) + 1;

            if (strcmp(returnedValue->string, list->left->string) != 0)
            {
                list->left = returnedValue;
                list->height = max(height(list->right), height(list->left)) + 1;
                return list;
            }

            return checkHeight(list, n);
        }
    }
    else
    {
        return list;
    }

    return list;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (sizer > 0)
    {
        return sizer;
    }
    else
    {
        return 0;
    }
}

bool unloader(treenode *ptr)
{

    if (ptr == NULL)
    {
        return true;
    }

    unloader(ptr->right);
    unloader(ptr->left);

    free(ptr->string);
    free(ptr);
    return true;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            unloader(table[i]->next);
            free(table[i]);
        }
    }

    return true;
}
