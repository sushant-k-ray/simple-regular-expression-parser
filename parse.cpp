#include "regex.h"

void regex::parse(std::string_view string)
{
    states.clear();
    std::vector<token> tokens;

    //tokenise the input
    auto addCharacter = [&](int startLocation,
                            int length,
                            bool charactersClass = false){
        try{
            character c;
            c.parse(string.substr(startLocation, length), charactersClass);
            tokens.push_back({token::character, startLocation, {state(c, 1)}});

        }catch(exception e){
            e.location += startLocation;
            throw e;
        }
    };

    std::vector<int> parenthesisStack;
    std::vector<std::pair<int, int>> evaluationList;

    for(int i = 0; i < string.length(); i++)
        switch(string[i])
        {
            case '(':
                parenthesisStack.push_back(tokens.size());
                break;

            case ')':
                if(parenthesisStack.size() == 0)
                    throw exception("stray \')\' without previous \'(\'", i);

                evaluationList.push_back( std::make_pair(
                    parenthesisStack.back(), tokens.size()));

                parenthesisStack.pop_back();
                break;

            case '\\':
                if(i + 1 == string.length())
                    throw exception("stray \'\\\' at the end of the regex", i);

                addCharacter(i, 2);
                i += 1;

                break;

            case '[':
            {
                int characterClassStart = i;
                while (++i < string.length()) {
                    if (string[i] == '\\'){
                        i++;
                        continue;
                    }

                    if (string[i] == ']') {
                        addCharacter(characterClassStart + 1,
                                     i - characterClassStart - 1, true);

                        break;
                    }
                }
            }

            break;

            case '+':
                tokens.push_back({ token::quantifier_one_many, i });
                break;

            case '*':
                tokens.push_back({ token::quantifier_zero_many, i });
                break;

            case '?':
                tokens.push_back({ token::quantifier_one_zero, i });
                break;

            case '|':
                tokens.push_back({ token::disjunction, i });
                break;

        default:
               addCharacter(i, 1);
        }

    if(parenthesisStack.size())
        throw exception("parenthesis is not closed",
                        tokens[parenthesisStack.back()].startLocation);

    evaluationList.push_back(std::make_pair(0, tokens.size()));

    //token collapsing
    for(int index = 0; index < evaluationList.size(); index++)
    {
        int& start = evaluationList[index].first;
        int& end   = evaluationList[index].second;

        int toRemove = end - start - 1;

        if(tokens[start].type != token::character)
            throw exception("invalid operator", tokens[start].startLocation);

        if(tokens[end - 1].type == token::disjunction)
            throw exception("invalid operator", tokens[end - 1].startLocation);

        //search for quantifiers
        for(int i = start + 1; i < end; i++)
        {
            auto& j = tokens[i - 1].stateVector;

            switch(tokens[i].type)
            {
            case token::quantifier_one_many:
                for(int k = j.size() - 1, l = 1; k >= 0 ; k--, l++)
                    if(j[k].match == l)
                        j[k].match += 1;

                j.push_back( state( -j.size() ));
                break;

            case token::quantifier_one_zero:
                j.push_front( state( j.size() + 1 ));
                break;

            case token::quantifier_zero_many:
                for(int k = j.size() - 1, l = 1; k >= 0 ; k--, l++)
                    if(j[k].match == l)
                        j[k].match = -k - 1;

                j.push_front( state( j.size() + 1 ));
                break;

            default:
                continue;
            }

            if(i + 1 < end && tokens[i + 1].type > token::disjunction)
                throw exception("multiple consecutive quantifiers",
                                tokens[i + 1].startLocation);

            tokens.erase(tokens.begin() + i--);
            end -= 1;
        }

        //search for disjunction
        for(int i = start + 1; i < end - 1; i++)
            if( tokens[i].type == token::disjunction ){
                auto& p = tokens[i - 1].stateVector;
                auto& n = tokens[i + 1].stateVector;

                for(int k = p.size() - 1, l = 1; k >= 0 ; k--, l++)
                    if(p[k].match == l)
                        p[k].match += n.size();

                p.push_front( state( p.size() + 1 ));
                p.insert( p.end(), n.begin(), n.end() );

                tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
                i -= 2;
                end -= 2;
            }

        //collapse the current evaluation vectors
        auto& s = tokens[start].stateVector;
        for(int i = start + 1; i < end; i++){
            auto& j = tokens[i].stateVector;
            s.insert( s.end(), j.begin(), j.end() );
        }

        tokens.erase(tokens.begin() + start + 1, tokens.begin() + end);

        //change the values for next items of evaluationList
        //due to collapse of current item
        for(int i = index + 1; i < evaluationList.size(); i++)
        {
            auto& parenthesis = evaluationList[i];

            if(parenthesis.first > start){
                parenthesis.first -= toRemove;
                parenthesis.second -= toRemove;
            }

            else if(parenthesis.second > start)
                parenthesis.second -= toRemove;
        }
    }

    if(tokens.size() != 1)
        throw exception("invalid regular expression");

    for(auto& i : tokens[0].stateVector) {
        //convert from relative to static indexing
        i.match += states.size();

        states.push_back(i);
    }
}
