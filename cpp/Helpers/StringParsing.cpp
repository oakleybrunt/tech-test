#include <cctype>
#include <string>

static void rm_cntrlchars(std::string &str) {
  str.erase(std::remove_if(
                str.begin(), str.end(),
                [](unsigned char c) { return std::iscntrl(c) && c != '\n'; }),
            str.end());
}

static void rm_nonprinting(std::string &str) {
  str.erase(std::remove_if(str.begin(), str.end(),
                           [](unsigned char c) { return !std::isprint(c); }),
            str.end());
}
