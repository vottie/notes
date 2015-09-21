#ifndef NOTES_H_
#define NOTES_H_

namespace vottie {
namespace notes {

/**
 * @class Notes
 */
class Notes {
public:
    Notes() {}

    ~Notes() {}

    int printHeader(std::string &title);
    int printBody(std::string &note);
    int printFooter();
    int parse(char const *request_uri);

private:
    void indexing();

};

}
} // end of namespace

#endif // NOTES_H_

