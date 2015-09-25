#include <iostream>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <fcgi_stdio.h>
#include "Notes.h"
#include "Parser.h"
#include "Config.h"

using namespace std;
using namespace vottie::notes;

int Notes::printHeader(string &title) 
{
    printf("Content-type: text/html\r\nStatus: 200 OK\r\n\r\n");
    // TODO title is maybe japanese
    printf("%s", Config::getInstance()->getHeader().c_str());
    printf("<title>%s</title></head>\n", title.c_str());
    printf("%s", Config::getInstance()->getStyleSheet().c_str());
    return 0;
}

int Notes::printBody(string &note)
{
    Parser parser;
    parser.parse(note);
    parser.display();
    return 0;
}

int Notes::printFooter()
{
    printf("%s", Config::getInstance()->getFooter().c_str());
    return 0;
}

int Notes::parse(const char *request_uri)
{
    string uri = string(request_uri);
    // TODO fix string is not better.
    string uri_base = Config::getInstance()->getURIBase();
    // string uri_base_tmp = uri_base.erase(uri_base.size() - 1);
    string title = boost::algorithm::replace_all_copy(uri, uri_base, "");
    printHeader(title);

    // TODO index
    if (uri == uri_base ||
        // uri == uri_base ||
        uri == uri_base + "index.html" ||
        uri == uri_base + "index.htm") {
        indexing();
    } else {
        // note : /notes/example.md
        // uri  : /notes/example
        string note = uri.append(Config::getInstance()->getSuffix());
        printBody(note);
    }
    printFooter();
    return 0;
}


/******************************************************************************
 * private Method
 ******************************************************************************/
void Notes::indexing()
{
    Config *config = Config::getInstance();
    printf("%s", config->getIdxHeader().c_str());

    string idx_path = config->getServerPath();
    idx_path.append(config->getURIBase());
    const boost::filesystem::path path(idx_path);
    BOOST_FOREACH(const boost::filesystem::path& p,
                  std::make_pair(boost::filesystem::directory_iterator(path),
                                 boost::filesystem::directory_iterator())) {

        if (!boost::filesystem::is_directory(p)) {
            printf(config->getLinkTag().c_str(),
                   p.stem().string().c_str(), p.stem().string().c_str());
        }
    }
    printf("<p><a href=\"%s/\">back</a></p>", config->getDomainName().c_str());
    printf("%s", config->getIdxFooter().c_str());
}

int main(int argc, char* argv[]) {
    char *request_uri;

    // prepare config
    Config *config = Config::getInstance();
    config->load();

    while (FCGI_Accept() >= 0) {
        request_uri = getenv("REQUEST_URI");
        Notes notes;
        notes.parse(request_uri);
    }
    return 0;
}
