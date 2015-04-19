#include "Utils.h"
#include <sstream>

std::vector<std::string> Utils::splitToStrVector(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}

	return elems;
}
std::vector<int> Utils::splitToIntVector(const std::string &s, char delim) {
	std::vector<int> elems;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		elems.push_back(std::stoi(item));
	}

	return elems;
}

std::string Utils::getStrValue(const std::string &data, const std::string &delim1, const std::string &delim2) {
	int pos1 = data.find(delim1);
	int pos2;
	int offset;

	if (pos1 != std::string::npos) {
		offset = delim1.length();
		pos2 = data.find(delim2, pos1 + offset);
		if (pos2 != std::string::npos) {
			return data.substr(pos1 + offset, pos2 - pos1 - offset);
		}
	}
	return "";
}

char *getSystemProxy() {
	return "";
}

int Utils::getProxyPort() {
	return 0;
}

char * Utils::getProxy() {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
    getSystemProxy();
#endif
	return "";
}