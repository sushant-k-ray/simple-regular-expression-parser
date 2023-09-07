#include "constants.h"
#include "regex.h"

bool regex::character::match(char _character)
{
    for(int i = 0; i < ranges.size(); i++)
        if(_character >= ranges[i].first && _character <= ranges[i].second)
            return !inverse;

    return inverse;
}

bool regex::character::specialCharacter(char _character)
{
    switch (_character) {
        case 'W':  add_array(antiAlphanum); break;
        case 'w':  add_array(alphanum); break;
        case 'S':  add_array(antiWhiteSpace); break;
        case 's':  add_array(whiteSpace); break;
        case 'D':  add_array(antiDigits); break;
        case 'd':  add_array(digits); break;
        case '^':  add_char('^'); break;
        case 'n':  add_char('\n'); break;
        case 't':  add_char('\t'); break;
        case 'r':  add_char('\r'); break;
        case 'v':  add_char('\v'); break;
        case 'f':  add_char('\f'); break;
        case 'b':  add_char('\b'); break;
        case '0':  add_char(0); break;
        case '\\': add_char('\\'); break;
        case '.':
        case '+':
        case '*':
        case '[':
        case ']':  add_char(_character);
            break;

        default:
            return false;
    }

    return true;
}

void regex::character::parse(std::string_view string, bool charGroup)
{
    int index = 0;

    enum{ lowerAlpha, upperAlpha, digit, special };
    auto type = [](char c)
    {
        switch(c)
        {
            case 'a' ... 'z' :
                return lowerAlpha;

            case 'A' ... 'Z' :
                return upperAlpha;

            case '0' ... '9' :
                return digit;

            default:
                return special;
        }
    };

    if (string[0] == '^' && charGroup) {
        inverse = true;
        index = 1;
    }

    while(index < string.length())
    {
        char c = string[index];

        switch(c)
        {
            case '\\':
                if(index + 1 >= string.length())
                    throw exception("escape sequence at end of string", index);

                if(!specialCharacter( string[index + 1] ))
                    throw exception("invalid escape sequence", index);

                index += 1;
                break;

            case 'a' ... 'z':
            case 'A' ... 'Z':
            case '0' ... '9':

                if( charGroup
                 && index + 2 < string.length()
                 && string[index + 1] == '-'
                 && type(string[index + 2]) == type(c))
                 {
                    char end = string[index + 2];
                    if(end < c)
                        throw exception("invalid character group range",
                                        index + 1);

                    add_range(c, end);

                    index += 2;
                    break;
                }

                add_char(c);
                break;

            default:
                add_char(c);
        }

        index += 1;
    }
}
