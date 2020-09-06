#pragma once
#include "Dependances.hpp"

void Log(std::string message);
void Log(HRESULT hr, std::string message);
std::wstring StringToWide(std::string str);
bool dirExists(char* path);
std::vector<std::string> readFile(std::string filename);
char* wchar_to_char(const wchar_t* pwchar);
void  StringtoChar(std::string s, char& outStr);
char* concatenchar(char array1[], const char array2[]);
char* concatenchar(char array1[], char array2[]);

