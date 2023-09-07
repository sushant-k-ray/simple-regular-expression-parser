#include "regex.h"
#include <vector>

bool regex::match(std::string_view string)
{
    std::vector<int> currentIndices{0};

    int stringIndex = 0;

    while(currentIndices.size() && stringIndex < string.size())
    {
        for(int i = 0; i < currentIndices.size(); i++){
            int j = currentIndices[i];

            if( j >= static_cast<int>(states.size()) || j == -1 ){
                currentIndices.erase(currentIndices.begin() + i--);
                continue;
            }

            if( states[j].type == state::connective )
                currentIndices[i] = states[j].ch.match(string[stringIndex])?
                                    states[j].match : -1;
            else {
                currentIndices[i--] += 1;
                currentIndices.push_back( states[j].stateNext );
            }
        }

        stringIndex += 1;
    }

    for(auto& i : currentIndices)
        if(i >= states.size())
            return true;

    return false;
}

std::vector<std::pair<int, int>> regex::find(std::string_view string)
{
    std::vector<std::pair<int, int>> searchList;

    for(int startPos = 0; startPos < string.length(); startPos++)
    {
        std::vector<int> currentIndices{0};
        int stringIndex = startPos;

        while(currentIndices.size() && stringIndex < string.size())
        {
            for(int i = 0; i < currentIndices.size(); i++){
                int j = currentIndices[i];

                if(j >= static_cast<int>(states.size())){
                    searchList.push_back(std::make_pair(startPos,
                                        stringIndex - startPos));

                    currentIndices.erase(currentIndices.begin() + i--);
                    continue;
                }

                if(j == -1){
                    currentIndices.erase(currentIndices.begin() + i--);
                    continue;
                }

                if( states[j].type == state::connective )
                    currentIndices[i]= states[j].ch.match(string[stringIndex])?
                                       states[j].match : -1;
                else {
                    currentIndices[i--] += 1;
                    currentIndices.push_back(states[j].stateNext);
                }
            }

            stringIndex += 1;
        }
    }

    return searchList;
}
