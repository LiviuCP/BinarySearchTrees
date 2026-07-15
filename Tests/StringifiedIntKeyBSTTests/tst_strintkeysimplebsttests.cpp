// clang-format off
#include <QTest>
#include <algorithm>

#include "testutils.h"
#include "stringifiedinteger.h"

import binarysearchtree;

using namespace TestUtils;
using StrIntStrBinarySearchTree = BinarySearchTree<StringifiedInteger, std::string>;
using StrIntStrBSTIterator = StrIntStrBinarySearchTree::InOrderForwardIterator;
using upStrIntStrBinarySearchTree = std::unique_ptr<StrIntStrBinarySearchTree>;

class StrIntKeySimpleBSTTests : public QObject
{
    Q_OBJECT

public:
    StrIntKeySimpleBSTTests();

private slots:
    void init();
    void cleanup();

    void testAddNodes();
    void testRemoveNodes();
    void testUpdateNodeValue();
    void testMoveSemantics();
    void testMergeTrees();
    void testInOrderForwardIterators();

    void testPrintTree(); // only required for improving code coverage

private:
    upStrIntStrBinarySearchTree mpSearchTree;
    upStrIntStrBinarySearchTree mpAuxSearchTree;
};

StrIntKeySimpleBSTTests::StrIntKeySimpleBSTTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

void StrIntKeySimpleBSTTests::init()
{
    QVERIFY(!mpSearchTree && !mpAuxSearchTree);
}

void StrIntKeySimpleBSTTests::cleanup()
{
    mpSearchTree.reset();
    mpAuxSearchTree.reset();
}

void StrIntKeySimpleBSTTests::testAddNodes()
{
    bool nodeInserted{false};
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeInserted = mpSearchTree->tryInsertNode("E_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT", 1));

    nodeInserted = mpSearchTree->tryInsertNode("H", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R", 2));

    nodeInserted = mpSearchTree->tryInsertNode("A_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL", 3));

    nodeInserted = mpSearchTree->tryInsertNode("B", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL/B:A_R", 4));

    nodeInserted = mpSearchTree->tryInsertNode("B_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL/B_:A_/B:A_", 5));

    nodeInserted = mpSearchTree->tryInsertNode("G", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL/B_:A_/B:A_/G:BR", 6));

    nodeInserted = mpSearchTree->tryInsertNode("Z", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL/B_:A_/B:A_/Z:B/G:B", 7));

    nodeInserted = mpSearchTree->tryInsertNode("I_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/A_:HL/B_:A_/B:A_/Z:B/G:B", 8));

    nodeInserted = mpSearchTree->tryInsertNode("AF", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/A_:H/AF:H/B_:A_/B:A_/Z:B/G:B", 9));

    nodeInserted = mpSearchTree->tryInsertNode("AD", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/A_:H/AF:H/B_:A_/B:A_/AD:AFL/Z:B/G:B", 10));

    nodeInserted = mpSearchTree->tryInsertNode("BC_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/B_:A_/B:A_/AD:AFL/Z:B/G:B", 11));

    nodeInserted = mpSearchTree->tryInsertNode("AG", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B", 12));

    nodeInserted = mpSearchTree->tryInsertNode("AF_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B", 13));

    nodeInserted = mpSearchTree->tryInsertNode("AB_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/AB_:AF_R/Z:B/G:B", 14));

    nodeInserted = mpSearchTree->tryInsertNode("AI", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/AB_:AF_R/Z:B/G:B/AI:AGR", 15));

    nodeInserted = mpSearchTree->tryInsertNode("AE_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/AB_:AF_R/Z:B/G:B/AI:AGR/AE_:AB_L", 16));

    // inserting nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    nodeInserted = mpSearchTree->tryInsertNode("BE", scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            *mpAuxSearchTree == *mpSearchTree);

    nodeInserted = mpAuxSearchTree->tryInsertNode("BE_", scCustomNullValue);
    QVERIFY(!nodeInserted &&
            *mpAuxSearchTree == *mpSearchTree);

    nodeInserted = mpSearchTree->tryInsertNode("BE", scDefaultValue);
    QVERIFY(nodeInserted &&
            *mpAuxSearchTree != *mpSearchTree);

    nodeInserted = mpAuxSearchTree->tryInsertNode("BE", scDefaultValue);
    QVERIFY(nodeInserted &&
            *mpAuxSearchTree == *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/AB_:AF_R/Z:B/G:B/AI:AGR/AE_:AB_L/BE:AIR", 17));

    // some additional (corner) cases
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"BC_", "AF_", "AE_", "AB_", "I_", "E_", "B_", "A_", "Z", "B", "G", "H", "AD", "AF", "AG", "AI"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "BC_:ROOT/AF_:BC_R/AE_:AF_R/AB_:AE_R/I_:AB_R/E_:I_R/B_:E_R/A_:B_R/Z:A_R/B:ZR/G:BR/H:GR/AD:HR/AF:ADR/AG:AFR/AI:AGR", 16));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AI", "AG", "AF", "AD", "H", "G", "B", "Z", "A_", "B_", "E_", "I_", "AB_", "AE_", "AF_", "BC_"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AI:ROOT/AG:AIL/AF:AGL/AD:AFL/H:ADL/G:HL/B:GL/Z:BL/A_:ZL/B_:A_L/E_:B_L/I_:E_L/AB_:I_L/AE_:AB_L/AF_:AE_L/BC_:AF_L", 16));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"BC_", "AI", "AF_", "AG", "AE_", "AF", "AB_", "AD", "I_", "H", "E_", "G", "B_", "B", "A_", "Z"}, scDefaultValue, scCustomNullValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "BC_:ROOT/AI:BC_R/AF_:AIL/AG:AF_R/AE_:AGL/AF:AE_R/AB_:AFL/AD:AB_R/I_:ADL/H:I_R/E_:HL/G:E_R/B_:GL/B:B_R/A_:BL/Z:A_R", 16));

    mpSearchTree->clear();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "B", "D", "Z", "A"}, scDefaultValue, scDefaultNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "C_", "B", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "A", "Z"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"C_", "E_", "B", "D", "Z", "A"}, scDefaultValue, scDefaultNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"C_", "B", "E_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{}, scDefaultValue, scDefaultNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    // small test including the "extreme" key values: N_ and N
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "N_", "AG", "AF_", "AB_", "N", "AE_"}, scDefaultValue);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/N_:I_L/A_:H/AF:H/AF_:N_R/B_:A_/B:A_/AD:AF/AG:AF/AB_:AF_R/Z:B/G:B/N:AGR/AE_:AB_L", 16));
}

void StrIntKeySimpleBSTTests::testRemoveNodes()
{
    bool nodeRemoved{false};
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(*mpSearchTree);

    nodeRemoved = mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AB_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B", 13));

    nodeRemoved = mpSearchTree->tryRemoveNode("H");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/AD:E_/BC_:I_L/A_:AD/AF:AD/AB_:BC_R/B_:A_/B:A_/AG:AFR/Z:B/G:B", 12));

    nodeRemoved = mpSearchTree->tryRemoveNode("Z");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/AD:E_/BC_:I_L/A_:AD/AF:AD/AB_:BC_R/B_:A_/B:A_/AG:AFR/G:BR", 11));

    nodeRemoved = mpSearchTree->tryRemoveNode("BC_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/AD:E_/AB_:I_L/A_:AD/AF:AD/B_:A_/B:A_/AG:AFR/G:BR", 10));

    nodeRemoved = mpSearchTree->tryRemoveNode("AG");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/AD:E_/AB_:I_L/A_:AD/AF:AD/B_:A_/B:A_/G:BR", 9));

    nodeRemoved = mpSearchTree->tryRemoveNode("E_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/I_:B_/AD:B_/AB_:I_L/A_:AD/AF:AD/B:A_R/G:BR", 8));

    nodeRemoved = mpSearchTree->tryRemoveNode("I_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/AB_:B_/AD:B_/A_:AD/AF:AD/B:A_R/G:BR", 7));

    nodeRemoved = mpSearchTree->tryRemoveNode("AD");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/AB_:B_/AF:B_/A_:AFL/B:A_R/G:BR", 6));

    nodeRemoved = mpSearchTree->tryRemoveNode("A_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/AB_:B_/AF:B_/B:AFL/G:BR", 5));

    nodeRemoved = mpSearchTree->tryRemoveNode("B_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/AB_:B/AF:B/G:AFL", 4));

    nodeRemoved = mpSearchTree->tryRemoveNode("AB_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/AF:BR/G:AFL", 3));

    nodeRemoved = mpSearchTree->tryRemoveNode("AF");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/G:BR", 2));

    nodeRemoved = mpSearchTree->tryRemoveNode("B");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT", 1));

    nodeRemoved = mpSearchTree->tryRemoveNode("G");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeRemoved = mpSearchTree->tryRemoveNode("G");
    QVERIFY(!nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeRemoved = mpSearchTree->tryRemoveNode("BE");
    QVERIFY(!nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // some additional (corner) cases
    nodeRemoved = mpAuxSearchTree->tryRemoveNode("I_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "E_:ROOT/BC_:E_/H:E_/AF_:BC_R/A_:H/AF:H/AB_:AF_R/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B", 13));

    nodeRemoved = mpAuxSearchTree->tryRemoveNode("A_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "E_:ROOT/BC_:E_/H:E_/AF_:BC_R/Z:H/AF:H/AB_:AF_R/B_:Z/B:Z/AD:AF/AG:AF/G:BR", 12));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "AB_"}, scDefaultValue);

    nodeRemoved = mpSearchTree->tryRemoveNode("B");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT", 1));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "AB_", "G"}, scDefaultValue);
    *mpAuxSearchTree = *mpSearchTree;

    nodeRemoved = mpSearchTree->tryRemoveNode("AB_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/G:BR", 2));

    nodeRemoved = mpSearchTree->tryRemoveNode("G");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT", 1));

    nodeRemoved = mpAuxSearchTree->tryRemoveNode("G");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "B:ROOT/AB_:BL", 2));

    nodeRemoved = mpAuxSearchTree->tryRemoveNode("AB_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "B:ROOT", 1));

    // removing null node from custom null value tree
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue, scCustomNullValue);

    nodeRemoved = mpSearchTree->tryRemoveNode("E_");
    QVERIFY(!nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/B_:A_/C:A_/B:C/D:C", 5));

    // removing same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue);

    nodeRemoved = mpSearchTree->tryRemoveNode("C");
    QVERIFY(nodeRemoved &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeRemoved = mpAuxSearchTree->tryRemoveNode("C");
    QVERIFY(nodeRemoved &&
            *mpSearchTree == *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/B_:A_/D:A_/B:DL", 4));
}

void StrIntKeySimpleBSTTests::testUpdateNodeValue()
{
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue("E_") &&
            scDefaultNullValue == mpSearchTree->getNodeValue("Z") &&
            scDefaultNullValue == mpSearchTree->getNodeValue("AF"));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());

    (void)mpSearchTree->tryInsertNode("E_", "a1");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("A_", "c3");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("B_", "e5");
    (void)mpSearchTree->tryInsertNode("G", "f6");
    (void)mpSearchTree->tryInsertNode("Z", "g7");
    (void)mpSearchTree->tryInsertNode("I_", "h8");
    (void)mpSearchTree->tryInsertNode("AF", "i9");
    (void)mpSearchTree->tryInsertNode("AD", "j10");
    (void)mpSearchTree->tryInsertNode("BC_", "k11");
    (void)mpSearchTree->tryInsertNode("AG", "l12");
    (void)mpSearchTree->tryInsertNode("AF_", "m13");
    (void)mpSearchTree->tryInsertNode("AB_", "n14");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/I_:h8:E_/H:b2:E_/BC_:k11:I_L/A_:c3:H/AF:i9:H/AF_:m13:BC_R/B_:e5:A_/B:d4:A_/AD:j10:AF/AG:l12:AF/AB_:n14:AF_R/Z:g7:B/G:f6:B", 14));

    QVERIFY("a1" == mpSearchTree->getNodeValue("E_") &&
            "g7" == mpSearchTree->getNodeValue("Z") &&
            "i9" == mpSearchTree->getNodeValue("AF"));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue("BE"));

    // update by inserting/modifying node
    bool nodeInserted{false};

    nodeInserted = mpSearchTree->tryInsertNode("B", "d4_1");
    QVERIFY(!nodeInserted &&
            "d4_1" == mpSearchTree->getNodeValue("B"));

    nodeInserted = mpSearchTree->tryInsertNode("I_", "b2");
    QVERIFY(!nodeInserted &&
            "b2" == mpSearchTree->getNodeValue("I_"));

    nodeInserted = mpSearchTree->tryInsertNode("AD", "j10");
    QVERIFY(!nodeInserted &&
            "j10" == mpSearchTree->getNodeValue("AD"));

    nodeInserted = mpSearchTree->tryInsertNode("AG", "L12");
    QVERIFY(!nodeInserted &&
            "L12" == mpSearchTree->getNodeValue("AG"));

    nodeInserted = mpSearchTree->tryInsertNode("AB_", scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            "n14" == mpSearchTree->getNodeValue("AB_"));

    nodeInserted = mpSearchTree->tryInsertNode("BE", scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue("BE"));

    nodeInserted = mpSearchTree->tryInsertNode("BE", "o15");
    QVERIFY(nodeInserted &&
            "o15" == mpSearchTree->getNodeValue("BE"));

    nodeInserted = mpSearchTree->tryInsertNode("BE", scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            "o15" == mpSearchTree->getNodeValue("BE"));

    // update by removing node
    bool nodeRemoved{false};

    nodeRemoved = mpSearchTree->tryRemoveNode("G");
    QVERIFY(nodeRemoved &&
            scDefaultNullValue == mpSearchTree->getNodeValue("G"));

    nodeRemoved = mpSearchTree->tryRemoveNode("BH_");
    QVERIFY(!nodeRemoved &&
            scDefaultNullValue == mpSearchTree->getNodeValue("BH_"));

    // remove and re-insert node with different value
    nodeRemoved = mpSearchTree->tryRemoveNode("AF");
    nodeInserted = mpSearchTree->tryInsertNode("AF", "i9_new");

    QVERIFY(nodeRemoved &&
            nodeInserted &&
            "i9_new" == mpSearchTree->getNodeValue("AF"));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/I_:b2:E_/H:b2:E_/BC_:k11:I_L/A_:c3:H/AG:L12:H/AF_:m13:BC_R/B_:e5:A_/B:d4_1:A_/AD:j10:AG/BE:o15:AG/AB_:n14:AF_R/Z:g7:BL/AF:i9_new:ADR", 14));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue("E_") &&
            scDefaultNullValue == mpSearchTree->getNodeValue("Z") &&
            scDefaultNullValue == mpSearchTree->getNodeValue("AF"));

    // test with same value for all nodes
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "E_:DF:ROOT/I_:DF:E_/H:DF:E_/BC_:DF:I_L/A_:DF:H/AF:DF:H/AF_:DF:BC_R/B_:DF:A_/B:DF:A_/AD:DF:AF/AG:DF:AF/AB_:DF:AF_R/Z:DF:B/G:DF:B", 14));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue("E_") &&
            scDefaultValue == mpAuxSearchTree->getNodeValue("Z") &&
            scDefaultValue == mpAuxSearchTree->getNodeValue("AF"));

    // test updating custom null value tree node values
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);

    (void)mpSearchTree->tryInsertNode("E_", "a1");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("A_", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("B_", "e5");
    (void)mpSearchTree->tryInsertNode("G", "f6");
    (void)mpSearchTree->tryInsertNode("H_", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("Z", "g7");
    (void)mpSearchTree->tryInsertNode("I_", "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/H_::E_/H:b2:E_/I_:h8:H_L/A_::HL/B_:e5:A_/B:d4:A_/Z:g7:B/G:f6:B", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());

    nodeInserted = mpSearchTree->tryInsertNode("B", "d4_1");
    QVERIFY(!nodeInserted &&
            "d4_1" == mpSearchTree->getNodeValue("B"));

    nodeInserted = mpSearchTree->tryInsertNode("A_", "c3");
    QVERIFY(!nodeInserted &&
            "c3" == mpSearchTree->getNodeValue("A_"));

    nodeInserted = mpSearchTree->tryInsertNode("G", scCustomNullValue);
    QVERIFY(!nodeInserted &&
            "f6" == mpSearchTree->getNodeValue("G"));

    nodeInserted = mpSearchTree->tryInsertNode("B_", scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue("B_"));

    nodeInserted = mpSearchTree->tryInsertNode("BE", scCustomNullValue);
    QVERIFY(!nodeInserted &&
            scCustomNullValue == mpSearchTree->getNodeValue("BE"));

    nodeInserted = mpSearchTree->tryInsertNode("G_", "i9");
    QVERIFY(nodeInserted &&
            "i9" == mpSearchTree->getNodeValue("G_"));

    nodeRemoved = mpSearchTree->tryRemoveNode("BE_");
    QVERIFY(!nodeRemoved &&
            scCustomNullValue == mpSearchTree->getNodeValue("BE_"));

    nodeRemoved = mpSearchTree->tryRemoveNode("H_");
    QVERIFY(nodeRemoved &&
            scCustomNullValue == mpSearchTree->getNodeValue("H_"));

    nodeRemoved = mpSearchTree->tryRemoveNode("B");
    QVERIFY(nodeRemoved &&
            scCustomNullValue == mpSearchTree->getNodeValue("B"));

    nodeRemoved = mpSearchTree->tryRemoveNode("A_");
    nodeInserted = mpSearchTree->tryInsertNode("A_", scDefaultNullValue);

    QVERIFY(nodeRemoved &&
            nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue("A_"));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/G_:i9:E_/H:b2:E_/I_:h8:G_L/Z:g7:HL/B_::Z/G:f6:Z/A_::B_R", 8));
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(*mpSearchTree);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    nodeInserted = mpAuxSearchTree->tryInsertNode("AG", "df");
    QVERIFY(nodeInserted &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    nodeInserted = mpSearchTree->tryInsertNode("G", scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue("G"));

    nodeInserted = mpAuxSearchTree->tryInsertNode("D", scCustomNullValue);
    QVERIFY(!nodeInserted &&
            scCustomNullValue == mpAuxSearchTree->getNodeValue("D"));

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AF:DF:ROOT/I_:DF:AF/AG:DF:AF/BC_:DF:I_/AD:DF:I_/AF_:DF:BC_R/G::ADL/AB_:DF:AF_R", 8));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeySimpleBSTTests::testMoveSemantics()
{
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    (void)mpSearchTree->tryInsertNode("E_", "a1");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("A_", "c3");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("B_", "e5");
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "E_:a1:ROOT/H:b2:E_R/A_:c3:HL/B_:e5:A_/B:d4:A_", 5));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->tryInsertNode("AG", "f6");
    mpSearchTree->tryInsertNode("Z", "g7");
    mpSearchTree->tryInsertNode("B_", "e5_1");
    mpSearchTree->tryInsertNode("C", "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "AG:f6:ROOT/Z:g7:AGL/B_:e5_1:Z/C:h8:Z", 4));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AG:f6:ROOT/Z:g7:AGL/B_:e5_1:Z/C:h8:Z", 4));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "AG:f6:ROOT/Z:g7:AGL/B_:e5_1:Z/C:h8:Z", 4));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // test move constructor for trees with custom null value
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryInsertNode("E", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("D", "newval");
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B_:DF:ROOT/E::B_R/D:newval:EL/Z:DF:DL/A_:DF:ZL", 5));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    (void)mpSearchTree->tryInsertNode("G", scDefaultNullValue);
    (void)mpAuxSearchTree->tryInsertNode("D", scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B_:DF:ROOT/E:DF:B_R/D:NULL:EL/Z:DF:DL/A_:DF:ZL", 5));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeySimpleBSTTests::testMergeTrees()
{
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    (void)mpSearchTree->tryInsertNode("E_", "a1_1");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("G", "f6");
    (void)mpSearchTree->tryInsertNode("BC_", "k11");
    (void)mpSearchTree->tryInsertNode("AG", "l12");
    (void)mpSearchTree->tryInsertNode("AB_", "n14");
    (void)mpSearchTree->tryInsertNode("AF", "i9_1");
    (void)mpSearchTree->tryInsertNode("Z", "g7_1");
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    (void)mpAuxSearchTree->tryInsertNode("H", "b2");
    (void)mpAuxSearchTree->tryInsertNode("A_", "c3");
    (void)mpAuxSearchTree->tryInsertNode("B_", "e5");
    (void)mpAuxSearchTree->tryInsertNode("Z", "g7_2");
    (void)mpAuxSearchTree->tryInsertNode("E_", "a1_2");
    (void)mpAuxSearchTree->tryInsertNode("AF", "i9_2");
    (void)mpAuxSearchTree->tryInsertNode("I_", "h8");
    (void)mpAuxSearchTree->tryInsertNode("AD", "j10");
    (void)mpAuxSearchTree->tryInsertNode("AF_", "m13");
    
    const StrIntStrBinarySearchTree searchTreeCopy{*mpSearchTree};
    const StrIntStrBinarySearchTree searchTreeAuxCopy{*mpAuxSearchTree};

    QVERIFY(searchTreeCopy == *mpSearchTree &&      // just a(n additional) check that the copy constructor and == operator work correctly
            searchTreeAuxCopy == *mpAuxSearchTree);

    // first (main) merge
    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1_2:ROOT/BC_:k11:E_/B:d4:E_/AB_:n14:BC_R/Z:g7_2:B/G:f6:B/AF_:m13:AB_/I_:h8:AB_/A_:c3:ZL/AG:l12:GR/B_:e5:A_L/AF:i9_2:AGL/H:b2:AFL/AD:j10:HR", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    
    const StrIntStrBinarySearchTree mainTreeAfterFirstMerge{*mpSearchTree};

    // merge empty tree into unempty tree
    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(*mpSearchTree == mainTreeAfterFirstMerge);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    // merge unempty tree with itself
    merged = mpSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(*mpSearchTree == mainTreeAfterFirstMerge);

    // merge unempty tree into empty tree
    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(*mpAuxSearchTree == mainTreeAfterFirstMerge);

    // merge empty tree with itself
    merged = mpSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // merge two empty trees
    mpAuxSearchTree->clear();
    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    // do inverse merge operation comparing to first merge
    *mpSearchTree = searchTreeCopy;
    *mpAuxSearchTree = searchTreeAuxCopy;

    QVERIFY(*mpSearchTree == searchTreeCopy &&      // just a(n additional) check that the copy assignment operator and == operator work correctly
            *mpAuxSearchTree == searchTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "H:b2:ROOT/A_:c3:H/AF:i9_1:H/B_:e5:A_/Z:g7_1:A_/AD:j10:AF/AG:l12:AF/E_:a1_1:B_L/B:d4:ZR/I_:h8:E_L/G:f6:BR/AF_:m13:I_L/BC_:k11:AF_/AB_:n14:AF_", 14));
    QVERIFY(*mpAuxSearchTree != mainTreeAfterFirstMerge); // test the != operator too
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // merge trees with (same) custom null value
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);

    (void)mpSearchTree->tryInsertNode("E_", "a1");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("A_", "c3");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("B_", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("H_", "e5");
    (void)mpSearchTree->tryInsertNode("G", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("Z", "g7");
    (void)mpSearchTree->tryInsertNode("I_", "h8");
    (void)mpSearchTree->tryInsertNode("G_", "i9");
    (void)mpSearchTree->tryRemoveNode("H_");
    (void)mpSearchTree->tryRemoveNode("B");
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    (void)mpAuxSearchTree->tryInsertNode("I_", scDefaultNullValue);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/G_:i9:E_/H:b2:E_/I_::G_L/A_:c3:H/AF:DF:H/BC_:DF:I_L/B_::A_/G:DF:A_/AD:DF:AF/AG:DF:AF/AF_:DF:BC_R/Z:g7:GL/AB_:DF:AF_R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. default)
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E", "AZ", "B_", "I", "D", "B", "G", "H_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_", "A"}, scDefaultValue, scDefaultNullValue);
    mpSearchTree->tryInsertNode("I", "abc");
    mpAuxSearchTree->tryInsertNode("G", "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E:DF:ROOT/B_:DF:E/AZ:DF:E/H_:DF:B_/D:DF:B_/I:abc:AZL/B:DF:DL/G:DF:IL", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AF:DF:ROOT/I_:DF:AF/AG:DF:AF/BC_:DF:I_/AD:DF:I_/AF_:DF:BC_R/G:xyz:ADL/AB_:DF:AF_R/A:DF:GL", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AF:DF:ROOT/I_:DF:AF/AG:DF:AF/BC_:DF:I_/AD:DF:I_/AF_:DF:BC_R/G:xyz:ADL/AB_:DF:AF_R/A:DF:GL", 9));

    mpAuxSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. custom)
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E", "AZ", "B_", "I", "D", "B", "G", "H_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_", "A"}, scDefaultValue, scCustomNullValue + "1");
    mpSearchTree->tryInsertNode("I", "abc");
    mpAuxSearchTree->tryInsertNode("G", "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E:DF:ROOT/B_:DF:E/AZ:DF:E/H_:DF:B_/D:DF:B_/I:abc:AZL/B:DF:DL/G:DF:IL", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AF:DF:ROOT/I_:DF:AF/AG:DF:AF/BC_:DF:I_/AD:DF:I_/AF_:DF:BC_R/G:xyz:ADL/AB_:DF:AF_R/A:DF:GL", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AF:DF:ROOT/I_:DF:AF/AG:DF:AF/BC_:DF:I_/AD:DF:I_/AF_:DF:BC_R/G:xyz:ADL/AB_:DF:AF_R/A:DF:GL", 9));

    mpAuxSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue + "1" == mpAuxSearchTree->getNullValue());
}

void StrIntKeySimpleBSTTests::testInOrderForwardIterators()
{
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    (void)mpSearchTree->tryInsertNode("E_", "b");
    (void)mpSearchTree->tryInsertNode("H", "z");
    (void)mpSearchTree->tryInsertNode("A_", "_ca");
    (void)mpSearchTree->tryInsertNode("B", "q1");
    (void)mpSearchTree->tryInsertNode("B_", "55");
    (void)mpSearchTree->tryInsertNode("G", "a");
    (void)mpSearchTree->tryInsertNode("Z", "fq");
    (void)mpSearchTree->tryInsertNode("I_", scDefaultValue);
    (void)mpSearchTree->tryInsertNode("AF", "cCc");
    (void)mpSearchTree->tryInsertNode("AD", "abab");
    (void)mpSearchTree->tryInsertNode("BC_", "-c");
    (void)mpSearchTree->tryInsertNode("AG", "b");
    (void)mpSearchTree->tryInsertNode("AF_", "qa");
    (void)mpSearchTree->tryInsertNode("AB_", "dev");
    (void)mpSearchTree->tryInsertNode("AI", "_ca");
    (void)mpSearchTree->tryInsertNode("AE_", scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true),
                                     mpSearchTree->getSize(),
                                     "E_:b:ROOT/I_:DF:E_/H:z:E_/BC_:-c:I_L/A_:_ca:H/AF:cCc:H/AF_:qa:BC_R/B_:55:A_/B:q1:A_/AD:abab:AF/AG:b:AF/AB_:dev:AF_R/Z:fq:B/G:a:B/AI:_ca:AGR/AE_:DF:AB_L",
                                     16));

    StrIntStrBSTIterator it{mpSearchTree->begin()};
    QVERIFY(it.getKey() == "BC_" && it.getValue() == "-c");

    it = mpSearchTree->root();
    QVERIFY(it.getKey() == "E_" && it.getValue() == "b");

    it = mpSearchTree->find("B_");
    QVERIFY(it.getKey() == "B_" && it.getValue() == "55");

    QVERIFY(mpSearchTree->find("BC_") == mpSearchTree->begin());
    QVERIFY(mpSearchTree->find("E_") == mpSearchTree->root());
    QVERIFY(mpSearchTree->find("AB") == mpSearchTree->end());

    std::vector<std::pair<std::optional<StringifiedInteger>, std::string>> traversedElements;
    const std::vector<std::pair<std::optional<StringifiedInteger>, std::string>> c_TraversedElementsRef{{"BC_", "-c"}, {"AF_", "qa"}, {"AE_", "DF"}, {"AB_", "dev"}, {"I_", "DF"}, {"E_", "b"}, {"B_", "55"}, {"A_", "_ca"},
                                                                          {"Z", "fq"}, {"B", "q1"}, {"G", "a"}, {"H", "z"}, {"AD", "abab"}, {"AF", "cCc"}, {"AG", "b"}, {"AI", "_ca"}};

    for (StrIntStrBSTIterator it{mpSearchTree->begin()}; it != mpSearchTree->end(); it.next())
    {
        traversedElements.push_back({it.getKey(), it.getValue()});
    }

    QVERIFY(std::equal(traversedElements.cbegin(), traversedElements.cend(), c_TraversedElementsRef.cbegin()));

    (void)mpSearchTree->tryInsertNode("AD", "BaBa");
    it = mpSearchTree->find("AD");

    QVERIFY(it.getValue() == "BaBa");

    it.next();
    QVERIFY(it.getKey() == "AF");

    it.setValue("bCCC");
    QVERIFY(mpSearchTree->getNodeValue("AF") == "bCCC");

    it = mpSearchTree->find("AI");
    it.next();

    QVERIFY(it == mpSearchTree->end());
    QVERIFY(!it.getKey().has_value() && it.getValue().empty());

    QVERIFY(mpSearchTree->find("B") != mpSearchTree->end());
    (void)mpSearchTree->tryRemoveNode("B");
    QVERIFY(mpSearchTree->find("B") == mpSearchTree->end());
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>("NullVal");
    QVERIFY(mpAuxSearchTree->begin() == mpAuxSearchTree->end() && mpAuxSearchTree->root() == mpAuxSearchTree->end() && mpAuxSearchTree->find("AD") == mpAuxSearchTree->end());

    StrIntStrBSTIterator itAux;
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue().empty() && itAux == mpSearchTree->end() && itAux != mpAuxSearchTree->end());

    itAux = mpAuxSearchTree->end();
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue() == "NullVal");

    // the last part of this test is only relevant for "simple" BSTs (for RB and AVL a balancing is being performed)
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"H_", "D_", "B_", "Z", "C", "E", "I", "AB"}, scDefaultValue);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H_:ROOT/D_:H_R/B_:D_R/Z:B_R/C:ZR/E:CR/I:ER/AB:IR", 8));
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AB", "I", "E", "C", "Z", "B_", "D_", "H_"}, scDefaultValue);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AB:ROOT/I:ABL/E:IL/C:EL/Z:CL/B_:ZL/D_:B_L/H_:D_L", 8));

    std::vector<std::optional<StringifiedInteger>> traversedKeys;
    const std::vector<std::optional<StringifiedInteger>> c_TraversedKeysRef{"H_", "D_", "B_", "Z", "C", "E", "I", "AB"};

    for (StrIntStrBSTIterator it{mpSearchTree->begin()}; it != mpSearchTree->end(); it.next())
    {
        traversedKeys.push_back(it.getKey());
    }

    QVERIFY(std::equal(traversedKeys.cbegin(), traversedKeys.cend(), c_TraversedKeysRef.cbegin()));

    traversedKeys.clear();

    for (StrIntStrBSTIterator it{mpAuxSearchTree->begin()}; it != mpAuxSearchTree->end(); it.next())
    {
        traversedKeys.push_back(it.getKey());
    }

    QVERIFY(std::equal(traversedKeys.cbegin(), traversedKeys.cend(), c_TraversedKeysRef.cbegin()));
}

void StrIntKeySimpleBSTTests::testPrintTree()
{
    qInfo("Creating new tree");
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_"}, scDefaultValue);

    QVERIFY(14 == mpSearchTree->getSize());

    qInfo("The tree nodes are: ");
    mpSearchTree->printTree();

    qInfo("Clearing tree content");
    mpSearchTree->clear();

    QVERIFY(0 == mpSearchTree->getSize());

    mpSearchTree->printTree();
    qInfo("The tree has no nodes");
}

QTEST_APPLESS_MAIN(StrIntKeySimpleBSTTests)

#include "tst_strintkeysimplebsttests.moc"
// clang-format on
