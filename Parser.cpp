#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <fcgi_stdio.h>
#include "Parser.h"
#include "Config.h"

using namespace std;
using namespace boost::filesystem;
using namespace vottie::notes;

void Parser::display()
{
    printf("%s", output_str.c_str());
    //std::cout << output_str << std::endl;
}


void Parser::execute(string &line)
{
    boost::regex reg_h("^#+");
    // TODO nested list - +
    boost::regex reg_list1("\\*[\\s\\t]");
    boost::regex reg_list2("\\-[\\s\\t]");
    boost::regex reg_list3("\\+[\\s\\t]");

    boost::regex reg_empty_line("\\n");
    boost::regex reg_bq("^>+");
    boost::regex reg_br("  $");
    boost::regex reg_hr("-{3,}$");
    boost::regex reg_link("\\[.+?\\]\\(.+?\\)");
    // TODO strong ** or __
    // TODO code ``
    // TODO complex conditon (EX: line has list and br...)

    //printf("%s\n", line.c_str());
    boost::match_results<string::const_iterator> m;

    // <h1>, <h2>, <h3>...
    if (boost::regex_search(line, m, reg_h))
    {
        string str;
        if (m.str().size() == 1)
        {
            str = regex_replace(line, reg_h, "<h1>");
            str.append("</h1>");
        }
        else if (m.str().size() == 2)
        {
            str = regex_replace(line, reg_h, "<h2>");
            str.append("</h2>");
        }
        else
        {
            str = regex_replace(line, reg_h, "<h3>");
            str.append("</h3>");
        } 
        output_str.append(str);
        output_str.append("\n");
    }
    // ul
    else if(boost::regex_search(line, reg_list1))
    {
        makeList(line, reg_list1);
    }
    else if(boost::regex_search(line, reg_list2))
    {
        makeList(line, reg_list2);
    }
    else if(boost::regex_search(line, reg_list3))
    {
        makeList(line, reg_list3);
    }
    // blockquote
    // TODO nested blockquote
    else if(boost::regex_search(line, reg_bq))
    {
        if (!bqflag) {
            bqflag = true;
            output_str.append("<blockquote>\n");
        }
        string str = regex_replace(line, reg_bq, "");
        output_str.append(str);
        output_str.append("\n");
    }
    // br
    else if(boost::regex_search(line, reg_br))
    {
        string str = regex_replace(line, reg_br, "<br>");
        output_str.append(str);
        output_str.append("\n");
    }
    // link
    else if (boost::regex_search(line, m, reg_link))
    {
        // TODO  multiple links in line, not parsed.
        // TODO  single quote sanitaize.
        string link = makeLink(m.str(0));
        string str = regex_replace(line, reg_link, link);
        output_str.append(str);
        output_str.append("\n");
    }
    // draw vertical line
    else if (boost::regex_search(line, reg_hr))
    {
        string str = regex_replace(line, reg_hr, "<hr />");
        output_str.append(str);
        output_str.append("\n");
    }
    // 
    else if (line.empty())
    {
        if (ulflag == true) {
            ulflag = false;
            output_str.append("</ul>");
        }
        if (pflag == true) {
            pflag = false;
            output_str.append("</p>\n");
        }
        if (bqflag == true) {
            bqflag = false;
            output_str.append("</blockquote>\n");
        }
        output_str.append("\n");
    }
    // 
    else
    {
        if (!pflag) {
            pflag = true;
            output_str.append("<p>");
        }
        output_str.append(line);
    }
}

int Parser::parse(string &note) {
    string note_path = Config::getInstance()->getServerPath();

    note_path.append(note);
    const path path(note_path);
    boost::system::error_code error;
    const bool result = exists(path, error);
    if (!result || error) {
        std::cout << path << std::endl;
        std::cout << "not found" << std::endl;
        return -1;
    }

    const std::time_t last_update = last_write_time(path);
    const boost::posix_time::ptime time = boost::posix_time::from_time_t(last_update);
    //std::cout << time << std::endl;

    std::ifstream ifs(path.c_str());
    //  No Exist File is Error
    if (ifs.fail()) {
        std::cerr << "fs fail" << std::endl;
        return -1;
    }
    string line;

    output_str.append(Config::getInstance()->getMdHeader());
    output_str.append("last update: ");
    output_str.append(boost::posix_time::to_simple_string(time));
    output_str.append("\n");
    //   while EOF Read Line
    while (getline(ifs, line))
    {
        execute(line);
    }

    output_str.append(Config::getInstance()->getMdFooter());

    return 0;
}


/***************************************
 *  private method
 **************************************/
string Parser::makeLink(string src)
{
    boost::regex reg_text("\\[.+?\\]");
    boost::regex reg_url("\\(.+?\\)");
    boost::match_results<string::const_iterator> m;

    string link = "<a href=\"";

    // input url
    if (boost::regex_search(src, m, reg_url))
    {
        //  1 is '[' trim
        // -2 is '[]' length trim
        string url = m.str(0).substr(1, m.str(0).size()-2);
        link.append(url);
    }
    link.append("\">");

    // input text
    if (boost::regex_search(src, m, reg_text))
    {
        string url_text = m.str(0).substr(1, m.str(0).size()-2);
        link.append(url_text);
    }
    link.append("</a>");
    
    return link;
}


void Parser::makeList(string &line, boost::regex reg_list)
{
    if (!ulflag) {
        ulflag = true;
        output_str.append("<ul>");
    }
    string str = regex_replace(line, reg_list, "<li>");
    str.append("</li>");
    output_str.append(str);
    output_str.append("\n");



}
