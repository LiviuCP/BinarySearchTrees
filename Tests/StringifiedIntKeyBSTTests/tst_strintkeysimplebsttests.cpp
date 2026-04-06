// clang-format off
#include <QTest>
#include <algorithm>

#include "testutils.h"
#include "binarysearchtree.h"
#include "stringifiedinteger.h"

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
    bool newNodeAdded{false};
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode("E_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode("H", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode("A_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode("B", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL/B:A_R", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode("B_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL/B_:A_/B:A_", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode("G", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL/B_:A_/B:A_/G:BR", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode("Z", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R/A_:HL/B_:A_/B:A_/Z:B/G:B", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode("I_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/A_:HL/B_:A_/B:A_/Z:B/G:B", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AF", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/A_:H/AF:H/B_:A_/B:A_/Z:B/G:B", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AD", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/A_:H/AF:H/B_:A_/B:A_/AD:AFL/Z:B/G:B", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode("BC_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/B_:A_/B:A_/AD:AFL/Z:B/G:B", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AG", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AF_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AB_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/AB_:AF_R/Z:B/G:B", 14));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AI", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/AB_:AF_R/Z:B/G:B/AI:AGR", 15));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AE_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AF_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/AB_:AF_R/Z:B/G:B/AI:AGR/AE_:AB_L", 16));

    // adding nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpSearchTree->addOrUpdateNode("BE", scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            *mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode("BE_", scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            *mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpSearchTree->addOrUpdateNode("BE", scDefaultValue);
    QVERIFY(newNodeAdded &&
            *mpAuxSearchTree != *mpSearchTree);

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode("BE", scDefaultValue);
    QVERIFY(newNodeAdded &&
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
}

void StrIntKeySimpleBSTTests::testRemoveNodes()
{
    bool nodeDeleted{false};
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(*mpSearchTree);

    nodeDeleted = mpSearchTree->removeNode("AF_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/H:E_/BC_:I_L/A_:H/AF:H/AB_:BC_R/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B", 13));

    nodeDeleted = mpSearchTree->removeNode("H");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/AD:E_/BC_:I_L/A_:AD/AF:AD/AB_:BC_R/B_:A_/B:A_/AG:AFR/Z:B/G:B", 12));

    nodeDeleted = mpSearchTree->removeNode("Z");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/AD:E_/BC_:I_L/A_:AD/AF:AD/AB_:BC_R/B_:A_/B:A_/AG:AFR/G:BR", 11));

    nodeDeleted = mpSearchTree->removeNode("BC_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/AD:E_/AB_:I_L/A_:AD/AF:AD/B_:A_/B:A_/AG:AFR/G:BR", 10));

    nodeDeleted = mpSearchTree->removeNode("AG");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/I_:E_/AD:E_/AB_:I_L/A_:AD/AF:AD/B_:A_/B:A_/G:BR", 9));

    nodeDeleted = mpSearchTree->removeNode("E_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/I_:B_/AD:B_/AB_:I_L/A_:AD/AF:AD/B:A_R/G:BR", 8));

    nodeDeleted = mpSearchTree->removeNode("I_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/AB_:B_/AD:B_/A_:AD/AF:AD/B:A_R/G:BR", 7));

    nodeDeleted = mpSearchTree->removeNode("AD");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/AB_:B_/AF:B_/A_:AFL/B:A_R/G:BR", 6));

    nodeDeleted = mpSearchTree->removeNode("A_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/AB_:B_/AF:B_/B:AFL/G:BR", 5));

    nodeDeleted = mpSearchTree->removeNode("B_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/AB_:B/AF:B/G:AFL", 4));

    nodeDeleted = mpSearchTree->removeNode("AB_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/AF:BR/G:AFL", 3));

    nodeDeleted = mpSearchTree->removeNode("AF");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/G:BR", 2));

    nodeDeleted = mpSearchTree->removeNode("B");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT", 1));

    nodeDeleted = mpSearchTree->removeNode("G");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->removeNode("G");
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->removeNode("BE");
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // some additional (corner) cases
    nodeDeleted = mpAuxSearchTree->removeNode("I_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "E_:ROOT/BC_:E_/H:E_/AF_:BC_R/A_:H/AF:H/AB_:AF_R/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B", 13));

    nodeDeleted = mpAuxSearchTree->removeNode("A_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "E_:ROOT/BC_:E_/H:E_/AF_:BC_R/Z:H/AF:H/AB_:AF_R/B_:Z/B:Z/AD:AF/AG:AF/G:BR", 12));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "AB_"}, scDefaultValue);

    nodeDeleted = mpSearchTree->removeNode("B");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT", 1));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "AB_", "G"}, scDefaultValue);
    *mpAuxSearchTree = *mpSearchTree;

    nodeDeleted = mpSearchTree->removeNode("AB_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/G:BR", 2));

    nodeDeleted = mpSearchTree->removeNode("G");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT", 1));

    nodeDeleted = mpAuxSearchTree->removeNode("G");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "B:ROOT/AB_:BL", 2));

    nodeDeleted = mpAuxSearchTree->removeNode("AB_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "B:ROOT", 1));

    // deleting null node from custom null value tree
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue, scCustomNullValue);

    nodeDeleted = mpSearchTree->removeNode("E_");
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/B_:A_/C:A_/B:C/D:C", 5));

    // deleting same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue);

    nodeDeleted = mpSearchTree->removeNode("C");
    QVERIFY(nodeDeleted &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeDeleted = mpAuxSearchTree->removeNode("C");
    QVERIFY(nodeDeleted &&
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

    (void)mpSearchTree->addOrUpdateNode("E_", "a1");
    (void)mpSearchTree->addOrUpdateNode("H", "b2");
    (void)mpSearchTree->addOrUpdateNode("A_", "c3");
    (void)mpSearchTree->addOrUpdateNode("B", "d4");
    (void)mpSearchTree->addOrUpdateNode("B_", "e5");
    (void)mpSearchTree->addOrUpdateNode("G", "f6");
    (void)mpSearchTree->addOrUpdateNode("Z", "g7");
    (void)mpSearchTree->addOrUpdateNode("I_", "h8");
    (void)mpSearchTree->addOrUpdateNode("AF", "i9");
    (void)mpSearchTree->addOrUpdateNode("AD", "j10");
    (void)mpSearchTree->addOrUpdateNode("BC_", "k11");
    (void)mpSearchTree->addOrUpdateNode("AG", "l12");
    (void)mpSearchTree->addOrUpdateNode("AF_", "m13");
    (void)mpSearchTree->addOrUpdateNode("AB_", "n14");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/I_:h8:E_/H:b2:E_/BC_:k11:I_L/A_:c3:H/AF:i9:H/AF_:m13:BC_R/B_:e5:A_/B:d4:A_/AD:j10:AF/AG:l12:AF/AB_:n14:AF_R/Z:g7:B/G:f6:B", 14));

    QVERIFY("a1" == mpSearchTree->getNodeValue("E_") &&
            "g7" == mpSearchTree->getNodeValue("Z") &&
            "i9" == mpSearchTree->getNodeValue("AF"));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue("BE"));

    // update by adding/modifying node
    bool newNodeAdded{false};

    newNodeAdded = mpSearchTree->addOrUpdateNode("B", "d4_1");
    QVERIFY(!newNodeAdded &&
            "d4_1" == mpSearchTree->getNodeValue("B"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("I_", "b2");
    QVERIFY(!newNodeAdded &&
            "b2" == mpSearchTree->getNodeValue("I_"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AD", "j10");
    QVERIFY(!newNodeAdded &&
            "j10" == mpSearchTree->getNodeValue("AD"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AG", "L12");
    QVERIFY(!newNodeAdded &&
            "L12" == mpSearchTree->getNodeValue("AG"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AB_", scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            "n14" == mpSearchTree->getNodeValue("AB_"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("BE", scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue("BE"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("BE", "o15");
    QVERIFY(newNodeAdded &&
            "o15" == mpSearchTree->getNodeValue("BE"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("BE", scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            "o15" == mpSearchTree->getNodeValue("BE"));

    // update by deleting node
    bool nodeDeleted{false};

    nodeDeleted = mpSearchTree->removeNode("G");
    QVERIFY(nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue("G"));

    nodeDeleted = mpSearchTree->removeNode("BH_");
    QVERIFY(!nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue("BH_"));

    // delete and re-add node with different value
    nodeDeleted = mpSearchTree->removeNode("AF");
    newNodeAdded = mpSearchTree->addOrUpdateNode("AF", "i9_new");

    QVERIFY(nodeDeleted &&
            newNodeAdded &&
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

    (void)mpSearchTree->addOrUpdateNode("E_", "a1");
    (void)mpSearchTree->addOrUpdateNode("H", "b2");
    (void)mpSearchTree->addOrUpdateNode("A_", scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode("B", "d4");
    (void)mpSearchTree->addOrUpdateNode("B_", "e5");
    (void)mpSearchTree->addOrUpdateNode("G", "f6");
    (void)mpSearchTree->addOrUpdateNode("H_", scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode("Z", "g7");
    (void)mpSearchTree->addOrUpdateNode("I_", "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/H_::E_/H:b2:E_/I_:h8:H_L/A_::HL/B_:e5:A_/B:d4:A_/Z:g7:B/G:f6:B", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());

    newNodeAdded = mpSearchTree->addOrUpdateNode("B", "d4_1");
    QVERIFY(!newNodeAdded &&
            "d4_1" == mpSearchTree->getNodeValue("B"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("A_", "c3");
    QVERIFY(!newNodeAdded &&
            "c3" == mpSearchTree->getNodeValue("A_"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("G", scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            "f6" == mpSearchTree->getNodeValue("G"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("B_", scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue("B_"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("BE", scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            scCustomNullValue == mpSearchTree->getNodeValue("BE"));

    newNodeAdded = mpSearchTree->addOrUpdateNode("G_", "i9");
    QVERIFY(newNodeAdded &&
            "i9" == mpSearchTree->getNodeValue("G_"));

    nodeDeleted = mpSearchTree->removeNode("BE_");
    QVERIFY(!nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue("BE_"));

    nodeDeleted = mpSearchTree->removeNode("H_");
    QVERIFY(nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue("H_"));

    nodeDeleted = mpSearchTree->removeNode("B");
    QVERIFY(nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue("B"));

    nodeDeleted = mpSearchTree->removeNode("A_");
    newNodeAdded = mpSearchTree->addOrUpdateNode("A_", scDefaultNullValue);

    QVERIFY(nodeDeleted &&
            newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue("A_"));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/G_:i9:E_/H:b2:E_/I_:h8:G_L/Z:g7:HL/B_::Z/G:f6:Z/A_::B_R", 8));
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(*mpSearchTree);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode("AG", "df");
    QVERIFY(newNodeAdded &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    newNodeAdded = mpSearchTree->addOrUpdateNode("G", scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue("G"));

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode("D", scCustomNullValue);
    QVERIFY(!newNodeAdded &&
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

    (void)mpSearchTree->addOrUpdateNode("E_", "a1");
    (void)mpSearchTree->addOrUpdateNode("H", "b2");
    (void)mpSearchTree->addOrUpdateNode("A_", "c3");
    (void)mpSearchTree->addOrUpdateNode("B", "d4");
    (void)mpSearchTree->addOrUpdateNode("B_", "e5");
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "E_:a1:ROOT/H:b2:E_R/A_:c3:HL/B_:e5:A_/B:d4:A_", 5));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->addOrUpdateNode("AG", "f6");
    mpSearchTree->addOrUpdateNode("Z", "g7");
    mpSearchTree->addOrUpdateNode("B_", "e5_1");
    mpSearchTree->addOrUpdateNode("C", "h8");

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

    (void)mpSearchTree->addOrUpdateNode("E", scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode("D", "newval");
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B_:DF:ROOT/E::B_R/D:newval:EL/Z:DF:DL/A_:DF:ZL", 5));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    (void)mpSearchTree->addOrUpdateNode("G", scDefaultNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode("D", scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B_:DF:ROOT/E:DF:B_R/D:NULL:EL/Z:DF:DL/A_:DF:ZL", 5));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeySimpleBSTTests::testMergeTrees()
{
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    (void)mpSearchTree->addOrUpdateNode("E_", "a1_1");
    (void)mpSearchTree->addOrUpdateNode("B", "d4");
    (void)mpSearchTree->addOrUpdateNode("G", "f6");
    (void)mpSearchTree->addOrUpdateNode("BC_", "k11");
    (void)mpSearchTree->addOrUpdateNode("AG", "l12");
    (void)mpSearchTree->addOrUpdateNode("AB_", "n14");
    (void)mpSearchTree->addOrUpdateNode("AF", "i9_1");
    (void)mpSearchTree->addOrUpdateNode("Z", "g7_1");
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    (void)mpAuxSearchTree->addOrUpdateNode("H", "b2");
    (void)mpAuxSearchTree->addOrUpdateNode("A_", "c3");
    (void)mpAuxSearchTree->addOrUpdateNode("B_", "e5");
    (void)mpAuxSearchTree->addOrUpdateNode("Z", "g7_2");
    (void)mpAuxSearchTree->addOrUpdateNode("E_", "a1_2");
    (void)mpAuxSearchTree->addOrUpdateNode("AF", "i9_2");
    (void)mpAuxSearchTree->addOrUpdateNode("I_", "h8");
    (void)mpAuxSearchTree->addOrUpdateNode("AD", "j10");
    (void)mpAuxSearchTree->addOrUpdateNode("AF_", "m13");
    
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

    (void)mpSearchTree->addOrUpdateNode("E_", "a1");
    (void)mpSearchTree->addOrUpdateNode("H", "b2");
    (void)mpSearchTree->addOrUpdateNode("A_", "c3");
    (void)mpSearchTree->addOrUpdateNode("B", "d4");
    (void)mpSearchTree->addOrUpdateNode("B_", scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode("H_", "e5");
    (void)mpSearchTree->addOrUpdateNode("G", scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode("Z", "g7");
    (void)mpSearchTree->addOrUpdateNode("I_", "h8");
    (void)mpSearchTree->addOrUpdateNode("G_", "i9");
    (void)mpSearchTree->removeNode("H_");
    (void)mpSearchTree->removeNode("B");
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode("I_", scDefaultNullValue);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1:ROOT/G_:i9:E_/H:b2:E_/I_::G_L/A_:c3:H/AF:DF:H/BC_:DF:I_L/B_::A_/G:DF:A_/AD:DF:AF/AG:DF:AF/AF_:DF:BC_R/Z:g7:GL/AB_:DF:AF_R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. default)
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"E", "AZ", "B_", "I", "D", "B", "G", "H_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_", "A"}, scDefaultValue, scDefaultNullValue);
    mpSearchTree->addOrUpdateNode("I", "abc");
    mpAuxSearchTree->addOrUpdateNode("G", "xyz");

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
    mpSearchTree->addOrUpdateNode("I", "abc");
    mpAuxSearchTree->addOrUpdateNode("G", "xyz");

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

    (void)mpSearchTree->addOrUpdateNode("E_", "b");
    (void)mpSearchTree->addOrUpdateNode("H", "z");
    (void)mpSearchTree->addOrUpdateNode("A_", "_ca");
    (void)mpSearchTree->addOrUpdateNode("B", "q1");
    (void)mpSearchTree->addOrUpdateNode("B_", "55");
    (void)mpSearchTree->addOrUpdateNode("G", "a");
    (void)mpSearchTree->addOrUpdateNode("Z", "fq");
    (void)mpSearchTree->addOrUpdateNode("I_", scDefaultValue);
    (void)mpSearchTree->addOrUpdateNode("AF", "cCc");
    (void)mpSearchTree->addOrUpdateNode("AD", "abab");
    (void)mpSearchTree->addOrUpdateNode("BC_", "-c");
    (void)mpSearchTree->addOrUpdateNode("AG", "b");
    (void)mpSearchTree->addOrUpdateNode("AF_", "qa");
    (void)mpSearchTree->addOrUpdateNode("AB_", "dev");
    (void)mpSearchTree->addOrUpdateNode("AI", "_ca");
    (void)mpSearchTree->addOrUpdateNode("AE_", scDefaultValue);

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

    (void)mpSearchTree->addOrUpdateNode("AD", "BaBa");
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
    (void)mpSearchTree->removeNode("B");
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
