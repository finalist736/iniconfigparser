#include "iniconfig.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONFIG_FILE_LINE_MAXLENGTH 100

void addNode(struct ConfigNode **n, char * name, char *val)
{
    if (*n == NULL) // found empty node, fill it
    {
        // let rewrite pointer with NULL
        *n = (struct ConfigNode*)malloc(sizeof(struct ConfigNode));
        (*n)->name = name; // remember name
        (*n)->value = val; // remember value
        (*n)->next = NULL; // fill NULL for end of nodes list
    } else {
        // need to remake with adding to first position, not to end
        // for performance
        addNode(&((*n)->next), name, val); // recursively add to list end
    }
}

struct ConfigNode* ConfigLoad(const char *filename)
{
    FILE * f;
    errno_t err = fopen_s(&f, filename, "r"); // open file with safe fopen
    if(err != 0) // check for errors
    {
       return NULL; // file not opened, returning
    }
    char str[CONFIG_FILE_LINE_MAXLENGTH]; // buffer for read line from file
    char *tok = NULL; // temp pointer for strings searching
    char *name = NULL, *value = NULL; // temp to allocate memory for names and values
    struct ConfigNode *begin = NULL; // begin of config nodes
    while(!feof(f)) // while end of file not reached
    {
        fgets(str, 99, f); // read line from file
        char *tmp = str;
        tok = strchr(tmp, '='); // search name, that before '='
        if (tok == NULL)
        {
            continue; // '=' not found, go to next line
        }
        // found, allocate memory for it
        int len = tok - tmp; // calc string length
        name = (char*)malloc(len + 1); // allocate memory for parameter name
        memset(name, 0, len + 1); // fill it with zero
        strncpy(name, tmp, len); // copy name
        // search value, that after =
        tmp = tok + 1; // move to next symbol
        tok = strchr(tmp, '\n'); // search end of lne
        if (tok == NULL) // new line not found
        {
            tok = strchr(tmp, '\0'); // search for <end of line>, in case this line last
            if (tok == NULL) // not found, maybe empty parameter?
            {
                len = 0; // allocate 0 bytes for empty string
            } else {
                len = tok - tmp; // calc string length
            }
        }
        else {
            len = tok - tmp; // calc string length
        }
        value = (char*)malloc(len + 1); // allocate for value
        memset(value, 0, len + 1); // fill it with zero
        strncpy(value, tmp, len); // copy value

        addNode(&begin, name, value); // create new node.
    }
    fclose(f); // close file
    return begin; // return begin of list
}

void ConfigFree(struct ConfigNode *begin)
{
    if (begin == NULL) // don't work with NULL
    {
        return;
    }
    if (begin->next != NULL) // next chain? free it
    {
        ConfigFree(begin->next);
    }
    free(begin->name); // free name memory allocated on line 47
    free(begin->value); // free value memory allocated on line 66
    free(begin); // free struct
}

struct ConfigNode *ConfigGetNode(struct ConfigNode* begin, const char *name)
{
    if (begin == NULL) // param name not found in all nodes
    {
        return begin; // return NULL?
    }
    if (strcmp(begin->name, name) == 0) // searching string found! return this node
    {
        return begin;
    }
    return ConfigGetNode(begin->next, name); // search with next node
}

// get int value of node
int ConfigValueInt(struct ConfigNode *node)
{
    int v = 0;
    if (node != NULL)
    {
        v = atoi(node->value); // alpha to integer
    }
    return v;
}

// get char value of node
char ConfigValueChar(struct ConfigNode *node)
{
    char v = '\0';
    if (node != NULL)
    {
        if (strlen(node->value) > 0)
        {
            v = node->value[0]; // get first char of array
        }
    }
    return v;
}

// get float value of node
float ConfigValueFloat(struct ConfigNode *node)
{
    float v = 0;
    if (node != NULL)
    {
        v = atof(node->value); // alpha to float, btw return type is double!
    }
    return v;
}

// get char* value of node
char *ConfigValueString(struct ConfigNode *node)
{
    if (node != NULL)
    {
        return node->value;
    }
    return ""; // empty const char *
}
