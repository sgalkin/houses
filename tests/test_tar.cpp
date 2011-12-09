#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include "../tar.h"
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <set>

BOOST_AUTO_TEST_CASE( test_tar_throw ) {
    BOOST_CHECK_THROW(Tar("/nonexistent"), TarError);
    BOOST_CHECK_THROW(Tar("Makefile"), TarError);
    BOOST_CHECK_NO_THROW(Tar("/dev/null"));
    BOOST_CHECK_NO_THROW(Tar("tests/test.tar"));
}

BOOST_AUTO_TEST_CASE( test_tar_list ) {
    BOOST_CHECK_EQUAL(Tar("/dev/null").list().size(), 0);
    std::set<std::string> expected = { "baz", "test/", "test/foo", "test/bar" };
    std::vector<std::string> list = Tar("tests/test.tar").list();
    std::set<std::string> actual(list.begin(), list.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), 
                                  list.begin(), list.end());
}

BOOST_AUTO_TEST_CASE( test_tar_extract ) {
    Tar tar("tests/test.tar");
    BOOST_CHECK_THROW(tar.extract("nonexistent"), TarError);
    for(auto name: { "baz", "test/foo", "test/bar" }) {
        BOOST_CHECK_EQUAL(tar.extract(name), 
                          boost::filesystem::basename(name) + "_content\n");
    }
}
