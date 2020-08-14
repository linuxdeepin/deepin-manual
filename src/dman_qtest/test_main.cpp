// test_main.cpp 测试入口

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QApplication>
#include <QTest>
int main(int argc, char *argv[])
{
    /*
       Q_UNUSED(argc)
       Q_UNUSED(argv)*/
    QApplication app(argc, argv);

    testing::InitGoogleTest();

    return RUN_ALL_TESTS();
}


