// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

typedef struct treenode
{
    char* string;
    struct treenode *right;
    struct treenode *left;
    int height;

} treenode;

// Represents a node in a hash table
typedef struct node
{
    char word[2];
    treenode* next;
} node;

// Prototypes
bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);
treenode* add_node(treenode* list, treenode* n);
int check_balance(treenode* list);
int max(int x, int y);
bool binary_search(treenode* ptr, const char* word);
treenode* checkHeight(treenode* ptr, treenode* n);
treenode* rightRotate(treenode* node);
treenode* leftRotate(treenode* node);


#endif // DICTIONARY_H
