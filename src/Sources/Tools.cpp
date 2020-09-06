#include "Tools.hpp"

bool dirExists(char* path)
{
	struct stat info;

	if (stat(path, &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else
		return false;
}

void Log(std::string message)
{
	std::string error_message = "Error: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Something went wrong!", MB_ICONERROR);
}

void Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::string error_message = "Error: " + message + "\n" + wchar_to_char(error.ErrorMessage());
	std::wstring e_message = StringToWide(error_message);
	MessageBoxW(NULL, (LPCWSTR)e_message.c_str(), L"Something went wrong!", MB_ICONERROR);
}

std::wstring StringToWide(std::string str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}

std::vector<std::string> readFile(std::string filename) {
	std::string STRING;
	std::vector<std::string> output;
	std::ifstream infile;
	infile.open(filename);
	while (infile.eof() == false) // To get you all the lines.
	{
		getline(infile, STRING); // Saves the line in STRING.
		output.push_back(STRING);
	}
	infile.close();
	return output;
}

char* wchar_to_char(const wchar_t* pwchar)
{
	// get the number of characters in the string.
	int currentCharIndex = 0;
	char currentChar = (char)pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = (char)pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	// allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		// convert to char (1 byte)
		char character = (char)pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);
	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}

void  StringtoChar(std::string s, char& outStr)
{
	char cstr[sizeof(s) + 1];
	s.copy(cstr, s.size() + 1);
	cstr[s.size()] = '\0';
	const char* out = cstr;
	//strcpy_s((char*)outStr, out);
	outStr = (char)out;
}

char* concatenchar(char array1[], char array2[]) {
	char* newArray = new char[std::strlen(array1) + std::strlen(array2) + 1];
	strcpy(newArray, array1);
	strcat(newArray, array2);
	return newArray;
	//delete[] newArray;
}

char* concatenchar(char array1[], const char array2[]) {
	char* newArray = new char[std::strlen(array1) + std::strlen(array2) + 1];
	std::strcpy(newArray, array1);
	std::strcat(newArray, array2);
	return newArray;
	//delete[] newArray;
}