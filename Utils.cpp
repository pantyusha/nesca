#include <Utils.h>

template<typename T> int Utils::ci_find_substr(const T& str1,
                                        const T& str2,
                                        const std::locale& locale) {

    auto it = std::search(str1.begin, str1.end, str2.begin, str2.end,
                          my_equal<typename T::value_type>(locale));
    if(it != str1.end()) return it - str1.begin();
    else return -1;
}
