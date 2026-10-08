#include <string>

void rm_nonprinting(std::string &str) {
  str.erase(std::remove_if(
                str.begin(), str.end(),
                [](unsigned char c) { return std::iscntrl(c) && c != '\n'; }),
            str.end());
}
