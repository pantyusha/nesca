#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <algorithm>

template<typename charT>
struct my_equal {
    my_equal( const std::locale loc ) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
        return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
    }
private:
    const std::locale& loc_;
};

class Utils {
public:
    // find substring (case insensitive)
    template<typename T> static int ci_find_substr(const T& str1,
                                            const T& str2,
                                            const std::locale& loc = std::locale()) {

        auto it = std::search(str1.begin(), str1.end(), str2.begin(), str2.end(),
                              my_equal<typename T::value_type>(loc));
        if(it != str1.end()) return it - str1.begin();
        else return -1;
    }

    char * getProxy();
    int getProxyPort();
};

#endif // UTILS_H
