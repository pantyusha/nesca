#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <algorithm>
#include <qstring.h>
#include <vector>
#include <qdatetime.h>

#define STRSTR(buff, str) Utils::ustrstr(buff, str)

using namespace std;

template<typename charT>
struct my_equal {
    my_equal( const locale loc ) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        return tolower(ch1) == tolower(ch2);
#else
        return tolower(ch1, loc_) == tolower(ch2, loc_);
#endif
    }
private:
    const locale& loc_;
};

class Utils {
private: static std::string startDate;
private: static std::string startTime;
private: static std::string currentTarget;
public:
	static int isDigest(const std::string *buffer);

    // find substring (case insensitive)
    template<typename T> static int ustrstr(const T& str1,
                                            const T& str2,
                                            const locale& loc = locale()) {

        auto it = std::search(str1.begin(), str1.end(), str2.begin(), str2.end(),
                              my_equal<typename T::value_type>(loc));
        if(it != str1.end()) return it - str1.begin();
        else return -1;
	}

	template<typename T> static int ustrstr(const T& str1,
		const char* str2c,
		const locale& loc = locale()) {

		std::string str2 = std::string(str2c);
		auto it = std::search(str1.begin(), str1.end(), str2.begin(), str2.end(),
			my_equal<typename T::value_type>(loc));
		if (it != str1.end()) return it - str1.begin();
		else return -1;
	}

	template<typename T> static int ustrstr(T *str1,
		const char* str2c,
		const locale& loc = locale()) {

		std::string str2 = std::string(str2c);
		auto it = std::search(str1->begin(), str1->end(), str2.begin(), str2.end(),
			my_equal<typename T::value_type>(loc));
		if (it != str1->end()) return it - str1->begin();
		else return -1;
	}

	static QString GetNSErrorDefinition(const char *str, const char *elem){
		const char *temp = strstr(str, elem);

		if (temp != NULL)
		{
			char definition[128] = { 0 };
			const char *firstComma = strstr(temp + strlen(elem) + 1, "\"");
			const char *lastComma = strstr(firstComma + 1, "\"");

			int sz = lastComma - firstComma - 1;

			strncpy(definition, firstComma + 1, (sz < 128 ? sz : 128));

			return QString(definition);
		}
		else return QString("No definition found!");
	}

    char * getProxy();
    int getProxyPort();
	static std::string getStrValue(const std::string &data, const std::string &delim1, const std::string &delim2);
	static std::vector<std::string> splitToStrVector(const std::string &s, char delim);
	static std::vector<int> splitToIntVector(const std::string &s, char delim);
	static void saveStartDate();
	static void saveStartTime();
	static std::string getStartDate();
	static std::string getStartTime();
	static void setCurrentTarget(const std::string target);
	static std::string getCurrentTarget();
	static void emitScaryError();
	static int addBARow(QString str1, QString str2, QString str3, int rowIndex);
	static std::string getHeaderValue(std::string *buff, const std::string headerValue, const std::string outputName);
};

#endif // UTILS_H
