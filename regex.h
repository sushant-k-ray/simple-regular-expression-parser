#ifndef _REGEX_H_INCLUDED_
#define _REGEX_H_INCLUDED_

#include <vector>
#include <deque>
#include <utility>
#include <string_view>
#include <exception>
#include <stdexcept>

class regex
{
    struct character{
        std::vector<std::pair<char, char>> ranges;
        bool inverse = false;

        void add_range (char rangeStart, char rangeEnd)
        { ranges.push_back(std::make_pair(rangeStart, rangeEnd)); }

        void add_char  (char _character)
        { ranges.push_back(std::make_pair(_character, _character)); }

        template<typename T>
        void add_array (T& array)
        { ranges.insert(ranges.end(), array.cbegin(), array.cend()); }

        bool specialCharacter(char);

    public:
        void parse(std::string_view, bool);
        bool match(char);
    };

    struct state{
        enum { connective, alternative } type;

        character ch;

        union{ struct{ int match; };
               struct{ int stateNext = 0; };
        };

        state(character ch, int match = 1)
            : type(connective), ch(ch), match(match) {}

        state(int state_next)
            : type(alternative), stateNext(state_next) {}
    };

    struct token{
        enum { character, disjunction,
               quantifier_one_zero,
               quantifier_one_many,
               quantifier_zero_many,
        } type;

        int startLocation;
        std::deque<state> stateVector;
    };

    std::vector<state> states;

public:

    class exception : public std::runtime_error
    {
    public:
        int location;
        exception( const char* string, int location = -1 )
            : std::runtime_error( string ), location( location ) {}
    };

    void parse(std::string_view);

    bool match(std::string_view);
    std::vector<std::pair<int, int>> find(std::string_view);
};

#endif
