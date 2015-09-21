#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <boost/regex.hpp>

namespace vottie {
namespace notes {

/**
 * @class Parser
 */
class Parser {
public:
    Parser() 
        :pflag(false), ulflag(false), bqflag(false),
         ulcnt(0) {}

    ~Parser() {}

    int parse(std::string &note);
    void execute(std::string &line);
    void display();
private:
    std::string output_str;    
    bool pflag;
    bool ulflag;
    bool bqflag;

    int ulcnt;

    std::string makeLink(std::string src);
    void makeList(std::string &line, boost::regex reg_list);
};

}
} // end of namespace

#endif // PARSER_H_

