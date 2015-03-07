#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <algorithm>

using namespace std;

template<typename charT>
struct my_equal {
    my_equal( const locale loc ) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		return toupper(ch1) == toupper(ch2);
#else
		return toupper(ch1, loc_) == toupper(ch2, loc_);
#endif
    }
private:
    const locale& loc_;
};

class Utils {
public:
    // find substring (case insensitive)
    template<typename T> static int ci_find_substr(const T& str1,
                                            const T& str2,
                                            const locale& loc = locale()) {

        auto it = std::search(str1.begin(), str1.end(), str2.begin(), str2.end(),
                              my_equal<typename T::value_type>(loc));
        if(it != str1.end()) return it - str1.begin();
        else return -1;
    }

    char * getProxy();
    int getProxyPort();
};

#endif // UTILS_H
