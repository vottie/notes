#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "Config.h"

using namespace std;
using namespace vottie::notes;

Config *Config::getInstance()
{
    static Config instance;
    return &instance;
}


int Config::load()
{
    int rtncode = 0;
    std::ifstream ifs("./config.txt");
    std::string str;
    if (ifs.fail()) {
        printf("load conf error\n");
        rtncode = -1;
        return rtncode;
    }

    while (getline(ifs, str)) {
        // skip empty line
        if (str.empty()) {
            continue;
        }

        std::size_t found = str.find_first_of('=');        
        if (found == std::string::npos) {
            printf("Configuration format Error\n");
            rtncode = -1;
            break;
        }

        string key = removeSpace(str, found, FIND_KEY);
        //string key = string(str.c_str(), found);
        // for debug
        // printf("key=%s\n", key.c_str());

        string val = removeSpace(str, found, FIND_VAL);
        //string val = str.substr(found + 1);
        // for debug
        // printf("val=%s\n", val.c_str());

        config[key] = val;
    }

    return rtncode;
}


// for debug
void Config::show()
{
    for (const auto &pair : config) {
        cout << pair.first
             << " = "
             << pair.second
             << '\n';
    }
}


string Config::getHeader()
{
    return config["header"]; 
}

string Config::getStyleSheet()
{
    return config["stylesheet"]; 
}

string Config::getFooter()
{
    return config["footer"]; 
}

string Config::getIdxHeader()
{
    return config["idx_header"]; 
}

string Config::getIdxFooter()
{
    return config["idx_footer"]; 
}

string Config::getURIBase()
{
    return config["uri_base"]; 
}

string Config::getSuffix()
{
    return config["suffix"]; 
}

string Config::getServerPath()
{
    return config["server_path"]; 
}

string Config::getLinkTag()
{
    return config["link_tag"]; 
}

string Config::getDomainName()
{
    return config["domain_name"]; 
}

string Config::getMdHeader()
{
    string md_header("<body><header style=\"background-color:");
    md_header.append(config["bg_color"]);
    md_header.append(";\" class=\"container\"><h1>");
    // remove "http://"
    md_header.append((config["domain_name"]).substr(7));
    md_header.append("</h1></header><section class=\"container\">");
    return md_header;
}

string Config::getMdFooter()
{
    string md_footer("<a href=\"");
    md_footer.append(config["domain_name"]);
    md_footer.append("\">back</a></p></section><hr/><footer style=\"background-color:");
    md_footer.append(config["bg_color"]);
    md_footer.append(";\" class=\"container\">");
    md_footer.append(config["copyright"]);
    md_footer.append("</footer>");
    return md_footer;
}

string Config::getAnalytics()
{
    return config["analytics"];
}


string Config::getAdsense()
{
    return config["adsense"];
}

// private method
string Config::removeSpace(string &str, size_t pos, int flag)
{
    string line;

    if (flag == FIND_KEY) {
        for(int i = pos - 1; i >= 0; i--) {
            if(str[i] != ' ') {
                line = str.substr(0, i + 1);
	        // printf("DBG key %s pos %d\n", line.c_str(), i);
                break;
            }
        }
    } else {
        for(int i = pos + 1; i <= (int)str.size(); i++) {
            if(str[i] != ' ') {
                line = str.substr(i);
	        // printf("DBG val %s pos %d\n", line.c_str(), i);
                break;
            }
        }
    }
    return line;
}
