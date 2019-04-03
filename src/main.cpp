#include <algorithm>
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
	std::vector<DirUtils::DirInfo> *arrRef;
	std::regex regex;
	int start, end, n;
};

int getMaxThreads(char **argv)
{
	int n_threads = atoi(argv[1]);
	if (!n_threads)
	{
		std::cout << "Error while reading thread max number!" << std::endl;
		exit(1);
	}

	return n_threads;
}

int getRegex(char **argv)
{
	int n_threads = atoi(argv[1]);
	if (!n_threads)
	{
		std::cout << "Error while reading regex!" << std::endl;
		exit(1);
	}

	return n_threads;
}

int getSearchPath(char **argv)
{
	int n_threads = atoi(argv[1]);
	if (!n_threads)
	{
		std::cout << "Error while reading max thread number!" << std::endl;
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

pthread_mutex_t lock;

void *searchRegexFiles(void *args)
{
	struct thread_param *params = (struct thread_param *)args;

	std::vector<DirUtils::DirInfo> *vec = params->arrRef;

	for (int i = params->start; i <= params->end && i < params->n; ++i)
	{

		std::vector<unsigned> matches = searchRegexFile(params->regex, (*vec)[i].path);

		pthread_mutex_lock(&lock);
		for (unsigned match : matches)
		{
			std::cout << (*vec)[i].path << ": " << match << std::endl;
		}
		pthread_mutex_unlock(&lock);
	}
	std::cout<< std::endl;

	return NULL;
}

bool compareFiles(const DirUtils::DirInfo &file1, const DirUtils::DirInfo &file2)
{
	return file1.size > file2.size;
}

std::vector<DirUtils::DirInfo> sortFilesToThreads(unsigned nThreads, const std::vector<DirUtils::DirInfo> &files)
{
	std::vector<DirUtils::DirInfo> sortedVec(files);

	std::sort(sortedVec.begin(), sortedVec.end(), compareFiles);

	for (unsigned i = 1, j = sortedVec.size() - 1; i < sortedVec.size() / 2; i += 2, j -= 2)
	{
		std::swap(sortedVec[i], sortedVec[j]);
	}

	return sortedVec;
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
	DirUtils::DirInfo dirInfo(dirInput, dirInput, -1, DirUtils::getFilesRecursively(dirInput));

	// std::cout << "Max thread number: " << maxThreads << std::endl;
	// std::cout << "Regex: " << regexStr << std::endl;
	// std::cout << "Search path: " << searchPath << std::endl;
	std::vector<DirUtils::DirInfo> res = dirInfo.getFileList();
	std::vector<DirUtils::DirInfo> sortedFiles = sortFilesToThreads(maxThreads, res);

	for (int i = 0; i < maxThreads; ++i)
	{
		unsigned n = res.size();
		int maxFilesPerThread = ceil((float)n / maxThreads);

		params[i].arrRef = &sortedFiles;
		params[i].start = maxFilesPerThread * i;
		params[i].end = maxFilesPerThread * i + maxFilesPerThread - 1;
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
