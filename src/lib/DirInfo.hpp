#include <iostream>
#include <algorithm>
#include <fstream>
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
	int size;
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
		const int size = -1,
		const std::vector<DirInfo> &children = {}) : name(name), path(path), size(size), children(children) {}

	DirInfo(
		const DirInfo &d) : name(d.name), path(d.path), size(d.size), children(d.children) {}

	~DirInfo()
	{
	}

	DirInfo &operator=(const DirInfo &d)
	{
		if (this == &d)
			return *this;

		this->name = d.name;
		this->path = d.path;
		this->size = d.size;
		this->children = d.children;

		return *this;
	}

	void printDirInfo(int step)
	{
		printStep(step);
		std::cout << this->name << ' ' << this->size << std::endl;

		for (auto &child : this->children)
			child.printDirInfo(step + 2);
	}

	std::vector<DirInfo> getFileList() const
	{
		std::vector<DirInfo> result;
		if (this->children.size() == 0)
		{
			result.push_back(*this);
			return result;
		}
		std::vector<std::vector<DirInfo>> result2;

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

std::vector<DirInfo> getFilesRecursively(std::string dirname)
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

		std::ifstream file(joined_path.c_str(), std::ifstream::ate);
		int size = file.tellg();
		file.close();

		children.push_back(DirInfo(dpName, joined_path, size, getFilesRecursively(joined_path)));
	}

	closedir(dirp);
	return children;
}

} // namespace DirUtils