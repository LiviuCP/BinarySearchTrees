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
    void testConversionToInteger();
    void testComparison();

private:
    // for testing the conversion to integer
    class TestStringifiedInteger : public StringifiedInteger
    {
    public:
        TestStringifiedInteger() = default;

        TestStringifiedInteger(const std::string& value)
            : StringifiedInteger{value}
        {
        }

        using StringifiedInteger::_getIntValue;
    };
};

void StringifiedIntegerTests::testEmptyStringifiedInteger()
{
    StringifiedInteger stringifiedInt;
    QVERIFY(stringifiedInt.getValue() == "Z");
}

void StringifiedIntegerTests::testInitializationByString()
{
    QVERIFY(StringifiedInteger{"Z"}.getValue() == "Z");
    QVERIFY(StringifiedInteger{"Z_"}.getValue() == "Z");
    QVERIFY(StringifiedInteger{"ZZZZZZZZ"}.getValue() == "Z");
    QVERIFY(StringifiedInteger{"ZZZZZZZZZZZZZZZZ"}.getValue() == "Z");
    QVERIFY(StringifiedInteger{"ZZZZZZZZZZZZZZZZ_"}.getValue() == "Z");
    QVERIFY(StringifiedInteger{"ZZZZZZZZZZZZZZZZA"}.getValue() == "A");
    QVERIFY(StringifiedInteger{"ZZZZZZZZZZZZZZZZA_"}.getValue() == "A_");
    QVERIFY(StringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZ"}.getValue() == "AZZZZZZZZZ");
    QVERIFY(StringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZ_"}.getValue() == "AZZZZZZZZZ_");
    QVERIFY(StringifiedInteger{"AZZZZZZZZZ"}.getValue() == "AZZZZZZZZZ");
    QVERIFY(StringifiedInteger{"AZZZZZZZZZ_"}.getValue() == "AZZZZZZZZZ_");
    QVERIFY(StringifiedInteger{"ABCDEFGHIZ"}.getValue() == "ABCDEFGHIZ");
    QVERIFY(StringifiedInteger{"ABCDEFGHIZ_"}.getValue() == "ABCDEFGHIZ_");
    QVERIFY(StringifiedInteger{"ZABCDEFGHI"}.getValue() == "ABCDEFGHI");
    QVERIFY(StringifiedInteger{"ZABCDEFGHI_"}.getValue() == "ABCDEFGHI_");
    QVERIFY(StringifiedInteger{"ABCDEFGHI_"}.getValue() == "ABCDEFGHI_");

    QVERIFY(StringifiedInteger{""}.getValue() == "N");
    QVERIFY(StringifiedInteger{"_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"N"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"N_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"_N"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"_N_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"AZZZZZZZZZZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"AZZZZZZZZZZ_"}.getValue() == "N_");

    QVERIFY(StringifiedInteger{"ABCDE GHIZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ABCDE GHIZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{" BCDEFGHIZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{" BCDEFGHIZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"ABCDEFGHI "}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ABCDEFGHI _"}.getValue() == "N_");

    QVERIFY(StringifiedInteger{"ABCDEJGHIZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ABCDEJGHIZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"JBCDEFGHIZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"JBCDEFGHIZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"ABCDEFGHIJ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ABCDEFGHIJ_"}.getValue() == "N_");

    QVERIFY(StringifiedInteger{"ABCDENGHIZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ABCDENGHIZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"NBCDEFGHIZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"NBCDEFGHIZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"ABCDEFGHIN"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ABCDEFGHIN_"}.getValue() == "N_");

    QVERIFY(StringifiedInteger{"ABCDE_GHIZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ABCDE_GHIZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"_BCDEFGHIZ"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"_BCDEFGHIZ_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"ABCDEFGHI__"}.getValue() == "N_");

    QVERIFY(StringifiedInteger{"ABCDEFGHI_Z"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ABCDEFGHI_Z_"}.getValue() == "N_");
    QVERIFY(StringifiedInteger{"ZBCDEFGHI_A"}.getValue() == "N");
    QVERIFY(StringifiedInteger{"ZBCDEFGHI_A_"}.getValue() == "N_");
}

void StringifiedIntegerTests::testConversionToInteger()
{
    QVERIFY(TestStringifiedInteger{"ABCDZZ_"}._getIntValue() == -123400);
    QVERIFY(TestStringifiedInteger{"Z"}._getIntValue() == 0);
    QVERIFY(TestStringifiedInteger{"DCABZZ"}._getIntValue() == 431200);
    QVERIFY(!TestStringifiedInteger{"N_"}._getIntValue().has_value());
    QVERIFY(!TestStringifiedInteger{"N"}._getIntValue().has_value());
    QVERIFY(TestStringifiedInteger{}._getIntValue() == 0);
}

void StringifiedIntegerTests::testComparison()
{
    QVERIFY(StringifiedInteger{"N_"} <=> StringifiedInteger{"N_"} == std::strong_ordering::equal);
    QVERIFY(StringifiedInteger{"N_"} <=> StringifiedInteger{"ABZ"} == std::strong_ordering::less);
    QVERIFY(StringifiedInteger{"ABZ"} <=> StringifiedInteger{"ABA"} == std::strong_ordering::less);
    QVERIFY(StringifiedInteger{"ABA"} <=> StringifiedInteger{"N"} == std::strong_ordering::less);
    QVERIFY(StringifiedInteger{"N"} <=> StringifiedInteger{"N"} == std::strong_ordering::equal);
    QVERIFY(StringifiedInteger{"ABZ"} <=> StringifiedInteger{"N_"} == std::strong_ordering::greater);
    QVERIFY(StringifiedInteger{"ABA"} <=> StringifiedInteger{"ABZ"} == std::strong_ordering::greater);
    QVERIFY(StringifiedInteger{"N"} <=> StringifiedInteger{"ABA"} == std::strong_ordering::greater);

    QVERIFY(StringifiedInteger{"N_"} == StringifiedInteger{"N_"});
    QVERIFY(StringifiedInteger{"N_"} != StringifiedInteger{"ABZ"});
    QVERIFY(StringifiedInteger{"ABZ"} == StringifiedInteger{"ABZ"});
    QVERIFY(StringifiedInteger{"ABZ"} != StringifiedInteger{"ABA"});
    QVERIFY(StringifiedInteger{"ABA"} != StringifiedInteger{"N"});
    QVERIFY(StringifiedInteger{"N"} == StringifiedInteger{"N"});
}

QTEST_APPLESS_MAIN(StringifiedIntegerTests)

#include "tst_stringifiedintegertests.moc"
// clang-format on
