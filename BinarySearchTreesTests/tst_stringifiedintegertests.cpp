// clang-format off
#include <QTest>

#include "stringifiedinteger.h"

using namespace TestUtils;

class StringifiedIntegerTests : public QObject
{
    Q_OBJECT

private slots:
    void testStringifiedIntInitialization();
    void testAssignmentOperator();
    void testConversionToInteger();
    void testEquivalenceOperator();
    void testEqualityOperator();

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
        using StringifiedInteger::operator=;
    };
};

void StringifiedIntegerTests::testStringifiedIntInitialization()
{
    QVERIFY(TestStringifiedInteger{}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"Z"}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"Z_"}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZ"}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZ"}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZ_"}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"zzzz"}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"zzzz_"}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"ZzZz"}.getValue() == "Z");
    QVERIFY(TestStringifiedInteger{"zZzZ_"}.getValue() == "Z");

    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZA"}.getValue() == "A");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZA_"}.getValue() == "A_");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZ"}.getValue() == "AZZZZZZZZZ");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZ_"}.getValue() == "AZZZZZZZZZ_");
    QVERIFY(TestStringifiedInteger{"AZZZZZZZZZ"}.getValue() == "AZZZZZZZZZ");
    QVERIFY(TestStringifiedInteger{"AZZZZZZZZZ_"}.getValue() == "AZZZZZZZZZ_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIZ"}.getValue() == "ABCDEFGHIZ");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIZ_"}.getValue() == "ABCDEFGHIZ_");
    QVERIFY(TestStringifiedInteger{"ZABCDEFGHI"}.getValue() == "ABCDEFGHI");
    QVERIFY(TestStringifiedInteger{"ZABCDEFGHI_"}.getValue() == "ABCDEFGHI_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI_"}.getValue() == "ABCDEFGHI_");
    QVERIFY(TestStringifiedInteger{"abcdefghiz"}.getValue() == "ABCDEFGHIZ");
    QVERIFY(TestStringifiedInteger{"abcdefghiz_"}.getValue() == "ABCDEFGHIZ_");
    QVERIFY(TestStringifiedInteger{"AbCdEfGhIz"}.getValue() == "ABCDEFGHIZ");
    QVERIFY(TestStringifiedInteger{"aBcDeFgHiZ_"}.getValue() == "ABCDEFGHIZ_");
    QVERIFY(TestStringifiedInteger{"A"}.getValue() == "A");
    QVERIFY(TestStringifiedInteger{"A_"}.getValue() == "A_");
    QVERIFY(TestStringifiedInteger{"B"}.getValue() == "B");
    QVERIFY(TestStringifiedInteger{"B_"}.getValue() == "B_");
    QVERIFY(TestStringifiedInteger{"C"}.getValue() == "C");
    QVERIFY(TestStringifiedInteger{"C_"}.getValue() == "C_");
    QVERIFY(TestStringifiedInteger{"D"}.getValue() == "D");
    QVERIFY(TestStringifiedInteger{"D_"}.getValue() == "D_");
    QVERIFY(TestStringifiedInteger{"E"}.getValue() == "E");
    QVERIFY(TestStringifiedInteger{"E_"}.getValue() == "E_");
    QVERIFY(TestStringifiedInteger{"F"}.getValue() == "F");
    QVERIFY(TestStringifiedInteger{"F_"}.getValue() == "F_");
    QVERIFY(TestStringifiedInteger{"G"}.getValue() == "G");
    QVERIFY(TestStringifiedInteger{"G_"}.getValue() == "G_");
    QVERIFY(TestStringifiedInteger{"H"}.getValue() == "H");
    QVERIFY(TestStringifiedInteger{"H_"}.getValue() == "H_");
    QVERIFY(TestStringifiedInteger{"I"}.getValue() == "I");
    QVERIFY(TestStringifiedInteger{"I_"}.getValue() == "I_");

    QVERIFY(TestStringifiedInteger{""}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"N"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"N_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"_N"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"_N_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"AZZZZZZZZZZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"AZZZZZZZZZZ_"}.getValue() == "N_");

    QVERIFY(TestStringifiedInteger{"ABCDE GHIZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDE GHIZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{" BCDEFGHIZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{" BCDEFGHIZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI "}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI _"}.getValue() == "N_");

    QVERIFY(TestStringifiedInteger{"ABCDEJGHIZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEJGHIZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"JBCDEFGHIZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"JBCDEFGHIZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIJ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIJ_"}.getValue() == "N_");

    QVERIFY(TestStringifiedInteger{"ABCDENGHIZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDENGHIZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"NBCDEFGHIZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"NBCDEFGHIZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIN"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIN_"}.getValue() == "N_");

    QVERIFY(TestStringifiedInteger{"ABCDE_GHIZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDE_GHIZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"_BCDEFGHIZ"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"_BCDEFGHIZ_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI__"}.getValue() == "N_");

    QVERIFY(TestStringifiedInteger{"ABCDEFGHI_Z"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI_Z_"}.getValue() == "N_");
    QVERIFY(TestStringifiedInteger{"ZBCDEFGHI_A"}.getValue() == "N");
    QVERIFY(TestStringifiedInteger{"ZBCDEFGHI_A_"}.getValue() == "N_");
}

void StringifiedIntegerTests::testAssignmentOperator()
{
    TestStringifiedInteger stringifiedInt;

    stringifiedInt = "ZABC_";
    QVERIFY(stringifiedInt.getValue() == "ABC_");

    stringifiedInt = "ABCZ";
    QVERIFY(stringifiedInt.getValue() == "ABCZ");

    stringifiedInt = "ABC _";
    QVERIFY(stringifiedInt.getValue() == "N_");

    stringifiedInt = "JABC";
    QVERIFY(stringifiedInt.getValue() == "N");

    stringifiedInt = "";
    QVERIFY(stringifiedInt.getValue() == "N");

    // special case, a nullptr const char* passed to stringified integer (this might cause a crash if not handled properly)
    stringifiedInt = nullptr;
    QVERIFY(stringifiedInt.getValue() == "N");
}

void StringifiedIntegerTests::testConversionToInteger()
{
    QVERIFY(TestStringifiedInteger{"ABCDZZ_"}._getIntValue() == -123400);
    QVERIFY(TestStringifiedInteger{"DCABZZ"}._getIntValue() == 431200);

    QVERIFY(TestStringifiedInteger{"I_"}._getIntValue() == -9);
    QVERIFY(TestStringifiedInteger{"H_"}._getIntValue() == -8);
    QVERIFY(TestStringifiedInteger{"G_"}._getIntValue() == -7);
    QVERIFY(TestStringifiedInteger{"F_"}._getIntValue() == -6);
    QVERIFY(TestStringifiedInteger{"E_"}._getIntValue() == -5);
    QVERIFY(TestStringifiedInteger{"D_"}._getIntValue() == -4);
    QVERIFY(TestStringifiedInteger{"C_"}._getIntValue() == -3);
    QVERIFY(TestStringifiedInteger{"B_"}._getIntValue() == -2);
    QVERIFY(TestStringifiedInteger{"A_"}._getIntValue() == -1);
    QVERIFY(TestStringifiedInteger{"Z"}._getIntValue() == 0);
    QVERIFY(TestStringifiedInteger{}._getIntValue() == 0);
    QVERIFY(TestStringifiedInteger{"A"}._getIntValue() == 1);
    QVERIFY(TestStringifiedInteger{"B"}._getIntValue() == 2);
    QVERIFY(TestStringifiedInteger{"C"}._getIntValue() == 3);
    QVERIFY(TestStringifiedInteger{"D"}._getIntValue() == 4);
    QVERIFY(TestStringifiedInteger{"E"}._getIntValue() == 5);
    QVERIFY(TestStringifiedInteger{"F"}._getIntValue() == 6);
    QVERIFY(TestStringifiedInteger{"G"}._getIntValue() == 7);
    QVERIFY(TestStringifiedInteger{"H"}._getIntValue() == 8);
    QVERIFY(TestStringifiedInteger{"I"}._getIntValue() == 9);

    QVERIFY(!TestStringifiedInteger{"N_"}._getIntValue().has_value());
    QVERIFY(!TestStringifiedInteger{"N"}._getIntValue().has_value());
}

void StringifiedIntegerTests::testEquivalenceOperator()
{
    QVERIFY(TestStringifiedInteger{"N_"} <=> TestStringifiedInteger{"N_"} == std::strong_ordering::equal);
    QVERIFY(TestStringifiedInteger{"N_"} <=> TestStringifiedInteger{"ABZ"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"ABZ"} <=> TestStringifiedInteger{"ABA"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"ABA"} <=> TestStringifiedInteger{"N"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"N"} <=> TestStringifiedInteger{"N"} == std::strong_ordering::equal);
    QVERIFY(TestStringifiedInteger{"ABZ"} <=> TestStringifiedInteger{"N_"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"ABA"} <=> TestStringifiedInteger{"ABZ"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"N"} <=> TestStringifiedInteger{"ABA"} == std::strong_ordering::greater);
}

void StringifiedIntegerTests::testEqualityOperator()
{
    QVERIFY(TestStringifiedInteger{"ABZ"} == TestStringifiedInteger{"abz"});
    QVERIFY(TestStringifiedInteger{"ZAB"} == TestStringifiedInteger{"ab"});
    QVERIFY(TestStringifiedInteger{"ZZACB_"} == TestStringifiedInteger{"acb_"});
    QVERIFY(TestStringifiedInteger{"ZACB"} == TestStringifiedInteger{"zacb"});

    QVERIFY(TestStringifiedInteger{"a_"} == TestStringifiedInteger{"A_"});
    QVERIFY(TestStringifiedInteger{"b_"} == TestStringifiedInteger{"B_"});
    QVERIFY(TestStringifiedInteger{"c_"} == TestStringifiedInteger{"C_"});
    QVERIFY(TestStringifiedInteger{"d_"} == TestStringifiedInteger{"D_"});
    QVERIFY(TestStringifiedInteger{"e_"} == TestStringifiedInteger{"E_"});
    QVERIFY(TestStringifiedInteger{"f_"} == TestStringifiedInteger{"F_"});
    QVERIFY(TestStringifiedInteger{"g_"} == TestStringifiedInteger{"G_"});
    QVERIFY(TestStringifiedInteger{"h_"} == TestStringifiedInteger{"H_"});
    QVERIFY(TestStringifiedInteger{"i_"} == TestStringifiedInteger{"I_"});
    QVERIFY(TestStringifiedInteger{"z_"} == TestStringifiedInteger{"Z_"});
    QVERIFY(TestStringifiedInteger{"n_"} == TestStringifiedInteger{"N_"});
    QVERIFY(TestStringifiedInteger{"A"} == TestStringifiedInteger{"a"});
    QVERIFY(TestStringifiedInteger{"B"} == TestStringifiedInteger{"b"});
    QVERIFY(TestStringifiedInteger{"C"} == TestStringifiedInteger{"c"});
    QVERIFY(TestStringifiedInteger{"D"} == TestStringifiedInteger{"d"});
    QVERIFY(TestStringifiedInteger{"E"} == TestStringifiedInteger{"e"});
    QVERIFY(TestStringifiedInteger{"F"} == TestStringifiedInteger{"f"});
    QVERIFY(TestStringifiedInteger{"G"} == TestStringifiedInteger{"g"});
    QVERIFY(TestStringifiedInteger{"H"} == TestStringifiedInteger{"h"});
    QVERIFY(TestStringifiedInteger{"I"} == TestStringifiedInteger{"i"});
    QVERIFY(TestStringifiedInteger{"Z"} == TestStringifiedInteger{"z"});
    QVERIFY(TestStringifiedInteger{"N"} == TestStringifiedInteger{"n"});
}

QTEST_APPLESS_MAIN(StringifiedIntegerTests)

#include "tst_stringifiedintegertests.moc"
// clang-format on
