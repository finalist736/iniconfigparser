#include <stdlib.h>
#include <stdio.h>
#include "iniconfig.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: %s <config.ini>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct ConfigNode* config = ConfigLoad(argv[1]);
    if (config == NULL)
    {
        fprintf(stderr, "no such config file: <%s>\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("%c\n", ConfigNodeChar(config, "test", 'a')); // with default value
    struct ConfigNode* result = ConfigGetNode(config, "test");
    if (result != NULL)
    {
        printf("%c\n", ConfigValueChar(result));
    }

    result = ConfigGetNode(config, "count");
    if (result != NULL)
    {
        printf("%d\n", ConfigValueInt(result));
    }

    result = ConfigGetNode(config, "starlife");
    if (result != NULL)
    {
        printf("%d\n", ConfigValueInt(result));
    }

    result = ConfigGetNode(config, "justforfun");
    if (result != NULL)
    {
        printf("%s\n", ConfigValueString(result));
    }

    result = ConfigGetNode(config, "novalue");
    if (result != NULL)
    {
        printf("this is ok, if this is empty: %s\n", ConfigValueString(result));
    }

    result = ConfigGetNode(config, "nonexists");
    if (result == NULL)
    {
        printf("non exists config node!\n");
    }

    ConfigFree(config); // don't forget to free config chain
    return 0;
}
