// clang-format off
#include <QTest>

#include "stringifiedinteger.h"

class StringifiedIntegerTests : public QObject
{
    Q_OBJECT

private slots:
    void testEmptyStringifiedInteger();
};

void StringifiedIntegerTests::testEmptyStringifiedInteger()
{
    StringifiedInteger stringifiedInt;
    QVERIFY(stringifiedInt.getValue() == "Z");
}

QTEST_APPLESS_MAIN(StringifiedIntegerTests)

#include "tst_stringifiedintegertests.moc"
// clang-format on
