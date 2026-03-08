// clang-format off
#include <QTest>

#include "stringifiedinteger.h"

using namespace TestUtils;

class StringifiedIntegerTests : public QObject
{
    Q_OBJECT

private slots:
    void testConstructors();
    void testConversionToInteger();
    void testEquivalenceOperator();
    void testEqualityOperator();

private:
    // for testing the conversion to integer
    class TestStringifiedInteger : public StringifiedInteger
    {
    public:
        using StringifiedInteger::StringifiedInteger;
        using StringifiedInteger::_convertToInteger;
    };
};

void StringifiedIntegerTests::testConstructors()
{
    QVERIFY(TestStringifiedInteger{}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"Z"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"Z_"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZ"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZ"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZ_"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"zzzz"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"zzzz_"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"ZzZz"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"zZzZ_"}.str() == "Z");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZA"}.str() == "A");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZA_"}.str() == "A_");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZ"}.str() == "AZZZZZZZZZ");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZ_"}.str() == "AZZZZZZZZZ_");
    QVERIFY(TestStringifiedInteger{"AZZZZZZZZZ"}.str() == "AZZZZZZZZZ");
    QVERIFY(TestStringifiedInteger{"AZZZZZZZZZ_"}.str() == "AZZZZZZZZZ_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIZ"}.str() == "ABCDEFGHIZ");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIZ_"}.str() == "ABCDEFGHIZ_");
    QVERIFY(TestStringifiedInteger{"ZABCDEFGHI"}.str() == "ABCDEFGHI");
    QVERIFY(TestStringifiedInteger{"ZABCDEFGHI_"}.str() == "ABCDEFGHI_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI_"}.str() == "ABCDEFGHI_");
    QVERIFY(TestStringifiedInteger{"abcdefghiz"}.str() == "ABCDEFGHIZ");
    QVERIFY(TestStringifiedInteger{"abcdefghiz_"}.str() == "ABCDEFGHIZ_");
    QVERIFY(TestStringifiedInteger{"AbCdEfGhIz"}.str() == "ABCDEFGHIZ");
    QVERIFY(TestStringifiedInteger{"aBcDeFgHiZ_"}.str() == "ABCDEFGHIZ_");
    QVERIFY(TestStringifiedInteger{"A"}.str() == "A");
    QVERIFY(TestStringifiedInteger{"A_"}.str() == "A_");
    QVERIFY(TestStringifiedInteger{"B"}.str() == "B");
    QVERIFY(TestStringifiedInteger{"B_"}.str() == "B_");
    QVERIFY(TestStringifiedInteger{"C"}.str() == "C");
    QVERIFY(TestStringifiedInteger{"C_"}.str() == "C_");
    QVERIFY(TestStringifiedInteger{"D"}.str() == "D");
    QVERIFY(TestStringifiedInteger{"D_"}.str() == "D_");
    QVERIFY(TestStringifiedInteger{"E"}.str() == "E");
    QVERIFY(TestStringifiedInteger{"E_"}.str() == "E_");
    QVERIFY(TestStringifiedInteger{"F"}.str() == "F");
    QVERIFY(TestStringifiedInteger{"F_"}.str() == "F_");
    QVERIFY(TestStringifiedInteger{"G"}.str() == "G");
    QVERIFY(TestStringifiedInteger{"G_"}.str() == "G_");
    QVERIFY(TestStringifiedInteger{"H"}.str() == "H");
    QVERIFY(TestStringifiedInteger{"H_"}.str() == "H_");
    QVERIFY(TestStringifiedInteger{"I"}.str() == "I");
    QVERIFY(TestStringifiedInteger{"I_"}.str() == "I_");

    // min 32bit integer value: -2147483648 => PASS
    QVERIFY(TestStringifiedInteger{"BADGDHCFDH_"}.str() == "BADGDHCFDH_");

    // -2147483649: underflow => FAIL (-infinite)
    QVERIFY(TestStringifiedInteger{"BADGDHCFDI_"}.str() == "N_");

    // max 32bit integer value: 2147483647 => PASS
    QVERIFY(TestStringifiedInteger{"BADGDHCFDG"}.str() == "BADGDHCFDG");

     // 2147483648: overflow => FAIL (+infinite)
    QVERIFY(TestStringifiedInteger{"BADGDHCFDH"}.str() == "N");

    QVERIFY(TestStringifiedInteger{""}.str() == "N");
    QVERIFY(TestStringifiedInteger{"_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"N"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"N_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"_N"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"_N_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ZZZZZZZZZZZZZZZZAZZZZZZZZZZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"AZZZZZZZZZZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"AZZZZZZZZZZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDE GHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDE GHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{" BCDEFGHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{" BCDEFGHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI "}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI _"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDE0GHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDE1GHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"2BCDEFGHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"3BCDEFGHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI4"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI5_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEJGHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEJGHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"JBCDEFGHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"JBCDEFGHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIJ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIJ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDENGHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDENGHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"NBCDEFGHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"NBCDEFGHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIN"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHIN_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDE_GHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDE_GHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"_BCDEFGHIZ"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"_BCDEFGHIZ_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI__"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI_Z"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ABCDEFGHI_Z_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"ZBCDEFGHI_A"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"ZBCDEFGHI_A_"}.str() == "N_");

    // special case, a nullptr const char* passed to stringified integer (this might cause a crash if not handled properly)
    QVERIFY(TestStringifiedInteger{nullptr}.str() == "N");

    // test with std::string too
    QVERIFY(TestStringifiedInteger{std::string{"BCE_"}}.str() == "BCE_");
    QVERIFY(TestStringifiedInteger{std::string{"ZBCE"}}.str() == "BCE");
    QVERIFY(TestStringifiedInteger{std::string{"BJE_"}}.str() == "N_");
    QVERIFY(TestStringifiedInteger{std::string{"BJ_E"}}.str() == "N");
}

void StringifiedIntegerTests::testConversionToInteger()
{
    QVERIFY(TestStringifiedInteger{"ABCDZZ_"}._convertToInteger() == -123400);
    QVERIFY(TestStringifiedInteger{"DCABZZ"}._convertToInteger() == 431200);
    QVERIFY(TestStringifiedInteger{"I_"}._convertToInteger() == -9);
    QVERIFY(TestStringifiedInteger{"H_"}._convertToInteger() == -8);
    QVERIFY(TestStringifiedInteger{"G_"}._convertToInteger() == -7);
    QVERIFY(TestStringifiedInteger{"F_"}._convertToInteger() == -6);
    QVERIFY(TestStringifiedInteger{"E_"}._convertToInteger() == -5);
    QVERIFY(TestStringifiedInteger{"D_"}._convertToInteger() == -4);
    QVERIFY(TestStringifiedInteger{"C_"}._convertToInteger() == -3);
    QVERIFY(TestStringifiedInteger{"B_"}._convertToInteger() == -2);
    QVERIFY(TestStringifiedInteger{"A_"}._convertToInteger() == -1);
    QVERIFY(TestStringifiedInteger{"Z"}._convertToInteger() == 0);
    QVERIFY(TestStringifiedInteger{}._convertToInteger() == 0);
    QVERIFY(TestStringifiedInteger{"A"}._convertToInteger() == 1);
    QVERIFY(TestStringifiedInteger{"B"}._convertToInteger() == 2);
    QVERIFY(TestStringifiedInteger{"C"}._convertToInteger() == 3);
    QVERIFY(TestStringifiedInteger{"D"}._convertToInteger() == 4);
    QVERIFY(TestStringifiedInteger{"E"}._convertToInteger() == 5);
    QVERIFY(TestStringifiedInteger{"F"}._convertToInteger() == 6);
    QVERIFY(TestStringifiedInteger{"G"}._convertToInteger() == 7);
    QVERIFY(TestStringifiedInteger{"H"}._convertToInteger() == 8);
    QVERIFY(TestStringifiedInteger{"I"}._convertToInteger() == 9);

    // min 32bit integer value: -2147483648 => PASS
    QVERIFY(TestStringifiedInteger{"BADGDHCFDH_"}._convertToInteger() == -2147483648);

    // -2147483649: underflow => FAIL (-infinite)
    QVERIFY(!TestStringifiedInteger{"BADGDHCFDI_"}._convertToInteger().has_value());

    // max 32bit integer value: 2147483647 => PASS
    QVERIFY(TestStringifiedInteger{"BADGDHCFDG"}._convertToInteger() == 2147483647);

    // 2147483648: overflow => FAIL (+infinite)
    QVERIFY(!TestStringifiedInteger{"BADGDHCFDH"}._convertToInteger().has_value());

    QVERIFY(TestStringifiedInteger{"-2147483648"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"2147483648_"}.str() == "N_");
    QVERIFY(TestStringifiedInteger{"2147483647"}.str() == "N");
    QVERIFY(TestStringifiedInteger{"2147483647_"}.str() == "N_");

    QVERIFY(!TestStringifiedInteger{"N_"}._convertToInteger().has_value());
    QVERIFY(!TestStringifiedInteger{"N"}._convertToInteger().has_value());
}

void StringifiedIntegerTests::testEquivalenceOperator()
{
    QVERIFY(TestStringifiedInteger{"N_"} <=> TestStringifiedInteger{"n_"} == std::strong_ordering::equal);
    QVERIFY(TestStringifiedInteger{"N_"} <=> TestStringifiedInteger{"ABZ"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"ABZ"} <=> TestStringifiedInteger{"ABA"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"ABA"} <=> TestStringifiedInteger{"aba"} == std::strong_ordering::equal);
    QVERIFY(TestStringifiedInteger{"ABA"} <=> TestStringifiedInteger{"N"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"N"} <=> TestStringifiedInteger{"n"} == std::strong_ordering::equal);
    QVERIFY(TestStringifiedInteger{"aBZ"} <=> TestStringifiedInteger{"n_"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"ABA"} <=> TestStringifiedInteger{"ABz"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"N"} <=> TestStringifiedInteger{"aBa"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"I_"} <=> TestStringifiedInteger{"H_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"H_"} <=> TestStringifiedInteger{"G_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"G_"} <=> TestStringifiedInteger{"F_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"F_"} <=> TestStringifiedInteger{"E_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"E_"} <=> TestStringifiedInteger{"D_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"D_"} <=> TestStringifiedInteger{"C_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"C_"} <=> TestStringifiedInteger{"B_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"B_"} <=> TestStringifiedInteger{"A_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"A_"} <=> TestStringifiedInteger{"Z_"} == std::strong_ordering::less);
    QVERIFY(TestStringifiedInteger{"Z_"} <=> TestStringifiedInteger{"Z"} == std::strong_ordering::equal);
    QVERIFY(TestStringifiedInteger{"A"} <=> TestStringifiedInteger{"Z"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"B"} <=> TestStringifiedInteger{"A"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"C"} <=> TestStringifiedInteger{"B"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"D"} <=> TestStringifiedInteger{"C"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"E"} <=> TestStringifiedInteger{"D"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"F"} <=> TestStringifiedInteger{"E"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"G"} <=> TestStringifiedInteger{"F"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"H"} <=> TestStringifiedInteger{"G"} == std::strong_ordering::greater);
    QVERIFY(TestStringifiedInteger{"I"} <=> TestStringifiedInteger{"H"} == std::strong_ordering::greater);
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
