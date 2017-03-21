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

int ConfigNodeInt(struct ConfigNode* begin, const char* name, int defaultValue);
char ConfigNodeChar(struct ConfigNode* begin, const char* name, char defaultValue);
float ConfigNodeFloat(struct ConfigNode* begin, const char* name, float defaultValue);

#endif // INICONFIG_H
