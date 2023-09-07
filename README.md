# Simple Regular Expression Parser

## About
This project is a simple regular expression parser made for educational purposes. Lots of potential optimization are possible. The included sample.cpp gives a glimpse of how to use this for regular expressions

*Note* : A C++ 20 compliant compiler is required to build the project

---
## Usage
Include header file
`#include "regex.h"`
and then create an object of class regex.

To add regular expression to regex object,
call regex::parse function. For e.g. :
```
regex r;
r.parse("your regex here");
```

Now either call match() function to check if whole string matches the regular expression
```
r.match("your string here");
```
r.match() returns true if the string matches the regular expression

To find the substrings which follows the regex, use
```
r.find("your string here");
```
r.find() returns vector of std::pair, where 1<sup>st</sup> is the starting index of the string, and 2<sup>nd</sup> is the length of the matched substrings

*Note* : All the exceptions are handled by regex::exception object

---
## Supported Functionalities:   

This project uses a simple and fast algorithm(O(m*n), where m is the complexity of the regex). As a result not many modern functionalities, such as lazy quantifiers and group naming are available. Only the basic quantifiers and disjunction is supported as of now

---

## Known Bugs
* Let me know if there are any
