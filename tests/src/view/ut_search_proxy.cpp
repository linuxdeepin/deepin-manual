#include "ut_search_proxy.h"

#include "view/search_proxy.h"
#include "view/web_window.h"
#include "view/widget/search_edit.h"

ut_search_proxy_test::ut_search_proxy_test()
{

}

void ut_search_proxy_test::SetUp()
{
    m_searchProxy = new SearchProxy;
}

void ut_search_proxy_test::TearDown()
{
    delete m_searchProxy;
}

TEST_F(ut_search_proxy_test, getKeyword)
{
    m_searchProxy->getKeyword("应用");
}

TEST_F(ut_search_proxy_test, updateSearch)
{

    m_searchProxy->updateSearch("应用");
}
