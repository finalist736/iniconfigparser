#include "iniconfig.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void addNode(struct ConfigNode **n, char * name, char *val)
{
    if (*n == NULL)
    {
        *n = (struct ConfigNode*)malloc(sizeof(struct ConfigNode));
        (*n)->name = name;
        (*n)->value = val;
        (*n)->next = NULL;
    } else {
        addNode(&((*n)->next), name, val);
    }
}

struct ConfigNode* ConfigLoad(const char *filename)
{
    FILE * f;
    errno_t err = fopen_s(&f, filename, "r");
    if(err != 0)
    {
       printf("file open error: %d\n", err);
       return NULL;
    }
    char str[100] = { 0 };
    char *tok = NULL;
    char *name = NULL, *value = NULL;
    struct ConfigNode *begin = NULL;
    while(!feof(f))
    {
        fgets(str, 99, f);
        char *tmp = str;
        // search name, that before =
        tok = strchr(tmp, '=');
        if (tok == NULL)
        {
            continue;
        }
        // found, allocate memory for it
        int len = tok - tmp;
        name = (char*)malloc(len + 1);
        memset(name, 0, len + 1);
        strncpy(name, tmp, len);
        // search value, that after =
        tmp = tok + 1;
        tok = strchr(tmp, '\n');
        if (tok == NULL) // new line not found, allocate 0 bytes for empty string
        {
            tok = strchr(tmp, '\0');
            if (tok == NULL)
            {
                len = 0;
            } else {
                len = tok - tmp;
            }
        }
        else {
            len = tok - tmp;
        }
        value = (char*)malloc(len + 1);
        memset(value, 0, len + 1);
        strncpy(value, tmp, len);

        addNode(&begin, name, value); // create new node.
    }
    fclose(f);
    return begin;
}

void ConfigFree(struct ConfigNode *begin)
{
    if (begin == NULL)
    {
        return;
    }
    if (begin->next != NULL)
    {
        ConfigFree(begin->next);
    }
    free(begin->name);
    free(begin->value);
    free(begin);
}

struct ConfigNode *ConfigGetNode(struct ConfigNode* begin, const char *name)
{
    if (begin == NULL)
    {
        return begin;
    }
    if (strcmp(begin->name, name) == 0)
    {
        return begin;
    }
    return ConfigGetNode(begin->next, name);
}

int ConfigValueInt(struct ConfigNode *node)
{
    int v = 0;
    if (node != NULL)
    {
        v = atoi(node->value);
    }
    return v;
}

char ConfigValueChar(struct ConfigNode *node)
{
    char v = '\0';
    if (node != NULL)
    {
        if (strlen(node->value) > 0)
        {
            v = node->value[0];
        }
    }
    return v;
}

float ConfigValueFloat(struct ConfigNode *node)
{
    float v = 0;
    if (node != NULL)
    {
        v = atof(node->value);
    }
    return v;
}

char *ConfigValueString(struct ConfigNode *node)
{
    if (node != NULL)
    {
        return node->value;
    }
    return "";
}
