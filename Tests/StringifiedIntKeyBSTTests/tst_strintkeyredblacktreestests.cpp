// clang-format off
#include <QTest>
#include <algorithm>

#include "testutils.h"
#include "redblacktree.h"
#include "stringifiedinteger.h"

using namespace TestUtils;
using StrIntStrRedBlackTree = RedBlackTree<StringifiedInteger, std::string>;
using StrIntStrRBTIterator = StrIntStrRedBlackTree::InOrderForwardIterator;
using upStrIntStrRedBlackTree = std::unique_ptr<StrIntStrRedBlackTree>;

class StrIntKeyRedBlackTreesTests : public QObject
{
    Q_OBJECT

public:
    StrIntKeyRedBlackTreesTests();

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
    upStrIntStrRedBlackTree mpSearchTree;
    upStrIntStrRedBlackTree mpAuxSearchTree;
};

StrIntKeyRedBlackTreesTests::StrIntKeyRedBlackTreesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

void StrIntKeyRedBlackTreesTests::init()
{
    QVERIFY(!mpSearchTree && !mpAuxSearchTree);
}

void StrIntKeyRedBlackTreesTests::cleanup()
{
    mpSearchTree.reset();
    mpAuxSearchTree.reset();
}

void StrIntKeyRedBlackTreesTests::testAddNodes()
{
    bool nodeInserted{false};

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeInserted = mpSearchTree->tryInsertNode("E_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT:BK", 1));

    nodeInserted = mpSearchTree->tryInsertNode("H", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT:BK/H:E_R:RD", 2));

    nodeInserted = mpSearchTree->tryInsertNode("A_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:RD/H:A_:RD", 3));

    nodeInserted = mpSearchTree->tryInsertNode("B", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/B:HL:RD", 4));

    nodeInserted = mpSearchTree->tryInsertNode("B_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/B_:E_R:RD/B:HL:RD", 5));

    nodeInserted = mpSearchTree->tryInsertNode("G", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/B_:E_R:RD/B:G:RD/H:G:RD", 6));

    nodeInserted = mpSearchTree->tryInsertNode("Z", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/B_:E_R:RD/B:G:BK/H:G:BK/Z:BL:RD", 7));

    nodeInserted = mpSearchTree->tryInsertNode("I_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/I_:E_:RD/B_:E_:RD/B:G:BK/H:G:BK/Z:BL:RD", 8));

    nodeInserted = mpSearchTree->tryInsertNode("AF", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/I_:E_:RD/B_:E_:RD/B:G:BK/H:G:BK/Z:BL:RD/AF:HR:RD", 9));

    nodeInserted = mpSearchTree->tryInsertNode("AD", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/I_:E_:RD/B_:E_:RD/B:G:BK/AD:G:BK/Z:BL:RD/H:AD:RD/AF:AD:RD", 10));

    nodeInserted = mpSearchTree->tryInsertNode("BC_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:RD/G:A_:RD/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:BK/BC_:I_L:RD/Z:BL:RD/H:AD:RD/AF:AD:RD", 11));

    nodeInserted = mpSearchTree->tryInsertNode("AG", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/BC_:I_L:RD/Z:BL:RD/H:AD:BK/AF:AD:BK/AG:AFR:RD", 12));

    nodeInserted = mpSearchTree->tryInsertNode("AF_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:RD/I_:AF_:RD/Z:BL:RD/H:AD:BK/AF:AD:BK/AG:AFR:RD", 13));

    nodeInserted = mpSearchTree->tryInsertNode("AB_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/I_:AF_:BK/Z:BL:RD/H:AD:BK/AF:AD:BK/AB_:I_L:RD/AG:AFR:RD", 14));

    nodeInserted = mpSearchTree->tryInsertNode("AI", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(),"A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/I_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/AB_:I_L:RD/AF:AG:RD/AI:AG:RD", 15));

    nodeInserted = mpSearchTree->tryInsertNode("AE_", scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/AB_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/AE_:AB_:RD/I_:AB_:RD/AF:AG:RD/AI:AG:RD", 16));

    // inserting nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue, scCustomNullValue);

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

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/AD:A_:BK/AF_:E_:RD/B_:E_:BK/G:AD:RD/AG:AD:RD/BC_:AF_:BK/AB_:AF_:BK/B:G:BK/H:G:BK/AF:AG:BK/AI:AG:BK/AE_:AB_:RD/I_:AB_:RD/Z:BL:RD/BE:AIR:RD", 17));

    // some additional (corner) cases
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"BC_", "AF_", "AE_", "AB_", "I_", "E_", "B_", "A_", "Z", "B", "G", "H", "AD", "AF", "AG", "AI"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AF_:AB_:BK/A_:AB_:RD/BC_:AF_:BK/AE_:AF_:BK/E_:A_:BK/H:A_:BK/I_:E_:BK/B_:E_:BK/B:H:RD/AF:H:RD/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 16));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AI", "AG", "AF", "AD", "H", "G", "B", "Z", "A_", "B_", "E_", "I_", "AB_", "AE_", "AF_", "BC_"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT:BK/Z:AD:RD/AG:AD:BK/I_:Z:BK/G:Z:BK/AF:AG:BK/AI:AG:BK/AE_:I_:RD/B_:I_:RD/B:G:BK/H:G:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/BC_:AF_L:RD", 16));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"BC_", "AI", "AF_", "AG", "AE_", "AF", "AB_", "AD", "I_", "H", "E_", "G", "B_", "B", "A_", "Z"}, scDefaultValue, scCustomNullValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AF_:AB_:BK/AD:AB_:RD/BC_:AF_:BK/AE_:AF_:BK/A_:AD:BK/AG:AD:BK/E_:A_:RD/G:A_:RD/AF:AG:BK/AI:AG:BK/I_:E_:BK/B_:E_:BK/B:G:BK/H:G:BK/Z:BL:RD", 16));

    mpSearchTree->clear();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "B", "D", "Z", "A"}, scDefaultValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "C_", "B", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree); // in this particular case due to RB tree construction rules the trees become equal when third element is being inserted (unlike the basic BST)

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "B", "C_", "D", "A", "Z"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"C_", "E_", "B", "D", "Z", "A"}, scDefaultValue, scDefaultNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"C_", "B", "E_", "D", "Z", "A"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{}, scDefaultValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    // small test including the "extreme" key values: N_ and N
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "N_", "AG", "AF_", "AB_", "N", "AE_"}, scDefaultValue);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/N_:AF_:BK/AB_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/AE_:AB_:RD/I_:AB_:RD/AF:AG:RD/N:AG:RD", 16));
}

void StrIntKeyRedBlackTreesTests::testRemoveNodes()
{
    bool nodeRemoved{false};

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(*mpSearchTree);

    nodeRemoved = mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AE_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AE_:BK/AB_:AE_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/I_:AB_R:RD/AF:AG:RD/AI:AG:RD", 15));

    nodeRemoved = mpSearchTree->tryRemoveNode("H");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AE_:E_:RD/B_:E_:BK/B:G:BK/AG:G:RD/BC_:AE_:BK/AB_:AE_:BK/Z:BL:RD/AD:AG:BK/AI:AG:BK/I_:AB_R:RD/AF:ADR:RD", 14));

    nodeRemoved = mpSearchTree->tryRemoveNode("Z");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AE_:E_:RD/B_:E_:BK/B:G:BK/AG:G:RD/BC_:AE_:BK/AB_:AE_:BK/AD:AG:BK/AI:AG:BK/I_:AB_R:RD/AF:ADR:RD", 13));

    nodeRemoved = mpSearchTree->tryRemoveNode("BC_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AB_:E_:RD/B_:E_:BK/B:G:BK/AG:G:RD/AE_:AB_:BK/I_:AB_:BK/AD:AG:BK/AI:AG:BK/AF:ADR:RD", 12));

    nodeRemoved = mpSearchTree->tryRemoveNode("AI");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AB_:E_:RD/B_:E_:BK/B:G:BK/AF:G:BK/AE_:AB_:BK/I_:AB_:BK/AD:AF:RD/AG:AF:RD", 11));

    nodeRemoved = mpSearchTree->tryRemoveNode("AG");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AB_:E_:RD/B_:E_:BK/B:G:BK/AF:G:BK/AE_:AB_:BK/I_:AB_:BK/AD:AFL:RD", 10));

    nodeRemoved = mpSearchTree->tryRemoveNode("E_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/G:A_:BK/AE_:AB_:BK/B_:AB_:BK/B:G:BK/AF:G:BK/I_:B_L:RD/AD:AFL:RD", 9));

    nodeRemoved = mpSearchTree->tryRemoveNode("I_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/G:A_:BK/AE_:AB_:BK/B_:AB_:BK/B:G:BK/AF:G:BK/AD:AFL:RD", 8));

    nodeRemoved = mpSearchTree->tryRemoveNode("AE_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/G:A_:RD/B_:AB_R:RD/B:G:BK/AF:G:BK/AD:AFL:RD", 7));

    nodeRemoved = mpSearchTree->tryRemoveNode("AD");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/G:A_:RD/B_:AB_R:RD/B:G:BK/AF:G:BK", 6));

    nodeRemoved = mpSearchTree->tryRemoveNode("A_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT:BK/AB_:B:BK/G:B:BK/B_:AB_R:RD/AF:GR:RD", 5));

    nodeRemoved = mpSearchTree->tryRemoveNode("B_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT:BK/AB_:B:BK/G:B:BK/AF:GR:RD", 4));

    nodeRemoved = mpSearchTree->tryRemoveNode("AB_");
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT:BK/B:G:BK/AF:G:BK", 3));

    nodeRemoved = mpSearchTree->tryRemoveNode("AF"); // root and two black children, erase right child
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT:BK/B:GL:RD", 2));

    nodeRemoved = mpSearchTree->tryRemoveNode("B"); // root and left child, erase left child
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT:BK", 1));

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
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/AB_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/I_:AB_R:RD/AF:AG:RD/AI:AG:RD", 15));

    (void)mpAuxSearchTree->tryRemoveNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/AB_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/I_:AB_R:RD/AF:AGL:RD", 14));

    (void)mpAuxSearchTree->tryRemoveNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/I_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/AF:AGL:RD", 13));

    (void)mpAuxSearchTree->tryRemoveNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/BC_:I_L:RD/Z:BL:RD/H:AD:BK/AG:AD:BK/AF:AGL:RD", 12));

    (void)mpAuxSearchTree->tryRemoveNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/BC_:I_L:RD/Z:BL:RD/H:AD:BK/AF:AD:BK", 11));

    (void)mpAuxSearchTree->tryRemoveNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/Z:BL:RD/H:AD:BK/AF:AD:BK", 10));

    (void)mpAuxSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AF:G:BK/Z:BL:RD/H:AFL:RD", 9));

    (void)mpAuxSearchTree->tryRemoveNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/H:G:BK/Z:BL:RD", 8));

    (void)mpAuxSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/B_:E_R:RD/B:G:BK/H:G:BK/Z:BL:RD", 7));

    (void)mpAuxSearchTree->tryRemoveNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/B_:E_R:RD/B:G:BK/H:G:BK", 6));

    (void)mpAuxSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/B_:E_R:RD/B:HL:RD", 5));

    (void)mpAuxSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/B:HL:RD", 4));

    (void)mpAuxSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK", 3));

    (void)mpAuxSearchTree->tryRemoveNode("A_"); // root and two black children, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "H:ROOT:BK/E_:HL:RD", 2));

    (void)mpAuxSearchTree->tryRemoveNode("H"); // root and left child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "E_:ROOT:BK", 1));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AF_:A_:BK/G:A_:BK/BC_:AF_:BK/AB_:AF_:RD/B:G:BK/AD:G:RD/AE_:AB_:BK/B_:AB_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/I_:B_L:RD/AF:AG:RD/AI:AG:RD", 15));

    (void)mpSearchTree->tryRemoveNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AF_:A_:BK/G:A_:BK/BC_:AF_:BK/AB_:AF_:RD/B:G:BK/AG:G:RD/AE_:AB_:BK/B_:AB_:BK/Z:BL:RD/AD:AG:BK/AI:AG:BK/I_:B_L:RD/AF:ADR:RD", 14));

    (void)mpSearchTree->tryRemoveNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT:BK/AF_:Z:BK/G:Z:BK/BC_:AF_:BK/AB_:AF_:RD/B:G:BK/AG:G:RD/AE_:AB_:BK/B_:AB_:BK/AD:AG:BK/AI:AG:BK/I_:B_L:RD/AF:ADR:RD", 13));

    (void)mpSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT:BK/AF_:Z:BK/AG:Z:BK/BC_:AF_:BK/AB_:AF_:RD/AD:AG:RD/AI:AG:BK/AE_:AB_:BK/B_:AB_:BK/G:AD:BK/AF:AD:BK/I_:B_L:RD", 12));

    (void)mpSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT:BK/AF_:Z:BK/AG:Z:BK/BC_:AF_:BK/AB_:AF_:RD/AD:AG:RD/AI:AG:BK/AE_:AB_:BK/I_:AB_:BK/G:AD:BK/AF:AD:BK", 11));

    (void)mpSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT:BK/AF_:Z:BK/AG:Z:BK/BC_:AF_:BK/AB_:AF_:RD/AD:AG:BK/AI:AG:BK/AE_:AB_:BK/I_:AB_:BK/AF:ADR:RD", 10));

    (void)mpSearchTree->tryRemoveNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT:BK/AF_:AD:BK/AG:AD:BK/BC_:AF_:BK/AB_:AF_:RD/AF:AG:BK/AI:AG:BK/AE_:AB_:BK/I_:AB_:BK", 9));

    (void)mpSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT:BK/AF_:AD:BK/AG:AD:BK/BC_:AF_:BK/AB_:AF_:BK/AF:AG:BK/AI:AG:BK/AE_:AB_L:RD", 8));

    (void)mpSearchTree->tryRemoveNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT:BK/AF_:AD:RD/AG:AD:BK/BC_:AF_:BK/AB_:AF_:BK/AI:AGR:RD/AE_:AB_L:RD", 7));

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT:BK/AF_:AG:RD/AI:AG:BK/BC_:AF_:BK/AB_:AF_:BK/AE_:AB_L:RD", 6));

    (void)mpSearchTree->tryRemoveNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT:BK/AE_:AG:BK/AI:AG:BK/AF_:AE_:RD/AB_:AE_:RD", 5));

    (void)mpSearchTree->tryRemoveNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AE_:ROOT:BK/AF_:AE_:BK/AI:AE_:BK/AB_:AIL:RD", 4));

    (void)mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AE_:AB_:BK/AI:AB_:BK", 3));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"BC_", "AF_", "AE_", "AB_", "I_", "E_", "B_", "A_", "Z", "B", "G", "H", "AD", "AF", "AG", "AI"}, scDefaultValue);

    (void)mpSearchTree->tryRemoveNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/H:A_:BK/AF_:AB_:BK/E_:AB_:RD/B:H:RD/AF:H:RD/AE_:AF_R:RD/I_:E_:BK/B_:E_:BK/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 15));

    (void)mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/H:A_:BK/AE_:AB_:BK/E_:AB_:RD/B:H:RD/AF:H:RD/I_:E_:BK/B_:E_:BK/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 14));

    (void)mpSearchTree->tryRemoveNode("AE_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/AB_:E_:BK/B_:E_:BK/B:H:RD/AF:H:RD/I_:AB_R:RD/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 13));

    (void)mpSearchTree->tryRemoveNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/I_:E_:BK/B_:E_:BK/B:H:RD/AF:H:RD/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 12));

    (void)mpSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/A_:H:BK/AF:H:BK/E_:A_:BK/B:A_:RD/AD:AF:BK/AG:AF:BK/B_:E_R:RD/Z:B:BK/G:B:BK/AI:AGR:RD", 11));

    (void)mpSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/A_:H:BK/AF:H:BK/B_:A_:BK/B:A_:RD/AD:AF:BK/AG:AF:BK/Z:B:BK/G:B:BK/AI:AGR:RD", 10));

    (void)mpSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/B:H:BK/AF:H:BK/A_:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/Z:A_R:RD/AI:AGR:RD", 9));

    (void)mpSearchTree->tryRemoveNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/B:H:BK/AF:H:BK/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 8));

    (void)mpSearchTree->tryRemoveNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/B:H:BK/AF:H:RD/G:BR:RD/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 7));

    (void)mpSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/G:H:BK/AF:H:RD/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 6));

    (void)mpSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AF:ROOT:BK/H:AF:BK/AG:AF:BK/AD:HR:RD/AI:AGR:RD", 5));

    (void)mpSearchTree->tryRemoveNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AF:ROOT:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 4));

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT:BK/AF:AG:BK/AI:AG:BK", 3));

    (void)mpSearchTree->tryRemoveNode("AF"); // root and two black children, erase left child
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT:BK/AI:AGR:RD", 2));

    (void)mpSearchTree->tryRemoveNode("AG"); // root and right child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AI:ROOT:BK", 1));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AI", "AG", "AF", "AD", "H", "G", "B", "Z", "A_", "B_", "E_", "I_", "AB_", "AE_", "AF_", "BC_"}, scDefaultValue, scCustomNullValue);

    (void)mpAuxSearchTree->tryRemoveNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT:BK/I_:Z:BK/AD:Z:BK/AE_:I_:RD/B_:I_:RD/G:AD:RD/AG:AD:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/B:G:BK/H:G:BK/AF:AGL:RD/BC_:AF_L:RD", 15));

    (void)mpAuxSearchTree->tryRemoveNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT:BK/I_:Z:BK/AD:Z:BK/AE_:I_:RD/B_:I_:RD/G:AD:RD/AF:AD:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/B:G:BK/H:G:BK/BC_:AF_L:RD", 14));

    (void)mpAuxSearchTree->tryRemoveNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT:BK/I_:Z:BK/G:Z:BK/AE_:I_:RD/B_:I_:RD/B:G:BK/AD:G:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/H:ADL:RD/BC_:AF_L:RD", 13));

    (void)mpAuxSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT:BK/I_:Z:BK/G:Z:BK/AE_:I_:RD/B_:I_:RD/B:G:BK/H:G:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/BC_:AF_L:RD", 12));

    (void)mpAuxSearchTree->tryRemoveNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:BK/Z:I_:BK/AF_:AE_:BK/AB_:AE_:BK/B_:Z:RD/G:Z:BK/BC_:AF_L:RD/E_:B_:BK/A_:B_:BK/B:GL:RD", 11));

    (void)mpAuxSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:BK/Z:I_:BK/AF_:AE_:BK/AB_:AE_:BK/B_:Z:RD/B:Z:BK/BC_:AF_L:RD/E_:B_:BK/A_:B_:BK", 10));

    (void)mpAuxSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:BK/B_:I_:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/Z:B_:BK/BC_:AF_L:RD/A_:ZL:RD", 9));

    (void)mpAuxSearchTree->tryRemoveNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:BK/B_:I_:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/BC_:AF_L:RD", 8));

    (void)mpAuxSearchTree->tryRemoveNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:RD/B_:I_:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_L:RD/BC_:AF_L:RD", 7));

    (void)mpAuxSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:RD/E_:I_:BK/AF_:AE_:BK/AB_:AE_:BK/BC_:AF_L:RD", 6));

    (void)mpAuxSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AE_:ROOT:BK/AF_:AE_:BK/I_:AE_:BK/BC_:AF_L:RD/AB_:I_L:RD", 5));

    (void)mpAuxSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AE_:ROOT:BK/AF_:AE_:BK/AB_:AE_:BK/BC_:AF_L:RD", 4));

    (void)mpAuxSearchTree->tryRemoveNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AF_:ROOT:BK/BC_:AF_:BK/AE_:AF_:BK", 3));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"BC_", "AI", "AF_", "AG", "AE_", "AF", "AB_", "AD", "I_", "H", "E_", "G", "B_", "B", "A_", "Z"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AF_:AB_:BK/A_:AB_:RD/BC_:AF_:BK/AE_:AF_:BK/E_:A_:BK/AF:A_:BK/I_:E_:BK/B_:E_:BK/G:AF:RD/AG:AF:BK/B:G:BK/H:G:BK/AI:AGR:RD/Z:BL:RD", 15));

    (void)mpSearchTree->tryRemoveNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AF_:AB_:BK/G:AB_:BK/BC_:AF_:BK/AE_:AF_:BK/A_:G:RD/AF:G:RD/E_:A_:BK/B:A_:BK/H:AF:BK/AG:AF:BK/B_:E_R:RD/Z:BL:RD/AI:AGR:RD", 14));

    (void)mpSearchTree->tryRemoveNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AF_:AB_:BK/G:AB_:BK/BC_:AF_:BK/AE_:AF_:BK/A_:G:RD/AG:G:RD/E_:A_:BK/B:A_:BK/AF:AG:BK/AI:AG:BK/B_:E_R:RD/Z:BL:RD", 13));

    (void)mpSearchTree->tryRemoveNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT:BK/AF_:E_:BK/G:E_:BK/BC_:AF_:BK/AE_:AF_:BK/A_:G:RD/AG:G:RD/B_:A_:BK/B:A_:BK/AF:AG:BK/AI:AG:BK/Z:BL:RD", 12));

    (void)mpSearchTree->tryRemoveNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT:BK/AF_:E_:BK/G:E_:BK/BC_:AF_:BK/AE_:AF_:BK/A_:G:RD/AG:G:BK/B_:A_:BK/B:A_:BK/AI:AGR:RD/Z:BL:RD", 11));

    (void)mpSearchTree->tryRemoveNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT:BK/AF_:B_:BK/G:B_:BK/BC_:AF_:BK/AE_:AF_:BK/Z:G:BK/AG:G:BK/A_:Z:RD/B:Z:RD/AI:AGR:RD", 10));

    (void)mpSearchTree->tryRemoveNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT:BK/AF_:B_:BK/AG:B_:BK/BC_:AF_:BK/AE_:AF_:BK/Z:AG:BK/AI:AG:BK/A_:Z:RD/B:Z:RD", 9));

    (void)mpSearchTree->tryRemoveNode("AE_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT:BK/AF_:B_:BK/AG:B_:RD/BC_:AF_L:RD/Z:AG:BK/AI:AG:BK/A_:Z:RD/B:Z:RD", 8));

    (void)mpSearchTree->tryRemoveNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT:BK/AF_:B_:BK/Z:B_:RD/BC_:AF_L:RD/A_:Z:BK/AI:Z:BK/B:AIL:RD", 7));

    (void)mpSearchTree->tryRemoveNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AF_:A_:BK/B:A_:BK/BC_:AF_L:RD/Z:B:RD/AI:B:RD", 6));

    (void)mpSearchTree->tryRemoveNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AF_:A_:BK/AI:A_:BK/BC_:AF_L:RD/Z:AIL:RD", 5));

    (void)mpSearchTree->tryRemoveNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/BC_:A_:BK/AI:A_:BK/Z:AIL:RD", 4));

    (void)mpSearchTree->tryRemoveNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/BC_:A_:BK/Z:A_:BK", 3));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"C_", "E_", "B"}, scDefaultValue);

    (void)mpAuxSearchTree->tryRemoveNode("E_"); // root and two red children, erase left child
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "C_:ROOT:BK/B:C_R:RD", 2));

    (void)mpAuxSearchTree->tryRemoveNode("B"); // root and right child, erase right child
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "C_:ROOT:BK", 1));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"C_", "E_", "B"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryRemoveNode("B"); // root and two red children, erase right child
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "C_:ROOT:BK/E_:C_L:RD", 2));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"C_", "E_", "B"}, scDefaultValue);

    (void)mpAuxSearchTree->tryRemoveNode("C_"); // root and two red children, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "B:ROOT:BK/E_:BL:RD", 2));

    // removing null node from custom null value tree
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue, scCustomNullValue);

    nodeRemoved = mpSearchTree->tryRemoveNode("E_");
    QVERIFY(!nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT:BK/A_:B:BK/C:B:BK/B_:A_L:RD/D:CR:RD", 5));

    // removing same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue);

    nodeRemoved = mpSearchTree->tryRemoveNode("C");
    QVERIFY(nodeRemoved &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeRemoved = mpAuxSearchTree->tryRemoveNode("C");
    QVERIFY(nodeRemoved &&
            *mpSearchTree == *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "B:ROOT:BK/A_:B:BK/D:B:BK/B_:A_L:RD", 4));
}

void StrIntKeyRedBlackTreesTests::testUpdateNodeValue()
{
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1:A_:BK/G:f6:A_:BK/AF_:m13:E_:RD/B_:e5:E_:BK/B:d4:G:BK/AD:j10:G:RD/BC_:k11:AF_:BK/I_:h8:AF_:BK/Z:g7:BL:RD/H:b2:AD:BK/AF:i9:AD:BK/AB_:n14:I_L:RD/AG:l12:AFR:RD", 14));

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1:A_:BK/H:b2:A_:BK/AF_:m13:E_:RD/B_:e5:E_:BK/B:d4_1:H:BK/AG:L12:H:RD/BC_:k11:AF_:BK/I_:b2:AF_:BK/Z:g7:BL:RD/AD:j10:AG:BK/BE:o15:AG:BK/AB_:n14:I_L:RD/AF:i9_new:ADR:RD", 14));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue("E_") &&
            scDefaultNullValue == mpSearchTree->getNodeValue("Z") &&
            scDefaultNullValue == mpSearchTree->getNodeValue("AF"));

    // test with same value for all nodes
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_"}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:DF:ROOT:BK/E_:DF:A_:BK/G:DF:A_:BK/AF_:DF:E_:RD/B_:DF:E_:BK/B:DF:G:BK/AD:DF:G:RD/BC_:DF:AF_:BK/I_:DF:AF_:BK/Z:DF:BL:RD/H:DF:AD:BK/AF:DF:AD:BK/AB_:DF:I_L:RD/AG:DF:AFR:RD", 14));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue("E_") &&
            scDefaultValue == mpAuxSearchTree->getNodeValue("Z") &&
            scDefaultValue == mpAuxSearchTree->getNodeValue("AF"));

    // test updating custom null value tree node values

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);

    (void)mpSearchTree->tryInsertNode("E_", "a1");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("A_", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("B_", "e5");
    (void)mpSearchTree->tryInsertNode("G", "f6");
    (void)mpSearchTree->tryInsertNode("H_", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("Z", "g7");
    (void)mpSearchTree->tryInsertNode("I_", "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_::ROOT:BK/E_:a1:A_:RD/G:f6:A_:RD/H_::E_:BK/B_:e5:E_:BK/B:d4:G:BK/H:b2:G:BK/I_:h8:H_L:RD/Z:g7:BL:RD", 9));
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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "Z:g7:ROOT:BK/E_:a1:Z:RD/G:f6:Z:BK/G_:i9:E_:BK/B_::E_:BK/H:b2:GR:RD/I_:h8:G_L:RD/A_::B_R:RD", 8));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(*mpSearchTree);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    nodeInserted = mpAuxSearchTree->tryInsertNode("AG", "df");
    QVERIFY(nodeInserted &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    nodeInserted = mpSearchTree->tryInsertNode("G", scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue("G"));

    nodeInserted = mpAuxSearchTree->tryInsertNode("D", scCustomNullValue);
    QVERIFY(!nodeInserted &&
            scCustomNullValue == mpAuxSearchTree->getNodeValue("D"));

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT:BK/I_:DF:AD:RD/AF:DF:AD:BK/AF_:DF:I_:BK/G::I_:BK/AG:DF:AFR:RD/BC_:DF:AF_:RD/AB_:DF:AF_:RD", 8));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeyRedBlackTreesTests::testMoveSemantics()
{
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    (void)mpSearchTree->tryInsertNode("E_", "a1");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("A_", "c3");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("B_", "e5");

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1:A_:BK/H:b2:A_:BK/B_:e5:E_R:RD/B:d4:HL:RD", 5));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->tryInsertNode("AG", "f6");
    mpSearchTree->tryInsertNode("Z", "g7");
    mpSearchTree->tryInsertNode("B_", "e5_1");
    mpSearchTree->tryInsertNode("C", "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "Z:g7:ROOT:BK/B_:e5_1:Z:BK/AG:f6:Z:BK/C:h8:AGL:RD", 4));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "Z:g7:ROOT:BK/B_:e5_1:Z:BK/AG:f6:Z:BK/C:h8:AGL:RD", 4));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "Z:g7:ROOT:BK/B_:e5_1:Z:BK/AG:f6:Z:BK/C:h8:AGL:RD", 4));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // test move constructor for trees with custom null value

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryInsertNode("E", scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode("D", "newval");

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "D:newval:ROOT:BK/A_:DF:D:BK/E::D:BK/B_:DF:A_:RD/Z:DF:A_:RD", 5));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    (void)mpSearchTree->tryInsertNode("G", scDefaultNullValue);
    (void)mpAuxSearchTree->tryInsertNode("D", scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "D:NULL:ROOT:BK/A_:DF:D:BK/E:DF:D:BK/B_:DF:A_:RD/Z:DF:A_:RD", 5));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeyRedBlackTreesTests::testMergeTrees()
{
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    (void)mpSearchTree->tryInsertNode("E_", "a1_1");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("G", "f6");
    (void)mpSearchTree->tryInsertNode("BC_", "k11");
    (void)mpSearchTree->tryInsertNode("AG", "l12");
    (void)mpSearchTree->tryInsertNode("AB_", "n14");
    (void)mpSearchTree->tryInsertNode("AF", "i9_1");
    (void)mpSearchTree->tryInsertNode("Z", "g7_1");

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    (void)mpAuxSearchTree->tryInsertNode("H", "b2");
    (void)mpAuxSearchTree->tryInsertNode("A_", "c3");
    (void)mpAuxSearchTree->tryInsertNode("B_", "e5");
    (void)mpAuxSearchTree->tryInsertNode("Z", "g7_2");
    (void)mpAuxSearchTree->tryInsertNode("E_", "a1_2");
    (void)mpAuxSearchTree->tryInsertNode("AF", "i9_2");
    (void)mpAuxSearchTree->tryInsertNode("I_", "h8");
    (void)mpAuxSearchTree->tryInsertNode("AD", "j10");
    (void)mpAuxSearchTree->tryInsertNode("AF_", "m13");

    const StrIntStrRedBlackTree searchTreeCopy{*mpSearchTree};
    const StrIntStrRedBlackTree searchTreeAuxCopy{*mpAuxSearchTree};

    QVERIFY(searchTreeCopy == *mpSearchTree &&      // just a(n additional) check that the copy constructor and == operator work correctly
            searchTreeAuxCopy == *mpAuxSearchTree);

    // first (main) merge
    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:BK/AF:i9_2:B:BK/BC_:k11:AB_:BK/A_:c3:AB_:RD/H:b2:AF:BK/AG:l12:AF:BK/AF_:m13:BC_R:RD/E_:a1_2:A_:BK/Z:g7_2:A_:BK/G:f6:H:RD/AD:j10:H:RD/I_:h8:E_:RD/B_:e5:E_:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    const StrIntStrRedBlackTree mainTreeAfterFirstMerge{*mpSearchTree};

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
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_1:A_:BK/H:b2:A_:BK/AB_:n14:E_:RD/B_:e5:E_:BK/B:d4:H:BK/AF:i9_1:H:BK/AF_:m13:AB_:BK/I_:h8:AB_:BK/Z:g7_1:B:RD/G:f6:B:RD/AD:j10:AF:RD/AG:l12:AF:RD/BC_:k11:AF_L:RD", 14));
    QVERIFY(*mpAuxSearchTree != mainTreeAfterFirstMerge); // test the != operator too
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // merge trees with (same) custom null value

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);

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

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    (void)mpAuxSearchTree->tryInsertNode("I_", scDefaultNullValue);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1:A_:BK/G:DF:A_:BK/I_::E_:RD/B_::E_:BK/Z:g7:G:BK/AD:DF:G:RD/AF_:DF:I_:BK/G_:i9:I_:BK/H:b2:AD:BK/AF:DF:AD:BK/BC_:DF:AF_:RD/AB_:DF:AF_:RD/AG:DF:AFR:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. default)

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E", "AZ", "B_", "I", "D", "B", "G", "H_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_", "A"}, scDefaultValue, scDefaultNullValue);
    mpSearchTree->tryInsertNode("I", "abc");
    mpAuxSearchTree->tryInsertNode("G", "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E:DF:ROOT:BK/B:DF:E:RD/I:abc:E:BK/B_:DF:B:BK/D:DF:B:BK/G:DF:I:RD/AZ:DF:I:RD/H_:DF:B_L:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT:BK/I_:DF:AD:RD/AF:DF:AD:BK/AF_:DF:I_:BK/G:xyz:I_:BK/AG:DF:AFR:RD/BC_:DF:AF_:RD/AB_:DF:AF_:RD/A:DF:GL:RD", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT:BK/I_:DF:AD:RD/AF:DF:AD:BK/AF_:DF:I_:BK/G:xyz:I_:BK/AG:DF:AFR:RD/BC_:DF:AF_:RD/AB_:DF:AF_:RD/A:DF:GL:RD", 9));

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

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E", "AZ", "B_", "I", "D", "B", "G", "H_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_", "A"}, scDefaultValue, scCustomNullValue + "1");
    mpSearchTree->tryInsertNode("I", "abc");
    mpAuxSearchTree->tryInsertNode("G", "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E:DF:ROOT:BK/B:DF:E:RD/I:abc:E:BK/B_:DF:B:BK/D:DF:B:BK/G:DF:I:RD/AZ:DF:I:RD/H_:DF:B_L:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT:BK/I_:DF:AD:RD/AF:DF:AD:BK/AF_:DF:I_:BK/G:xyz:I_:BK/AG:DF:AFR:RD/BC_:DF:AF_:RD/AB_:DF:AF_:RD/A:DF:GL:RD", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "AD:DF:ROOT:BK/I_:DF:AD:RD/AF:DF:AD:BK/AF_:DF:I_:BK/G:xyz:I_:BK/AG:DF:AFR:RD/BC_:DF:AF_:RD/AB_:DF:AF_:RD/A:DF:GL:RD", 9));

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

void StrIntKeyRedBlackTreesTests::testInOrderForwardIterators()
{
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();

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
                                     "A_:_ca:ROOT:BK/E_:b:A_:BK/G:a:A_:BK/AF_:qa:E_:RD/B_:55:E_:BK/B:q1:G:BK/AD:abab:G:RD/"
                                     "BC_:-c:AF_:BK/AB_:dev:AF_:BK/Z:fq:BL:RD/H:z:AD:BK/AG:b:AD:BK/AE_:DF:AB_:RD/I_:DF:AB_:RD/AF:cCc:AG:RD/AI:_ca:AG:RD",
                                     16));

    StrIntStrRBTIterator it{mpSearchTree->begin()};
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

    for (StrIntStrRBTIterator it{mpSearchTree->begin()}; it != mpSearchTree->end(); it.next())
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

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>("NullVal");
    QVERIFY(mpAuxSearchTree->begin() == mpAuxSearchTree->end() && mpAuxSearchTree->root() == mpAuxSearchTree->end() && mpAuxSearchTree->find("AD") == mpAuxSearchTree->end());

    StrIntStrRBTIterator itAux;
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue().empty() && itAux == mpSearchTree->end() && itAux != mpAuxSearchTree->end());

    itAux = mpAuxSearchTree->end();
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue() == "NullVal");
}

void StrIntKeyRedBlackTreesTests::testPrintTree()
{
    qInfo("Creating new tree");
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue);

    QVERIFY(16 == mpSearchTree->getSize());

    qInfo("The tree nodes are: ");
    mpSearchTree->printTree();

    qInfo("Clearing tree content");
    mpSearchTree->clear();

    QVERIFY(0 == mpSearchTree->getSize());

    mpSearchTree->printTree();
    qInfo("The red-black tree has no nodes");
}

QTEST_APPLESS_MAIN(StrIntKeyRedBlackTreesTests)

#include "tst_strintkeyredblacktreestests.moc"
// clang-format on
