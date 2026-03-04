// clang-format off
#include <QTest>

#include "stringifiedinteger.h"

using namespace TestUtils;

class StringifiedIntegerTests : public QObject
{
    Q_OBJECT

private slots:
    void testEmptyStringifiedInteger();
    void testInitializationByString();
};

void StringifiedIntegerTests::testEmptyStringifiedInteger()
{
    StringifiedInteger stringifiedInt;
    QVERIFY(stringifiedInt.getValue() == "Z");
}

void StringifiedIntegerTests::testInitializationByString()
{
    StringifiedInteger stringifiedInt{""};
    QVERIFY(stringifiedInt.getValue() == "N");

    stringifiedInt = StringifiedInteger{"ZZZZZZZZ"};
    QVERIFY(stringifiedInt.getValue() == "ZZZZZZZZ");
}

QTEST_APPLESS_MAIN(StringifiedIntegerTests)

#include "tst_stringifiedintegertests.moc"
// clang-format on
