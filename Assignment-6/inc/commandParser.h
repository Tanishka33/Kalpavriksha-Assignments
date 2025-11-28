#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include "file.h"        
#include "directory.h"  

void handleDirectoryCommands(char* command);
void handleFileCommands(char* command);
int handleUtilityCommands(char* command);

#endif
