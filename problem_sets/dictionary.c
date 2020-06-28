// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASHTABLE_SIZE 1000

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[HASHTABLE_SIZE];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char check_word[strlen(word)];
    strcpy(check_word, word);
    for (int i = 0; check_word[i] != '\0'; i++)
    {
        check_word[i] = tolower(check_word[i]);
    }
    int index= hash(check_word);
    if(table[index] != NULL)
    {
        for(node* nodeptr = table[index]; nodeptr!=NULL; nodeptr = nodeptr->next)
        {
            if(strcmp(nodeptr->word, check_word) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

unsigned int num_words = 0;
bool dict_is_loaded = false;
// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if(isalpha(word[i]))
        {
            n = word[i] - 'a' + 1;
        }
        else
        {
            n = 27;
        }
        hash = ((hash << 3) + n) % HASHTABLE_SIZE;
    }
    return 0;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE* infile = fopen(dictionary, "r");
    if(infile == NULL)
    {
        return false;
    }
    for(int i = 0; i < HASHTABLE_SIZE; i++)
    {
        table[i] = NULL;
    }
    char word[LENGTH + 1];
    node* new_nodeptr;
    while(fscanf(infile, "%s", word) != EOF)
    {
        num_words++;
        do
        {
        new_nodeptr = malloc(sizeof(node));
            if (new_nodeptr == NULL)
            {
                free(new_nodeptr);
            }
        } while (new_nodeptr == NULL);

        strcpy(new_nodeptr->word, word);
        int index = hash(word);

        new_nodeptr->next = table[index];
        table[index] = new_nodeptr;
    }
    fclose(infile);
    dict_is_loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (!dict_is_loaded)
    {
        return 0;
    }
    return num_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    if (!dict_is_loaded)
    {
        return false;
    }
    for(int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if(table[i] != NULL)
        {
            node* ptr = table[i];
            while(ptr != NULL)
            {
                node* predptr = ptr;
                ptr = ptr->next;
                free(predptr);
            }
        }
    }
    return true;
}
