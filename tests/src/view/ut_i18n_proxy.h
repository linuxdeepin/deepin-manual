#ifndef UT_I18N_PROXY_H
#define UT_I18N_PROXY_H

#include "gtest/gtest.h"
#include <QTest>

class I18nProxy;

class ut_i18n_proxy : public testing::Test
{
public:
    ut_i18n_proxy();
    virtual void SetUp() override;
    virtual void TearDown() override;
    I18nProxy *ip = nullptr;

};

#endif // UT_I18N_PROXY_H
