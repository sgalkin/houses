#ifndef TAR_H_INCLUDED
#define TAR_H_INCLUDED

#include <string>
#include <vector>
#include <stdexcept>
#include <map>

class TarError : public std::runtime_error {
public:
    explicit TarError(const std::string& message) : 
        std::runtime_error(message) {}
};

class Tar {
    class Handler;
public:
    explicit Tar(const std::string& path);

    std::vector<std::string> list() const;
    std::string extract(const std::string& file) const;

private:
    std::string path_;
    std::map<std::string, off_t> offset_;
};

#endif // TAR_H_INCLUDED
