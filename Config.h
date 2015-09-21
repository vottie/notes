#ifndef CONFIG_H_
#define CONFIG_H_

#include <iostream>
#include <map>

namespace vottie {
namespace notes {

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
};

}
} // end of namespace

#endif // CONFIG_H_
