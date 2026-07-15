// clang-format off
#include <QTest>
#include <algorithm>

#include "testutils.h"
#include "stringifiedinteger.h"

import avltree;

using namespace TestUtils;
using StrIntStrAVLTree = AVLTree<StringifiedInteger, std::string>;
using StrIntStrAVLIterator = StrIntStrAVLTree::InOrderForwardIterator;
using upStrIntStrAVLTree = std::unique_ptr<StrIntStrAVLTree>;

class StrIntKeyAVLTreesTests : public QObject
{
    Q_OBJECT

public:
    StrIntKeyAVLTreesTests();

private slots:
    void init();
    void cleanup();

    void testAddNodes();
    void testRemoveNodes();
    void testUpdateNodeValue();
    void testMoveSemantics();
    void testMergeTrees();
    void testInOrderForwardIterators();

private:
    upStrIntStrAVLTree mpSearchTree;
    upStrIntStrAVLTree mpAuxSearchTree;
};

StrIntKeyAVLTreesTests::StrIntKeyAVLTreesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

void StrIntKeyAVLTreesTests::init()
{
    QVERIFY(!mpSearchTree && !mpAuxSearchTree);
}

void StrIntKeyAVLTreesTests::cleanup()
{
    mpSearchTree.reset();
    mpAuxSearchTree.reset();
}

void StrIntKeyAVLTreesTests::testAddNodes()
{
    bool nodeInserted{false};

    mpSearchTree = std::make_unique<StrIntStrAVLTree>();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeInserted = mpSearchTree->tryInsertNode("E_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT", 1));

    nodeInserted = mpSearchTree->tryInsertNode("H", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/H:E_R", 2));

    nodeInserted = mpSearchTree->tryInsertNode("A_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/H:A_", 3));

    nodeInserted = mpSearchTree->tryInsertNode("B", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/H:A_/B:HL", 4));

    nodeInserted = mpSearchTree->tryInsertNode("B_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/H:A_/B_:E_R/B:HL", 5));

    nodeInserted = mpSearchTree->tryInsertNode("G", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/B_:E_R/B:G/H:G", 6));

    nodeInserted = mpSearchTree->tryInsertNode("Z", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/B_:E_R/B:G/H:G/Z:BL", 7));

    nodeInserted = mpSearchTree->tryInsertNode("I_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/I_:E_/B_:E_/B:G/H:G/Z:BL", 8));

    nodeInserted = mpSearchTree->tryInsertNode("AF", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/I_:E_/B_:E_/B:G/H:G/Z:BL/AF:HR", 9));

    nodeInserted = mpSearchTree->tryInsertNode("AD", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/I_:E_/B_:E_/B:G/AD:G/Z:BL/H:AD/AF:AD", 10));

    nodeInserted = mpSearchTree->tryInsertNode("BC_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/I_:E_/B_:E_/B:G/AD:G/BC_:I_L/Z:BL/H:AD/AF:AD", 11));

    nodeInserted = mpSearchTree->tryInsertNode("AG", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/I_:E_/B_:E_/B:G/AD:G/BC_:I_L/Z:BL/H:AD/AF:AD/AG:AFR", 12));

    nodeInserted = mpSearchTree->tryInsertNode("AF_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/AF_:E_/B_:E_/B:G/AD:G/BC_:AF_/I_:AF_/Z:BL/H:AD/AF:AD/AG:AFR", 13));

    nodeInserted = mpSearchTree->tryInsertNode("AB_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AF_:I_/E_:I_/B:G/AD:G/BC_:AF_/AB_:AF_/B_:E_R/Z:BL/H:AD/AF:AD/AG:AFR", 14));

    nodeInserted = mpSearchTree->tryInsertNode("AI", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(),"A_:ROOT/I_:A_/G:A_/AF_:I_/E_:I_/B:G/AD:G/BC_:AF_/AB_:AF_/B_:E_R/Z:BL/H:AD/AG:AD/AF:AG/AI:AG", 15));

    nodeInserted = mpSearchTree->tryInsertNode("AE_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AF_:I_/E_:I_/B:G/AD:G/BC_:AF_/AB_:AF_/B_:E_R/Z:BL/H:AD/AG:AD/AE_:AB_L/AF:AG/AI:AG", 16));

    // inserting nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue, scCustomNullValue);

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

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AF_:I_/E_:I_/B:G/AG:G/BC_:AF_/AB_:AF_/B_:E_R/Z:BL/AD:AG/AI:AG/AE_:AB_L/H:AD/AF:AD/BE:AIR", 17));

    // some additional (corner) cases
    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"BC_", "AF_", "AE_", "AB_", "I_", "E_", "B_", "A_", "Z", "B", "G", "H", "AD", "AF", "AG", "AI"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AB_:A_/H:A_/AF_:AB_/E_:AB_/B:H/AF:H/BC_:AF_/AE_:AF_/I_:E_/B_:E_/Z:B/G:B/AD:AF/AG:AF/AI:AGR", 16));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"AI", "AG", "AF", "AD", "H", "G", "B", "Z", "A_", "B_", "E_", "I_", "AB_", "AE_", "AF_", "BC_"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT/I_:Z/AD:Z/AE_:I_/B_:I_/G:AD/AG:AD/AF_:AE_/AB_:AE_/E_:B_/A_:B_/B:G/H:G/AF:AG/AI:AG/BC_:AF_L", 16));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"BC_", "AI", "AF_", "AG", "AE_", "AF", "AB_", "AD", "I_", "H", "E_", "G", "B_", "B", "A_", "Z"}, scDefaultValue, scCustomNullValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "I_:ROOT/AE_:I_/B:I_/AF_:AE_/AB_:AE_/B_:B/AD:B/BC_:AF_L/E_:B_/A_:B_/G:AD/AG:AD/Z:A_R/H:GR/AF:AG/AI:AG", 16));

    mpSearchTree->clear();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "B", "D", "Z", "A"}, scDefaultValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "C_", "B", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree); // in this particular case due to AVL tree construction rules the trees become equal when third element is being inserted (unlike the basic BST)

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "A", "Z"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"C_", "E_", "B", "D", "Z", "A"}, scDefaultValue, scDefaultNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"C_", "B", "E_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{}, scDefaultValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    // small test including the "extreme" key values: N_ and N
    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "N_", "AG", "AF_", "AB_", "N", "AE_"}, scDefaultValue);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AF_:I_/E_:I_/B:G/AD:G/N_:AF_/AB_:AF_/B_:E_R/Z:BL/H:AD/AG:AD/AE_:AB_L/AF:AG/N:AG", 16));
}

void StrIntKeyAVLTreesTests::testRemoveNodes()
{
    bool nodeRemoved{false};

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(*mpSearchTree);

    nodeRemoved = mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AE_:I_/E_:I_/B:G/AD:G/BC_:AE_/AB_:AE_/B_:E_R/Z:BL/H:AD/AG:AD/AF:AG/AI:AG", 15));

    nodeRemoved = mpSearchTree->tryRemoveNode("H");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AE_:I_/E_:I_/B:G/AF:G/BC_:AE_/AB_:AE_/B_:E_R/Z:BL/AD:AF/AG:AF/AI:AGR", 14));

    nodeRemoved = mpSearchTree->tryRemoveNode("Z");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/AF:A_/AE_:I_/E_:I_/G:AF/AG:AF/BC_:AE_/AB_:AE_/B_:E_R/B:G/AD:G/AI:AGR", 13));

    nodeRemoved = mpSearchTree->tryRemoveNode("BC_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/AF:A_/AE_:I_/E_:I_/G:AF/AG:AF/AB_:AE_R/B_:E_R/B:G/AD:G/AI:AGR", 12));

    nodeRemoved = mpSearchTree->tryRemoveNode("AI");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/AF:A_/AE_:I_/E_:I_/G:AF/AG:AF/AB_:AE_R/B_:E_R/B:G/AD:G", 11));

    nodeRemoved = mpSearchTree->tryRemoveNode("AG");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AE_:I_/E_:I_/B:G/AF:G/AB_:AE_R/B_:E_R/AD:AFL", 10));

    nodeRemoved = mpSearchTree->tryRemoveNode("E_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AE_:I_/B_:I_/B:G/AF:G/AB_:AE_R/AD:AFL", 9));

    nodeRemoved = mpSearchTree->tryRemoveNode("I_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AB_:A_/G:A_/AE_:AB_/B_:AB_/B:G/AF:G/AD:AFL", 8));

    nodeRemoved = mpSearchTree->tryRemoveNode("AE_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AB_:A_/G:A_/B_:AB_R/B:G/AF:G/AD:AFL", 7));

    nodeRemoved = mpSearchTree->tryRemoveNode("AD");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AB_:A_/G:A_/B_:AB_R/B:G/AF:G", 6));

    nodeRemoved = mpSearchTree->tryRemoveNode("A_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/AB_:B/G:B/B_:AB_R/AF:GR", 5));

    nodeRemoved = mpSearchTree->tryRemoveNode("B_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/AB_:B/G:B/AF:GR", 4));

    nodeRemoved = mpSearchTree->tryRemoveNode("AB_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT/B:G/AF:G", 3));

    nodeRemoved = mpSearchTree->tryRemoveNode("AF"); // root and two children, erase right child
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT/B:GL", 2));

    nodeRemoved = mpSearchTree->tryRemoveNode("B"); // root and left child, erase left child
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

    // we will stop checking the nodeRemoved variable (obvious enough)
    (void)mpAuxSearchTree->tryRemoveNode("AE_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AF_:I_/E_:I_/B:G/AD:G/BC_:AF_/AB_:AF_/B_:E_R/Z:BL/H:AD/AG:AD/AF:AG/AI:AG", 15));

    (void)mpAuxSearchTree->tryRemoveNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AF_:I_/E_:I_/B:G/AD:G/BC_:AF_/AB_:AF_/B_:E_R/Z:BL/H:AD/AG:AD/AF:AGL", 14));

    (void)mpAuxSearchTree->tryRemoveNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/AF_:I_/E_:I_/B:G/AD:G/BC_:AF_L/B_:E_R/Z:BL/H:AD/AG:AD/AF:AGL", 13));

    (void)mpAuxSearchTree->tryRemoveNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/BC_:I_/E_:I_/B:G/AD:G/B_:E_R/Z:BL/H:AD/AG:AD/AF:AGL", 12));

    (void)mpAuxSearchTree->tryRemoveNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/I_:A_/G:A_/BC_:I_/E_:I_/B:G/AD:G/B_:E_R/Z:BL/H:AD/AF:AD", 11));

    (void)mpAuxSearchTree->tryRemoveNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/I_:E_/B_:E_/B:G/AD:G/Z:BL/H:AD/AF:AD", 10));

    (void)mpAuxSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/I_:E_/B_:E_/B:G/AF:G/Z:BL/H:AFL", 9));

    (void)mpAuxSearchTree->tryRemoveNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/I_:E_/B_:E_/B:G/H:G/Z:BL", 8));

    (void)mpAuxSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/B_:E_R/B:G/H:G/Z:BL", 7));

    (void)mpAuxSearchTree->tryRemoveNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/E_:A_/G:A_/B_:E_R/B:G/H:G", 6));

    (void)mpAuxSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/E_:A_/H:A_/B_:E_R/B:HL", 5));

    (void)mpAuxSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/E_:A_/H:A_/B:HL", 4));

    (void)mpAuxSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT/E_:A_/H:A_", 3));

    (void)mpAuxSearchTree->tryRemoveNode("A_"); // root and two children, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "H:ROOT/E_:HL", 2));

    (void)mpAuxSearchTree->tryRemoveNode("H"); // root and left child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "E_:ROOT", 1));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AB_:A_/G:A_/AF_:AB_/I_:AB_/B:G/AD:G/BC_:AF_/AE_:AF_/B_:I_R/Z:BL/H:AD/AG:AD/AF:AG/AI:AG", 15));

    (void)mpSearchTree->tryRemoveNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AB_:A_/G:A_/AF_:AB_/I_:AB_/B:G/AF:G/BC_:AF_/AE_:AF_/B_:I_R/Z:BL/AD:AF/AG:AF/AI:AGR", 14));

    (void)mpSearchTree->tryRemoveNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT/AB_:Z/AF:Z/AF_:AB_/I_:AB_/G:AF/AG:AF/BC_:AF_/AE_:AF_/B_:I_R/B:G/AD:G/AI:AGR", 13));

    (void)mpSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT/AB_:Z/AF:Z/AF_:AB_/I_:AB_/G:AF/AG:AF/BC_:AF_/AE_:AF_/B_:I_R/AD:GR/AI:AGR", 12));

    (void)mpSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT/AB_:Z/AF:Z/AF_:AB_/I_:AB_/G:AF/AG:AF/BC_:AF_/AE_:AF_/AD:GR/AI:AGR", 11));

    (void)mpSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT/AB_:Z/AF:Z/AF_:AB_/I_:AB_/AD:AF/AG:AF/BC_:AF_/AE_:AF_/AI:AGR", 10));

    (void)mpSearchTree->tryRemoveNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT/AB_:AD/AG:AD/AF_:AB_/I_:AB_/AF:AG/AI:AG/BC_:AF_/AE_:AF_", 9));

    (void)mpSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT/AF_:AD/AG:AD/BC_:AF_/AB_:AF_/AF:AG/AI:AG/AE_:AB_L", 8));

    (void)mpSearchTree->tryRemoveNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT/AF_:AD/AG:AD/BC_:AF_/AB_:AF_/AI:AGR/AE_:AB_L", 7));

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT/AF_:AB_/AG:AB_/BC_:AF_/AE_:AF_/AI:AGR", 6));

    (void)mpSearchTree->tryRemoveNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT/AF_:AB_/AG:AB_/AE_:AF_R/AI:AGR", 5));

    (void)mpSearchTree->tryRemoveNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT/AF_:AB_/AI:AB_/AE_:AF_R", 4));

    (void)mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT/AE_:AB_/AI:AB_", 3));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"BC_", "AF_", "AE_", "AB_", "I_", "E_", "B_", "A_", "Z", "B", "G", "H", "AD", "AF", "AG", "AI"}, scDefaultValue);

    (void)mpSearchTree->tryRemoveNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AB_:A_/H:A_/AF_:AB_/E_:AB_/B:H/AF:H/AE_:AF_R/I_:E_/B_:E_/Z:B/G:B/AD:AF/AG:AF/AI:AGR", 15));

    (void)mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AB_:A_/H:A_/AE_:AB_/E_:AB_/B:H/AF:H/I_:E_/B_:E_/Z:B/G:B/AD:AF/AG:AF/AI:AGR", 14));

    (void)mpSearchTree->tryRemoveNode("AE_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/I_:A_/H:A_/AB_:I_/E_:I_/B:H/AF:H/B_:E_R/Z:B/G:B/AD:AF/AG:AF/AI:AGR", 13));

    (void)mpSearchTree->tryRemoveNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT/A_:H/AF:H/E_:A_/B:A_/AD:AF/AG:AF/I_:E_/B_:E_/Z:B/G:B/AI:AGR", 12));

    (void)mpSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT/A_:H/AF:H/E_:A_/B:A_/AD:AF/AG:AF/B_:E_R/Z:B/G:B/AI:AGR", 11));

    (void)mpSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT/A_:H/AF:H/B_:A_/B:A_/AD:AF/AG:AF/Z:B/G:B/AI:AGR", 10));

    (void)mpSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT/Z:H/AF:H/A_:Z/B:Z/AD:AF/AG:AF/G:BR/AI:AGR", 9));

    (void)mpSearchTree->tryRemoveNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT/B:H/AF:H/Z:B/G:B/AD:AF/AG:AF/AI:AGR", 8));

    (void)mpSearchTree->tryRemoveNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT/B:H/AF:H/G:BR/AD:AF/AG:AF/AI:AGR", 7));

    (void)mpSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AF:ROOT/H:AF/AG:AF/G:H/AD:H/AI:AGR", 6));

    (void)mpSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AF:ROOT/H:AF/AG:AF/AD:HR/AI:AGR", 5));

    (void)mpSearchTree->tryRemoveNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AF:ROOT/AD:AF/AG:AF/AI:AGR", 4));

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT/AF:AG/AI:AG", 3));

    (void)mpSearchTree->tryRemoveNode("AF"); // root and two children, erase left child
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT/AI:AGR", 2));

    (void)mpSearchTree->tryRemoveNode("AG"); // root and right child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AI:ROOT", 1));

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"AI", "AG", "AF", "AD", "H", "G", "B", "Z", "A_", "B_", "E_", "I_", "AB_", "AE_", "AF_", "BC_"}, scDefaultValue, scCustomNullValue);

    (void)mpAuxSearchTree->tryRemoveNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT/I_:Z/AD:Z/AE_:I_/B_:I_/G:AD/AG:AD/AF_:AE_/AB_:AE_/E_:B_/A_:B_/B:G/H:G/AF:AGL/BC_:AF_L", 15));

    (void)mpAuxSearchTree->tryRemoveNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT/I_:Z/AD:Z/AE_:I_/B_:I_/G:AD/AF:AD/AF_:AE_/AB_:AE_/E_:B_/A_:B_/B:G/H:G/BC_:AF_L", 14));

    (void)mpAuxSearchTree->tryRemoveNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT/I_:Z/G:Z/AE_:I_/B_:I_/B:G/AD:G/AF_:AE_/AB_:AE_/E_:B_/A_:B_/H:ADL/BC_:AF_L", 13));

    (void)mpAuxSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT/AE_:I_/Z:I_/AF_:AE_/AB_:AE_/B_:Z/G:Z/BC_:AF_L/E_:B_/A_:B_/B:G/H:G", 12));

    (void)mpAuxSearchTree->tryRemoveNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT/AE_:I_/Z:I_/AF_:AE_/AB_:AE_/B_:Z/G:Z/BC_:AF_L/E_:B_/A_:B_/B:GL", 11));

    (void)mpAuxSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT/AE_:I_/Z:I_/AF_:AE_/AB_:AE_/B_:Z/B:Z/BC_:AF_L/E_:B_/A_:B_", 10));

    (void)mpAuxSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT/AE_:I_/B_:I_/AF_:AE_/AB_:AE_/E_:B_/Z:B_/BC_:AF_L/A_:ZL", 9));

    (void)mpAuxSearchTree->tryRemoveNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT/AE_:I_/B_:I_/AF_:AE_/AB_:AE_/E_:B_/A_:B_/BC_:AF_L", 8));

    (void)mpAuxSearchTree->tryRemoveNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT/AE_:I_/B_:I_/AF_:AE_/AB_:AE_/E_:B_L/BC_:AF_L", 7));

    (void)mpAuxSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AE_:ROOT/AF_:AE_/I_:AE_/BC_:AF_L/AB_:I_/E_:I_", 6));

    (void)mpAuxSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AE_:ROOT/AF_:AE_/I_:AE_/BC_:AF_L/AB_:I_L", 5));

    (void)mpAuxSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AE_:ROOT/AF_:AE_/AB_:AE_/BC_:AF_L", 4));

    (void)mpAuxSearchTree->tryRemoveNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AF_:ROOT/BC_:AF_/AE_:AF_", 3));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"BC_", "AI", "AF_", "AG", "AE_", "AF", "AB_", "AD", "I_", "H", "E_", "G", "B_", "B", "A_", "Z"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "I_:ROOT/AE_:I_/B:I_/AF_:AE_/AB_:AE_/B_:B/AF:B/BC_:AF_L/E_:B_/A_:B_/G:AF/AG:AF/Z:A_R/H:GR/AI:AGR", 15));

    (void)mpSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/AE_:E_/B:E_/AF_:AE_/AB_:AE_/A_:B/AF:B/BC_:AF_L/B_:A_/Z:A_/G:AF/AG:AF/H:GR/AI:AGR", 14));

    (void)mpSearchTree->tryRemoveNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT/AE_:E_/B:E_/AF_:AE_/AB_:AE_/A_:B/AF:B/BC_:AF_L/B_:A_/Z:A_/G:AF/AG:AF/AI:AGR", 13));

    (void)mpSearchTree->tryRemoveNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/E_:B/AF:B/AF_:E_/A_:E_/G:AF/AG:AF/BC_:AF_/AE_:AF_/B_:A_/Z:A_/AI:AGR", 12));

    (void)mpSearchTree->tryRemoveNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/E_:B/AG:B/AF_:E_/A_:E_/G:AG/AI:AG/BC_:AF_/AE_:AF_/B_:A_/Z:A_", 11));

    (void)mpSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/B_:B/AG:B/AF_:B_/A_:B_/G:AG/AI:AG/BC_:AF_/AE_:AF_/Z:A_R", 10));

    (void)mpSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/B_:B/AG:B/AF_:B_/A_:B_/AI:AGR/BC_:AF_/AE_:AF_/Z:A_R", 9));

    (void)mpSearchTree->tryRemoveNode("AE_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/B_:B/AG:B/AF_:B_/A_:B_/AI:AGR/BC_:AF_L/Z:A_R", 8));

    (void)mpSearchTree->tryRemoveNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT/AF_:B_/B:B_/BC_:AF_L/A_:B/AI:B/Z:A_R", 7));

    (void)mpSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AF_:A_/B:A_/BC_:AF_L/Z:B/AI:B", 6));

    (void)mpSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/AF_:A_/AI:A_/BC_:AF_L/Z:AIL", 5));

    (void)mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/BC_:A_/AI:A_/Z:AIL", 4));

    (void)mpSearchTree->tryRemoveNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT/BC_:A_/Z:A_", 3));

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"C_", "E_", "B"}, scDefaultValue);

    (void)mpAuxSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "C_:ROOT/B:C_R", 2));

    (void)mpAuxSearchTree->tryRemoveNode("B"); // root and right child, erase right child
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "C_:ROOT", 1));

    // removing null node from custom null value tree
    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue, scCustomNullValue);

    nodeRemoved = mpSearchTree->tryRemoveNode("E_");
    QVERIFY(!nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT/A_:B/C:B/B_:A_L/D:CR", 5));

    // removing same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue);

    nodeRemoved = mpSearchTree->tryRemoveNode("C");
    QVERIFY(nodeRemoved &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeRemoved = mpAuxSearchTree->tryRemoveNode("C");
    QVERIFY(nodeRemoved &&
            *mpSearchTree == *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "B:ROOT/A_:B/D:B/B_:A_L", 4));
}

void StrIntKeyAVLTreesTests::testUpdateNodeValue()
{
    mpSearchTree = std::make_unique<StrIntStrAVLTree>();

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/I_:h8:A_/G:f6:A_/AF_:m13:I_/E_:a1:I_/B:d4:G/AD:j10:G/BC_:k11:AF_/AB_:n14:AF_/B_:e5:E_R/Z:g7:BL/H:b2:AD/AF:i9:AD/AG:l12:AFR", 14));

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/I_:b2:A_/H:b2:A_/AF_:m13:I_/E_:a1:I_/B:d4_1:H/AG:L12:H/BC_:k11:AF_/AB_:n14:AF_/B_:e5:E_R/Z:g7:BL/AD:j10:AG/BE:o15:AG/AF:i9_new:ADR", 14));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue("E_") &&
            scDefaultNullValue == mpSearchTree->getNodeValue("Z") &&
            scDefaultNullValue == mpSearchTree->getNodeValue("AF"));

    // test with same value for all nodes
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:DF:ROOT/I_:DF:A_/G:DF:A_/AF_:DF:I_/E_:DF:I_/B:DF:G/AD:DF:G/BC_:DF:AF_/AB_:DF:AF_/B_:DF:E_R/Z:DF:BL/H:DF:AD/AF:DF:AD/AG:DF:AFR", 14));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue("E_") &&
            scDefaultValue == mpAuxSearchTree->getNodeValue("Z") &&
            scDefaultValue == mpAuxSearchTree->getNodeValue("AF"));

    // test updating custom null value tree node values

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);

    (void)mpSearchTree->tryInsertNode("E_", "a1");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("A_", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("B_", "e5");
    (void)mpSearchTree->tryInsertNode("G", "f6");
    (void)mpSearchTree->tryInsertNode("H_", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("Z", "g7");
    (void)mpSearchTree->tryInsertNode("I_", "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_::ROOT/E_:a1:A_/G:f6:A_/H_::E_/B_:e5:E_/B:d4:G/H:b2:G/I_:h8:H_L/Z:g7:BL", 9));
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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "Z:g7:ROOT/E_:a1:Z/G:f6:Z/G_:i9:E_/B_::E_/H:b2:GR/I_:h8:G_L/A_::B_R", 8));

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(*mpSearchTree);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    nodeInserted = mpAuxSearchTree->tryInsertNode("AG", "df");
    QVERIFY(nodeInserted &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    nodeInserted = mpSearchTree->tryInsertNode("G", scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue("G"));

    nodeInserted = mpAuxSearchTree->tryInsertNode("D", scCustomNullValue);
    QVERIFY(!nodeInserted &&
            scCustomNullValue == mpAuxSearchTree->getNodeValue("D"));

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT/I_:DF:AD/AF:DF:AD/AF_:DF:I_/G::I_/AG:DF:AFR/BC_:DF:AF_/AB_:DF:AF_", 8));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeyAVLTreesTests::testMoveSemantics()
{
    mpSearchTree = std::make_unique<StrIntStrAVLTree>();

    (void)mpSearchTree->tryInsertNode("E_", "a1");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("A_", "c3");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("B_", "e5");

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/E_:a1:A_/H:b2:A_/B_:e5:E_R/B:d4:HL", 5));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->tryInsertNode("AG", "f6");
    mpSearchTree->tryInsertNode("Z", "g7");
    mpSearchTree->tryInsertNode("B_", "e5_1");
    mpSearchTree->tryInsertNode("C", "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "Z:g7:ROOT/B_:e5_1:Z/AG:f6:Z/C:h8:AGL", 4));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "Z:g7:ROOT/B_:e5_1:Z/AG:f6:Z/C:h8:AGL", 4));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "Z:g7:ROOT/B_:e5_1:Z/AG:f6:Z/C:h8:AGL", 4));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // test move constructor for trees with custom null value

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryInsertNode("E", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("D", "newval");

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "D:newval:ROOT/A_:DF:D/E::D/B_:DF:A_/Z:DF:A_", 5));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    (void)mpSearchTree->tryInsertNode("G", scDefaultNullValue);
    (void)mpAuxSearchTree->tryInsertNode("D", scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "D:NULL:ROOT/A_:DF:D/E:DF:D/B_:DF:A_/Z:DF:A_", 5));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeyAVLTreesTests::testMergeTrees()
{
    mpSearchTree = std::make_unique<StrIntStrAVLTree>();

    (void)mpSearchTree->tryInsertNode("E_", "a1_1");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("G", "f6");
    (void)mpSearchTree->tryInsertNode("BC_", "k11");
    (void)mpSearchTree->tryInsertNode("AG", "l12");
    (void)mpSearchTree->tryInsertNode("AB_", "n14");
    (void)mpSearchTree->tryInsertNode("AF", "i9_1");
    (void)mpSearchTree->tryInsertNode("Z", "g7_1");

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();

    (void)mpAuxSearchTree->tryInsertNode("H", "b2");
    (void)mpAuxSearchTree->tryInsertNode("A_", "c3");
    (void)mpAuxSearchTree->tryInsertNode("B_", "e5");
    (void)mpAuxSearchTree->tryInsertNode("Z", "g7_2");
    (void)mpAuxSearchTree->tryInsertNode("E_", "a1_2");
    (void)mpAuxSearchTree->tryInsertNode("AF", "i9_2");
    (void)mpAuxSearchTree->tryInsertNode("I_", "h8");
    (void)mpAuxSearchTree->tryInsertNode("AD", "j10");
    (void)mpAuxSearchTree->tryInsertNode("AF_", "m13");

    const StrIntStrAVLTree searchTreeCopy{*mpSearchTree};
    const StrIntStrAVLTree searchTreeAuxCopy{*mpAuxSearchTree};

    QVERIFY(searchTreeCopy == *mpSearchTree &&      // just a(n additional) check that the copy constructor and == operator work correctly
            searchTreeAuxCopy == *mpAuxSearchTree);

    // first (main) merge
    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/E_:a1_2:B/AF:i9_2:B/AB_:n14:E_/A_:c3:E_/H:b2:AF/AG:l12:AF/BC_:k11:AB_/I_:h8:AB_/B_:e5:A_/Z:g7_2:A_/G:f6:H/AD:j10:H/AF_:m13:BC_R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    const StrIntStrAVLTree mainTreeAfterFirstMerge{*mpSearchTree};

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
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/AB_:n14:A_/H:b2:A_/AF_:m13:AB_/E_:a1_1:AB_/B:d4:H/AF:i9_1:H/BC_:k11:AF_L/I_:h8:E_/B_:e5:E_/Z:g7_1:B/G:f6:B/AD:j10:AF/AG:l12:AF", 14));
    QVERIFY(*mpAuxSearchTree != mainTreeAfterFirstMerge); // test the != operator too
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // merge trees with (same) custom null value

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);

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

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    (void)mpAuxSearchTree->tryInsertNode("I_", scDefaultNullValue);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/I_::A_/AD:DF:A_/AF_:DF:I_/E_:a1:I_/G:DF:AD/AF:DF:AD/BC_:DF:AF_/AB_:DF:AF_/G_:i9:E_/B_::E_/Z:g7:G/H:b2:G/AG:DF:AFR", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. default)

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E", "AZ", "B_", "I", "D", "B", "G", "H_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_", "A"}, scDefaultValue, scDefaultNullValue);
    mpSearchTree->tryInsertNode("I", "abc");
    mpAuxSearchTree->tryInsertNode("G", "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E:DF:ROOT/B:DF:E/I:abc:E/B_:DF:B/D:DF:B/G:DF:I/AZ:DF:I/H_:DF:B_L", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT/I_:DF:AD/AF:DF:AD/AF_:DF:I_/G:xyz:I_/AG:DF:AFR/BC_:DF:AF_/AB_:DF:AF_/A:DF:GL", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT/I_:DF:AD/AF:DF:AD/AF_:DF:I_/G:xyz:I_/AG:DF:AFR/BC_:DF:AF_/AB_:DF:AF_/A:DF:GL", 9));

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

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"E", "AZ", "B_", "I", "D", "B", "G", "H_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_", "A"}, scDefaultValue, scCustomNullValue + "1");
    mpSearchTree->tryInsertNode("I", "abc");
    mpAuxSearchTree->tryInsertNode("G", "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E:DF:ROOT/B:DF:E/I:abc:E/B_:DF:B/D:DF:B/G:DF:I/AZ:DF:I/H_:DF:B_L", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT/I_:DF:AD/AF:DF:AD/AF_:DF:I_/G:xyz:I_/AG:DF:AFR/BC_:DF:AF_/AB_:DF:AF_/A:DF:GL", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT/I_:DF:AD/AF:DF:AD/AF_:DF:I_/G:xyz:I_/AG:DF:AFR/BC_:DF:AF_/AB_:DF:AF_/A:DF:GL", 9));

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

void StrIntKeyAVLTreesTests::testInOrderForwardIterators()
{
    mpSearchTree = std::make_unique<StrIntStrAVLTree>();

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(),
                                     "A_:_ca:ROOT/I_:DF:A_/G:a:A_/AF_:qa:I_/E_:b:I_/B:q1:G/AD:abab:G/BC_:-c:AF_/AB_:dev:AF_/"
                                     "B_:55:E_R/Z:fq:BL/H:z:AD/AG:b:AD/AE_:DF:AB_L/AF:cCc:AG/AI:_ca:AG",
                                     16));

    StrIntStrAVLIterator it{mpSearchTree->begin()};
    QVERIFY(it.getKey() == "BC_" && it.getValue() == "-c");

    it = mpSearchTree->root();
    QVERIFY(it.getKey() == "A_" && it.getValue() == "_ca");

    it = mpSearchTree->find("B_");
    QVERIFY(it.getKey() == "B_" && it.getValue() == "55");

    QVERIFY(mpSearchTree->find("BC_") == mpSearchTree->begin());
    QVERIFY(mpSearchTree->find("A_") == mpSearchTree->root());
    QVERIFY(mpSearchTree->find("AB") == mpSearchTree->end());

    std::vector<std::pair<std::optional<StringifiedInteger>, std::string>> traversedElements;
    const std::vector<std::pair<std::optional<StringifiedInteger>, std::string>> c_TraversedElementsRef{{"BC_", "-c"}, {"AF_", "qa"}, {"AE_", "DF"}, {"AB_", "dev"}, {"I_", "DF"}, {"E_", "b"}, {"B_", "55"}, {"A_", "_ca"},
                                                                          {"Z", "fq"}, {"B", "q1"}, {"G", "a"}, {"H", "z"}, {"AD", "abab"}, {"AF", "cCc"}, {"AG", "b"}, {"AI", "_ca"}};

    for (StrIntStrAVLIterator it{mpSearchTree->begin()}; it != mpSearchTree->end(); it.next())
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

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>("NullVal");
    QVERIFY(mpAuxSearchTree->begin() == mpAuxSearchTree->end() && mpAuxSearchTree->root() == mpAuxSearchTree->end() && mpAuxSearchTree->find("AD") == mpAuxSearchTree->end());

    StrIntStrAVLIterator itAux;
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue().empty() && itAux == mpSearchTree->end() && itAux != mpAuxSearchTree->end());

    itAux = mpAuxSearchTree->end();
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue() == "NullVal");
}

QTEST_APPLESS_MAIN(StrIntKeyAVLTreesTests)

#include "tst_strintkeyavltreestests.moc"
// clang-format on
