#include <stdio.h>
#include "iniconfig.h"

int main(int argc, char *argv[])
{
    printf("Hello World!\n");

    struct ConfigNode* config = ConfigLoad("test.ini");
    // show starlife in int
    // first - find node
    // second get data in int value
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
        printf("%s\n", ConfigValueString(result));
    }

    ConfigFree(config);
    return 0;
}
