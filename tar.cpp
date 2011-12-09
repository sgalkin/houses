#include "tar.h"

#include <libtar.h>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <fcntl.h>

namespace {
template<typename Exception>
void error(int err) {
    char buf[255];
    throw Exception(strerror_r(err, buf, sizeof(buf)));
}

class TmpFile {
public:
    TmpFile() {
        char buf[] = "tmp.XXXXXX";
        if((fd_ = mkstemp(buf)) == -1) 
            error<std::runtime_error>(errno);
        path_ = buf;
    }

    ~TmpFile() {
        unlink(path_.c_str());
    }

    const std::string& name() const { return path_; }
    
    std::string data() const {
        if(fsync(fd_) == -1) 
            error<std::runtime_error>(errno);
        off_t size = lseek(fd_, 0, SEEK_END);
        if(size == -1) 
            error<std::runtime_error>(errno);
        if(lseek(fd_, 0, SEEK_SET) == -1) 
            error<std::runtime_error>(errno);
        std::string data(size, 0);
        if(read(fd_, &data[0], size) == -1) 
            error<std::runtime_error>(errno);
        return data;
    }

private:
    int fd_;
    std::string path_;
};

}

class Tar::Handler {
public:
    explicit Handler(const std::string& path) {
        if(tar_open(&tar_, const_cast<char*>(path.c_str()), NULL, O_RDONLY, 0, 0) == -1) {
            error<TarError>(errno);
        }
    }

    ~Handler() {
        tar_close(tar_); // ignore error :(
    }

    operator TAR* () { return tar_; }
    TAR* operator->() { return tar_; }

private:
    TAR* tar_;
};

Tar::Tar(const std::string& path) : path_(path) {
    Handler tar(path_);
    while(true) {
        off_t off = lseek(tar->fd, 0, SEEK_CUR);
        int res = th_read(tar);
        if(res == 1) 
            break;
        if(res == -1) 
            error<TarError>(errno);
        offset_[th_get_pathname(tar)] = off;
        tar_skip_regfile(tar);        
    }
}

std::vector<std::string> Tar::list() const {
    std::vector<std::string> files;
    std::transform(offset_.begin(), offset_.end(), std::back_inserter(files), 
                   [](const std::map<std::string, off_t>::value_type& value) {
                       return value.first;
                   });
    return files;
}

std::string Tar::extract(const std::string& file) const {
    if(offset_.count(file) == 0) 
        throw TarError("file not found");
    Handler tar(path_);
    if(lseek(tar->fd, offset_.at(file), SEEK_SET) == -1) 
        error<std::runtime_error>(errno);
    if(th_read(tar) == -1) 
        error<TarError>(errno);
    TmpFile tmp;
    tar_extract_file(tar, const_cast<char*>(tmp.name().c_str()));
    return tmp.data();
}
