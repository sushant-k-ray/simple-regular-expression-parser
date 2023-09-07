#include "regex.h"
#include <iostream>

int main()
{
    regex r;
    try{
        r.parse("abc*|def(ghi)*|(jkl)+a?");
        std::cout << r.match("abdefghighia") << std::endl;

        std::string_view strview = "abdefghighiabcefjklb";
        auto i = r.find(strview);

        for(auto j : i)
        {
            std::cout << j.first << "\t"
                      << j.second << "\t"
                      << strview.substr(j.first, j.second)
                      << std::endl;
        }

    } catch(regex::exception e) {
        std::cout << "ERROR : " << e.what() << " at location " << e.location << std::endl;
        return -1;
    }

    return 0;
}
