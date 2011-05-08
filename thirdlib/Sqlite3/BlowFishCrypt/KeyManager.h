#pragma  once

class KeyManager
{
public:
    int GetServerKey(char key[16], short version);
    int GetClientKey(char key[16], short version);
    int GetSessionKey(char key[16], short version);
    int GetKeyBase(char *str, short version, short& len);
};


