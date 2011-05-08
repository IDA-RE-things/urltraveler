
#include <stdlib.h>
#include <string.h>
#include "KeyManager.h"

int KeyManager::GetServerKey(char key[16], short version)
{
    memcpy(key, "aaaaaaaaaaaaaaaa", 16);
    return 1;
}

int KeyManager::GetClientKey(char key[16], short version)
{
    memcpy(key, "Authentic@51.COM", 16);
    return 1;
}

int KeyManager::GetSessionKey(char key[16], short version)
{
    memcpy(key, "aaaaaaaaaaaaaaaa", 16);
    return 1;
}

int KeyManager::GetKeyBase(char *str, short version, short& len)
{
    memcpy(str, "51Auth", 6);
    len = 6;
    return 1;
}



