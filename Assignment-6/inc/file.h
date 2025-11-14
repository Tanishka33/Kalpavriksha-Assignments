#ifndef FILE_H
#define FILE_H

#include "directory.h"   

void createFile(char* name);
void writeFile(char* name, char* data);
void readFile(char* name);
void deleteFile(char* name);

#endif
