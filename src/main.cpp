#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <DirInfo.hpp>

int getMaxThreads(char **argv)
{
	int n_threads = atoi(argv[1]);
	if (!n_threads)
	{
		std::cout << "Erro ao ler numero maximo de threads!" << std::endl;
		exit(1);
	}

	return n_threads;
}

int getRegex(char **argv)
{
	int n_threads = atoi(argv[1]);
	if (!n_threads)
	{
		std::cout << "Erro ao ler numero maximo de threads!" << std::endl;
		exit(1);
	}

	return n_threads;
}

int getSearchPath(char **argv)
{
	int n_threads = atoi(argv[1]);
	if (!n_threads)
	{
		std::cout << "Erro ao ler numero maximo de threads!" << std::endl;
		exit(1);
	}

	return n_threads;
}

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		std::cout << "usage: " << argv[0]
				  << " <max_threads> <regex> <search_path>" << std::endl;
		return 1;
	}

	int maxThreads = getMaxThreads(argv);
	std::string regex(argv[2]), searchPath(argv[3]);
	std::string dirInput(".");
	DirUtils::DirInfo dirInfo(dirInput, dirInput, DirUtils::getFilesRecursively(dirInput, 0));

	std::cout << "Max thread number: " << maxThreads << std::endl;
	std::cout << "Regex: " << regex << std::endl;
	std::cout << "Search path: " << searchPath << std::endl;
	std::vector<std::string> res = dirInfo.getFileList();
	std::cout << dirInfo << std::endl;
}
