#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include "../utils.h"
#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_path2id ) {
    BOOST_CHECK_EQUAL("file___", path2id(""));
    BOOST_CHECK_EQUAL("file___foo", path2id("/foo"));
    BOOST_CHECK_EQUAL("file___bar", path2id("/foo/bar"));
    BOOST_CHECK_EQUAL("file___foo", path2id("/foo.jpg"));
    BOOST_CHECK_EQUAL("file___baz_orig", path2id("/foo/bar/baz.orig.jpg"));
}

BOOST_AUTO_TEST_CASE( test_id2path ) {
    BOOST_CHECK_EQUAL("", id2path(""));
    BOOST_CHECK_EQUAL("", id2path("file___"));
    BOOST_CHECK_EQUAL("foo", id2path("file___foo"));
    BOOST_CHECK_EQUAL("bar", id2path("file___bar"));
    BOOST_CHECK_EQUAL("foo.file...", id2path("foo_file___"));
    BOOST_CHECK_EQUAL("baz.orig", id2path("file___baz_orig"));
}
