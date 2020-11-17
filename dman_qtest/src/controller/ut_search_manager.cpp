#include "ut_search_manager.h"

#define private public
#include "controller/search_manager.h"
#undef private

namespace dman {

ut_search_manager_test::ut_search_manager_test()
{

}

TEST_F(ut_search_manager_test, initSearchManager)
{
    SearchManager sm;
}
}
