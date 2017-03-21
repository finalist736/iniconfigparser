#define _CRT_SECURE_NO_WARNINGS // for win32 with fopen secure warning
#include "iniconfig.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONFIG_FILE_LINE_MAXLENGTH 1024

int trimWhiteSpaceLeftLength(char *tok)
{
    int trimTemp = 0;
    while (*((tok-1) - trimTemp) == ' ') {
        trimTemp++;
    }
    return trimTemp;
}
char *trimWhiteSpaceRight(char *tmp)
{
    while (*tmp == ' ') tmp++;
    return tmp;
}

struct ConfigNode* ConfigLoad(const char *filename)
{
    FILE * f = fopen(filename, "r"); // open file
    if(f == NULL) // check for open error
    {
       return NULL; // file not opened, returning
    }
    char *str = (char*)malloc(CONFIG_FILE_LINE_MAXLENGTH); // buffer for read line from file
    char *tok = NULL; // temp pointer for strings searching
    char *tmp = NULL;
    char *name = NULL, *value = NULL; // temp to allocate memory for names and values
    struct ConfigNode *begin = NULL; // begin of config nodes
    int len = 0; // temporary length var
    char firstLetter = ' ';
    while(!feof(f)) // while end of file not reached
    {
        memset(str, 0, CONFIG_FILE_LINE_MAXLENGTH);
        fgets(str, CONFIG_FILE_LINE_MAXLENGTH - 1, f); // read line from file

        // check for empty lines
        // [Sections] lines
        // ; and # comments lines
        // and skip all of them
        if (strlen(str) == 0)
        {
            continue;
        }
        firstLetter = str[0];
        if (firstLetter == '[' || firstLetter == ';' ||
                firstLetter == '#' || firstLetter == '\n')
        {
            continue;
        }

        tmp = str;
        // need trim whitespaces
        tmp = trimWhiteSpaceRight(tmp);
        tok = strchr(tmp, '='); // search name, that before '='
        if (tok == NULL)
        {
            continue; // '=' not found, go to next line
        }
        // found, allocate memory for it
        len = tok - tmp; // calc string length
        // need trim whitespaces
        len -= trimWhiteSpaceLeftLength(tok);
        name = (char*)malloc(len + 1); // allocate memory for parameter name
        memset(name, 0, len + 1); // fill it with zero
        strncpy(name, tmp, len); // copy name
        // search value, that after =
        tmp = trimWhiteSpaceRight(tok + 1); // move to next symbol
        tok = strchr(tmp, '\n'); // search end of lne
        if (tok == NULL) // new line not found
        {
            tok = strchr(tmp, '\0'); // search for <end of line>, in case this line last
            if (tok == NULL) // not found, maybe empty parameter?
            {
                len = 0; // allocate 0 bytes for empty string
            } else {
                len = tok - tmp; // calc string length
                len -= trimWhiteSpaceLeftLength(tok);
            }
        }
        else {
            len = tok - tmp; // calc string length
            len -= trimWhiteSpaceLeftLength(tok);
        }
        value = (char*)malloc(len + 1); // allocate for value
        memset(value, 0, len + 1); // fill it with zero
        strncpy(value, tmp, len); // copy value

        // create new node.
        struct ConfigNode* node = (struct ConfigNode*)malloc(sizeof(struct ConfigNode));
        node->name = name; // store name
        node->value = value; // store value
        node->next = begin; // store previous node
        begin = node; // set this node first

    }
    free(str); // free buffer
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

int ConfigNodeInt(struct ConfigNode *begin, const char *name, int defaultValue)
{
    struct ConfigNode* node = ConfigGetNode(begin, name);
    if (node == NULL)
    {
        return defaultValue;
    }
    return ConfigValueInt(node);
}

char ConfigNodeChar(struct ConfigNode *begin, const char *name, char defaultValue)
{
    struct ConfigNode* node = ConfigGetNode(begin, name);
    if (node == NULL)
    {
        return defaultValue;
    }
    return ConfigValueChar(node);
}




float ConfigNodeFloat(struct ConfigNode *begin, const char *name, float defaultValue)
{
    struct ConfigNode* node = ConfigGetNode(begin, name);
    if (node == NULL)
    {
        return defaultValue;
    }
    return ConfigValueFloat(node);
}
