#include <fstream>
#include <iostream>
#include <regex>
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

//Returns line numbers of matches of the regex on the file designated by filePath
std::vector<unsigned> searchRegexFile(std::regex expr, std::string filePath)
{
	std::ifstream file(filePath);
	std::vector<unsigned> matches;

	if (file)
	{
		std::string line;
		unsigned lineI = 0;
		while (getline(file, line))
		{
			std::smatch m;
			if (std::regex_search(line, m, expr))
			{
				matches.push_back(lineI);
			}
			++lineI;
		}
	}

	file.close();
	return matches;
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
	std::string regexStr(argv[2]), searchPath(argv[3]);

	std::vector<pthread_t> threads(maxThreads);
	std::regex regex(regexStr);
	std::string dirInput(searchPath);
	DirUtils::DirInfo dirInfo(dirInput, dirInput, DirUtils::getFilesRecursively(dirInput, 0));

	std::cout << "Max thread number: " << maxThreads << std::endl;
	std::cout << "Regex: " << regexStr << std::endl;
	std::cout << "Search path: " << searchPath << std::endl;
	std::vector<std::string> res = dirInfo.getFileList();

	std::vector<unsigned> lineMatches = searchRegexFile(regex, res[0]);

	for (unsigned matchLine : lineMatches)
	{
		std::cout << res[0] << ':' << matchLine << std::endl;
	}
}
