#ifndef CONFIG_H_
#define CONFIG_H_

#include <iostream>
#include <map>

namespace vottie {
namespace notes {

#define FIND_KEY 0
#define FIND_VAL 1

/**
 * @class Config
 */
class Config {
public:
    // Singleton
    static Config *getInstance();

    // configuration load
    int load();

    std::string getHeader();
    std::string getStyleSheet();
    std::string getFooter();
    std::string getIdxHeader();
    std::string getIdxFooter();
    std::string getURIBase();
    std::string getSuffix();
    std::string getServerPath();
    std::string getLinkTag();
    std::string getDomainName();
    std::string getMdHeader();
    std::string getMdFooter();

    // for debug
    void show();

private:
    Config(){}
    Config(const Config &other){}

    static Config *instance;

    std::map<std::string, std::string> config;

    std::string removeSpace(std::string &str, size_t pos, int flag);
};

}
} // end of namespace

#endif // CONFIG_H_
