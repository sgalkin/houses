#include "utils.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/convenience.hpp>

namespace {
    const std::string PREFIX = "file___";
}

std::string path2id(const std::string& path) {
  if(path.empty()) return PREFIX; // for boost 1.47 + gcc-mp-4.6 boost::filesystem::basename error on empty string
  return PREFIX + boost::replace_all_copy(boost::filesystem::basename(path), ".", "_");
}

std::string id2path(const std::string& id) {
    return boost::replace_all_copy(boost::starts_with(id, PREFIX) ? 
                                   boost::erase_head_copy(id, PREFIX.size()) : id, "_", ".");
}
