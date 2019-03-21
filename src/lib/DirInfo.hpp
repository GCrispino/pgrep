#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

void printStep(int step)
{
	while (--step >= 0)
		printf(" ", step);
}

namespace DirUtils
{
struct DirInfo
{

	std::string name;
	std::string path;
	std::vector<DirInfo> children;

	friend std::ostream &operator<<(std::ostream &os, DirInfo &d)
	{
		d.printDirInfo(0);
		return os;
	}

  public:
	DirInfo(
		const std::string &name = "",
		const std::string &path = "",
		const std::vector<DirInfo> &children = {}) : name(name), path(path), children(children) {}

	~DirInfo()
	{
	}

	void printDirInfo(int step)
	{
		printStep(step);
		std::cout << this->name << std::endl;

		for (auto &child : this->children)
			child.printDirInfo(step + 2);
	}

	std::vector<std::string> getFileList() const
	{
		std::vector<std::string> result;
		if (this->children.size() == 0)
		{
			std::cout << "oi" << this->path << std::endl;
			result.push_back(this->path);
			return result;
		}
		std::vector<std::vector<std::string>> result2;

		// transform each children in a vector of its sub-files (if any)
		for (auto &c : this->children)
		{
			result2.push_back(c.getFileList());
		}

		for (auto &v : result2)
		{
			//merge array
			result.insert(result.end(), v.begin(), v.end());
		}
		return result;
	}
};

std::vector<DirInfo> getFilesRecursively(std::string dirname, int step)
{
	DIR *dirp = opendir(dirname.c_str());
	struct dirent *dp;
	std::vector<DirInfo> children;

	if (dirp == NULL)
	{
		if (errno == ENOTDIR)
		{
			return children;
		}
		std::cout << "Erro ao abrir caminho " << dirname.c_str() << std::endl;
		std::cout << "Código de erro: " << errno << std::endl;
		exit(errno);
	}

	while ((dp = readdir(dirp)) != NULL)
	{
		std::string dpName(dp->d_name);
		if (dpName == "." || dpName == "..")
		{
			continue;
		}

		std::string joined_path(dirname + '/' + dpName);
		children.push_back(DirInfo(dpName, joined_path, getFilesRecursively(joined_path, step + 2)));
	}

	closedir(dirp);
	return children;
}

} // namespace DirUtils