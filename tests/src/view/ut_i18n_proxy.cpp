#include "ut_i18n_proxy.h"
#include "view/i18n_proxy.h"
#include "src/third-party/stub/stub.h"

#include <QLocale>

ut_i18n_proxy::ut_i18n_proxy()
{

}

void ut_i18n_proxy::SetUp()
{
    ip = new I18nProxy;
}

void ut_i18n_proxy::TearDown()
{
    delete ip;
}

TEST_F(ut_i18n_proxy, getLocale)
{
    QVariantHash qh;
    qh = ip->getSentences();
    ASSERT_EQ(qh.size(), 7);
}

QString stub_en_US()
{
    return "en_US";
}

QString stub_zh_CN()
{
    return "zh_CN";
}

QString stub_other()
{
    return "other";
}

TEST_F(ut_i18n_proxy, getKeyword2)
{
    const QString locale = "en_US";
    Stub st;
    st.set(ADDR(QLocale, name), stub_en_US);
    ASSERT_EQ(ip->getLocale(), locale);
    st.reset(ADDR(QLocale, name));
}

TEST_F(ut_i18n_proxy, getKeyword3)
{
    const QString locale = "zh_CN";
    Stub st;
    st.set(ADDR(QLocale, name), stub_zh_CN);
    ASSERT_EQ(ip->getLocale(), locale);
    st.reset(ADDR(QLocale, name));
}

TEST_F(ut_i18n_proxy, getKeyword4)
{
    const QString locale = "";
    Stub st;
    st.set(ADDR(QLocale, name), stub_other);
    ASSERT_EQ(ip->getLocale(), locale);
    st.reset(ADDR(QLocale, name));
}
