#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <DirInfo.hpp>

int main()
{
	std::string dirInput(".");
	DirUtils::DirInfo dirInfo(dirInput, dirInput, DirUtils::getFilesRecursively(dirInput, 0));

	std::cout << dirInfo << std::endl;
}
