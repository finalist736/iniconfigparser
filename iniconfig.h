#ifndef INICONFIG_H
#define INICONFIG_H

struct ConfigNode {
    char *name;
    char *value;
    struct ConfigNode *next;
};

struct ConfigNode *ConfigLoad(const char* filename);
void ConfigFree(struct ConfigNode* begin);

struct ConfigNode *ConfigGetNode(struct ConfigNode* begin, const char* name);

int ConfigValueInt(struct ConfigNode* node);
char ConfigValueChar(struct ConfigNode* node);
float ConfigValueFloat(struct ConfigNode* node);
char *ConfigValueString(struct ConfigNode* node);

#endif // INICONFIG_H
