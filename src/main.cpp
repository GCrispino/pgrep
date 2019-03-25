#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <dirent.h>
#include <pthread.h>
#include <DirInfo.hpp>

struct thread_param
{
	std::vector<std::string> *arrRef;
	std::regex regex;
	int start, end, n;
};

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

	std::cout << "File path: " << filePath << std::endl;
	if (file)
	{
		std::string line;
		unsigned lineI = 0;
		while (getline(file, line))
		{
			// std::cout << "line: " << line << std::endl;
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

void *searchRegexFiles(void *args)
{
	struct thread_param *params = (struct thread_param *)args;

	std::cout << "Iniciando searchRegexFiles: " << params->start << ',' << params->end << std::endl;
	std::vector<std::string> *vec = params->arrRef;
	std::cout << "  Arquivos: " << std::endl;
	for (int i = params->start; i <= params->end && i < params->n; ++i)
	{

		std::cout << "    " << (*vec)[i] << std::endl;
	}
	for (int i = params->start; i <= params->end && i < params->n; ++i)
	{

		searchRegexFile(params->regex, (*vec)[i]);
	}
	std::cout << "Finalizar searchRegexFiles: " << params->start << ',' << params->end << std::endl;

	return NULL;
	//------------------------------------------------------------------------------
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
	std::vector<struct thread_param> params(maxThreads);

	std::vector<pthread_t> threads(maxThreads);
	std::regex regex(regexStr);
	std::string dirInput(searchPath);
	DirUtils::DirInfo dirInfo(dirInput, dirInput, DirUtils::getFilesRecursively(dirInput, 0));

	std::cout << "Max thread number: " << maxThreads << std::endl;
	std::cout << "Regex: " << regexStr << std::endl;
	std::cout << "Search path: " << searchPath << std::endl;
	std::vector<std::string> res = dirInfo.getFileList();

	for (int i = 0; i < maxThreads; ++i)
	{
		unsigned n = res.size();
		int d = ceil((float)n / maxThreads);
		params[i].arrRef = &res;
		params[i].start = d * i;
		params[i].end = d * i + d - 1;
		params[i].regex = regex;
		params[i].n = n;

		pthread_create(&threads[i], NULL, searchRegexFiles, &params[i]);
	}

	for (int i = 0; i < maxThreads; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	return 0;
}
