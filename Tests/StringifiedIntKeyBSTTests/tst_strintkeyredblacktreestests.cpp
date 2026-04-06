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
    bool newNodeAdded{false};

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode("E_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT:BK", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode("H", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT:BK/H:E_R:RD", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode("A_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:RD/H:A_:RD", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode("B", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/B:HL:RD", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode("B_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/B_:E_R:RD/B:HL:RD", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode("G", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/B_:E_R:RD/B:G:RD/H:G:RD", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode("Z", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/B_:E_R:RD/B:G:BK/H:G:BK/Z:BL:RD", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode("I_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/I_:E_:RD/B_:E_:RD/B:G:BK/H:G:BK/Z:BL:RD", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AF", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/I_:E_:RD/B_:E_:RD/B:G:BK/H:G:BK/Z:BL:RD/AF:HR:RD", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AD", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/I_:E_:RD/B_:E_:RD/B:G:BK/AD:G:BK/Z:BL:RD/H:AD:RD/AF:AD:RD", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode("BC_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:RD/G:A_:RD/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:BK/BC_:I_L:RD/Z:BL:RD/H:AD:RD/AF:AD:RD", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AG", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/BC_:I_L:RD/Z:BL:RD/H:AD:BK/AF:AD:BK/AG:AFR:RD", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AF_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:RD/I_:AF_:RD/Z:BL:RD/H:AD:BK/AF:AD:BK/AG:AFR:RD", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AB_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/I_:AF_:BK/Z:BL:RD/H:AD:BK/AF:AD:BK/AB_:I_L:RD/AG:AFR:RD", 14));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AI", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(),"A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/I_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/AB_:I_L:RD/AF:AG:RD/AI:AG:RD", 15));

    newNodeAdded = mpSearchTree->addOrUpdateNode("AE_", scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/AB_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/AE_:AB_:RD/I_:AB_:RD/AF:AG:RD/AI:AG:RD", 16));

    // adding nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue, scCustomNullValue);

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

    QVERIFY(*mpSearchTree == *mpAuxSearchTree); // in this particular case due to RB tree construction rules the trees become equal when third element is being added (unlike the basic BST)

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
}

void StrIntKeyRedBlackTreesTests::testRemoveNodes()
{
    bool nodeDeleted{false};

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(*mpSearchTree);

    nodeDeleted = mpSearchTree->removeNode("AF_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AE_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AE_:BK/AB_:AE_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/I_:AB_R:RD/AF:AG:RD/AI:AG:RD", 15));

    nodeDeleted = mpSearchTree->removeNode("H");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AE_:E_:RD/B_:E_:BK/B:G:BK/AG:G:RD/BC_:AE_:BK/AB_:AE_:BK/Z:BL:RD/AD:AG:BK/AI:AG:BK/I_:AB_R:RD/AF:ADR:RD", 14));

    nodeDeleted = mpSearchTree->removeNode("Z");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AE_:E_:RD/B_:E_:BK/B:G:BK/AG:G:RD/BC_:AE_:BK/AB_:AE_:BK/AD:AG:BK/AI:AG:BK/I_:AB_R:RD/AF:ADR:RD", 13));

    nodeDeleted = mpSearchTree->removeNode("BC_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AB_:E_:RD/B_:E_:BK/B:G:BK/AG:G:RD/AE_:AB_:BK/I_:AB_:BK/AD:AG:BK/AI:AG:BK/AF:ADR:RD", 12));

    nodeDeleted = mpSearchTree->removeNode("AI");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AB_:E_:RD/B_:E_:BK/B:G:BK/AF:G:BK/AE_:AB_:BK/I_:AB_:BK/AD:AF:RD/AG:AF:RD", 11));

    nodeDeleted = mpSearchTree->removeNode("AG");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AB_:E_:RD/B_:E_:BK/B:G:BK/AF:G:BK/AE_:AB_:BK/I_:AB_:BK/AD:AFL:RD", 10));

    nodeDeleted = mpSearchTree->removeNode("E_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/G:A_:BK/AE_:AB_:BK/B_:AB_:BK/B:G:BK/AF:G:BK/I_:B_L:RD/AD:AFL:RD", 9));

    nodeDeleted = mpSearchTree->removeNode("I_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/G:A_:BK/AE_:AB_:BK/B_:AB_:BK/B:G:BK/AF:G:BK/AD:AFL:RD", 8));

    nodeDeleted = mpSearchTree->removeNode("AE_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/G:A_:RD/B_:AB_R:RD/B:G:BK/AF:G:BK/AD:AFL:RD", 7));

    nodeDeleted = mpSearchTree->removeNode("AD");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/G:A_:RD/B_:AB_R:RD/B:G:BK/AF:G:BK", 6));

    nodeDeleted = mpSearchTree->removeNode("A_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT:BK/AB_:B:BK/G:B:BK/B_:AB_R:RD/AF:GR:RD", 5));

    nodeDeleted = mpSearchTree->removeNode("B_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT:BK/AB_:B:BK/G:B:BK/AF:GR:RD", 4));

    nodeDeleted = mpSearchTree->removeNode("AB_");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT:BK/B:G:BK/AF:G:BK", 3));

    nodeDeleted = mpSearchTree->removeNode("AF"); // root and two black children, erase right child
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT:BK/B:GL:RD", 2));

    nodeDeleted = mpSearchTree->removeNode("B"); // root and left child, erase left child
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "G:ROOT:BK", 1));

    nodeDeleted = mpSearchTree->removeNode("G");
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->removeNode("G");
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->removeNode("BE");
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // we will stop checking the nodeDeleted variable (obvious enough)
    (void)mpAuxSearchTree->removeNode("AE_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/AB_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/I_:AB_R:RD/AF:AG:RD/AI:AG:RD", 15));

    (void)mpAuxSearchTree->removeNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/AB_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/I_:AB_R:RD/AF:AGL:RD", 14));

    (void)mpAuxSearchTree->removeNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/AF_:E_:RD/B_:E_:BK/B:G:BK/AD:G:RD/BC_:AF_:BK/I_:AF_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/AF:AGL:RD", 13));

    (void)mpAuxSearchTree->removeNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/BC_:I_L:RD/Z:BL:RD/H:AD:BK/AG:AD:BK/AF:AGL:RD", 12));

    (void)mpAuxSearchTree->removeNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/BC_:I_L:RD/Z:BL:RD/H:AD:BK/AF:AD:BK", 11));

    (void)mpAuxSearchTree->removeNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AD:G:RD/Z:BL:RD/H:AD:BK/AF:AD:BK", 10));

    (void)mpAuxSearchTree->removeNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/AF:G:BK/Z:BL:RD/H:AFL:RD", 9));

    (void)mpAuxSearchTree->removeNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:BK/I_:E_:BK/B_:E_:BK/B:G:BK/H:G:BK/Z:BL:RD", 8));

    (void)mpAuxSearchTree->removeNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/B_:E_R:RD/B:G:BK/H:G:BK/Z:BL:RD", 7));

    (void)mpAuxSearchTree->removeNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/G:A_:RD/B_:E_R:RD/B:G:BK/H:G:BK", 6));

    (void)mpAuxSearchTree->removeNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/B_:E_R:RD/B:HL:RD", 5));

    (void)mpAuxSearchTree->removeNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/B:HL:RD", 4));

    (void)mpAuxSearchTree->removeNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK", 3));

    (void)mpAuxSearchTree->removeNode("A_"); // root and two black children, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "H:ROOT:BK/E_:HL:RD", 2));

    (void)mpAuxSearchTree->removeNode("H"); // root and left child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "E_:ROOT:BK", 1));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E_", "H", "A_", "B", "B_", "G", "Z", "I_", "AF", "AD", "BC_", "AG", "AF_", "AB_", "AI", "AE_"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->removeNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AF_:A_:BK/G:A_:BK/BC_:AF_:BK/AB_:AF_:RD/B:G:BK/AD:G:RD/AE_:AB_:BK/B_:AB_:BK/Z:BL:RD/H:AD:BK/AG:AD:BK/I_:B_L:RD/AF:AG:RD/AI:AG:RD", 15));

    (void)mpSearchTree->removeNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AF_:A_:BK/G:A_:BK/BC_:AF_:BK/AB_:AF_:RD/B:G:BK/AG:G:RD/AE_:AB_:BK/B_:AB_:BK/Z:BL:RD/AD:AG:BK/AI:AG:BK/I_:B_L:RD/AF:ADR:RD", 14));

    (void)mpSearchTree->removeNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT:BK/AF_:Z:BK/G:Z:BK/BC_:AF_:BK/AB_:AF_:RD/B:G:BK/AG:G:RD/AE_:AB_:BK/B_:AB_:BK/AD:AG:BK/AI:AG:BK/I_:B_L:RD/AF:ADR:RD", 13));

    (void)mpSearchTree->removeNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT:BK/AF_:Z:BK/AG:Z:BK/BC_:AF_:BK/AB_:AF_:RD/AD:AG:RD/AI:AG:BK/AE_:AB_:BK/B_:AB_:BK/G:AD:BK/AF:AD:BK/I_:B_L:RD", 12));

    (void)mpSearchTree->removeNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT:BK/AF_:Z:BK/AG:Z:BK/BC_:AF_:BK/AB_:AF_:RD/AD:AG:RD/AI:AG:BK/AE_:AB_:BK/I_:AB_:BK/G:AD:BK/AF:AD:BK", 11));

    (void)mpSearchTree->removeNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "Z:ROOT:BK/AF_:Z:BK/AG:Z:BK/BC_:AF_:BK/AB_:AF_:RD/AD:AG:BK/AI:AG:BK/AE_:AB_:BK/I_:AB_:BK/AF:ADR:RD", 10));

    (void)mpSearchTree->removeNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT:BK/AF_:AD:BK/AG:AD:BK/BC_:AF_:BK/AB_:AF_:RD/AF:AG:BK/AI:AG:BK/AE_:AB_:BK/I_:AB_:BK", 9));

    (void)mpSearchTree->removeNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT:BK/AF_:AD:BK/AG:AD:BK/BC_:AF_:BK/AB_:AF_:BK/AF:AG:BK/AI:AG:BK/AE_:AB_L:RD", 8));

    (void)mpSearchTree->removeNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AD:ROOT:BK/AF_:AD:RD/AG:AD:BK/BC_:AF_:BK/AB_:AF_:BK/AI:AGR:RD/AE_:AB_L:RD", 7));

    (void)mpSearchTree->removeNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT:BK/AF_:AG:RD/AI:AG:BK/BC_:AF_:BK/AB_:AF_:BK/AE_:AB_L:RD", 6));

    (void)mpSearchTree->removeNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT:BK/AE_:AG:BK/AI:AG:BK/AF_:AE_:RD/AB_:AE_:RD", 5));

    (void)mpSearchTree->removeNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AE_:ROOT:BK/AF_:AE_:BK/AI:AE_:BK/AB_:AIL:RD", 4));

    (void)mpSearchTree->removeNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AE_:AB_:BK/AI:AB_:BK", 3));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"BC_", "AF_", "AE_", "AB_", "I_", "E_", "B_", "A_", "Z", "B", "G", "H", "AD", "AF", "AG", "AI"}, scDefaultValue);

    (void)mpSearchTree->removeNode("BC_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/H:A_:BK/AF_:AB_:BK/E_:AB_:RD/B:H:RD/AF:H:RD/AE_:AF_R:RD/I_:E_:BK/B_:E_:BK/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 15));

    (void)mpSearchTree->removeNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AB_:A_:BK/H:A_:BK/AE_:AB_:BK/E_:AB_:RD/B:H:RD/AF:H:RD/I_:E_:BK/B_:E_:BK/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 14));

    (void)mpSearchTree->removeNode("AE_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/AB_:E_:BK/B_:E_:BK/B:H:RD/AF:H:RD/I_:AB_R:RD/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 13));

    (void)mpSearchTree->removeNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/E_:A_:BK/H:A_:BK/I_:E_:BK/B_:E_:BK/B:H:RD/AF:H:RD/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 12));

    (void)mpSearchTree->removeNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/A_:H:BK/AF:H:BK/E_:A_:BK/B:A_:RD/AD:AF:BK/AG:AF:BK/B_:E_R:RD/Z:B:BK/G:B:BK/AI:AGR:RD", 11));

    (void)mpSearchTree->removeNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/A_:H:BK/AF:H:BK/B_:A_:BK/B:A_:RD/AD:AF:BK/AG:AF:BK/Z:B:BK/G:B:BK/AI:AGR:RD", 10));

    (void)mpSearchTree->removeNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/B:H:BK/AF:H:BK/A_:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/Z:A_R:RD/AI:AGR:RD", 9));

    (void)mpSearchTree->removeNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/B:H:BK/AF:H:BK/Z:B:BK/G:B:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 8));

    (void)mpSearchTree->removeNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/B:H:BK/AF:H:RD/G:BR:RD/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 7));

    (void)mpSearchTree->removeNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "H:ROOT:BK/G:H:BK/AF:H:RD/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 6));

    (void)mpSearchTree->removeNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AF:ROOT:BK/H:AF:BK/AG:AF:BK/AD:HR:RD/AI:AGR:RD", 5));

    (void)mpSearchTree->removeNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AF:ROOT:BK/AD:AF:BK/AG:AF:BK/AI:AGR:RD", 4));

    (void)mpSearchTree->removeNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT:BK/AF:AG:BK/AI:AG:BK", 3));

    (void)mpSearchTree->removeNode("AF"); // root and two black children, erase left child
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AG:ROOT:BK/AI:AGR:RD", 2));

    (void)mpSearchTree->removeNode("AG"); // root and right child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AI:ROOT:BK", 1));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AI", "AG", "AF", "AD", "H", "G", "B", "Z", "A_", "B_", "E_", "I_", "AB_", "AE_", "AF_", "BC_"}, scDefaultValue, scCustomNullValue);

    (void)mpAuxSearchTree->removeNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT:BK/I_:Z:BK/AD:Z:BK/AE_:I_:RD/B_:I_:RD/G:AD:RD/AG:AD:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/B:G:BK/H:G:BK/AF:AGL:RD/BC_:AF_L:RD", 15));

    (void)mpAuxSearchTree->removeNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT:BK/I_:Z:BK/AD:Z:BK/AE_:I_:RD/B_:I_:RD/G:AD:RD/AF:AD:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/B:G:BK/H:G:BK/BC_:AF_L:RD", 14));

    (void)mpAuxSearchTree->removeNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT:BK/I_:Z:BK/G:Z:BK/AE_:I_:RD/B_:I_:RD/B:G:BK/AD:G:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/H:ADL:RD/BC_:AF_L:RD", 13));

    (void)mpAuxSearchTree->removeNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "Z:ROOT:BK/I_:Z:BK/G:Z:BK/AE_:I_:RD/B_:I_:RD/B:G:BK/H:G:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/BC_:AF_L:RD", 12));

    (void)mpAuxSearchTree->removeNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:BK/Z:I_:BK/AF_:AE_:BK/AB_:AE_:BK/B_:Z:RD/G:Z:BK/BC_:AF_L:RD/E_:B_:BK/A_:B_:BK/B:GL:RD", 11));

    (void)mpAuxSearchTree->removeNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:BK/Z:I_:BK/AF_:AE_:BK/AB_:AE_:BK/B_:Z:RD/B:Z:BK/BC_:AF_L:RD/E_:B_:BK/A_:B_:BK", 10));

    (void)mpAuxSearchTree->removeNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:BK/B_:I_:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/Z:B_:BK/BC_:AF_L:RD/A_:ZL:RD", 9));

    (void)mpAuxSearchTree->removeNode("Z");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:BK/B_:I_:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_:BK/A_:B_:BK/BC_:AF_L:RD", 8));

    (void)mpAuxSearchTree->removeNode("A_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:RD/B_:I_:BK/AF_:AE_:BK/AB_:AE_:BK/E_:B_L:RD/BC_:AF_L:RD", 7));

    (void)mpAuxSearchTree->removeNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "I_:ROOT:BK/AE_:I_:RD/E_:I_:BK/AF_:AE_:BK/AB_:AE_:BK/BC_:AF_L:RD", 6));

    (void)mpAuxSearchTree->removeNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AE_:ROOT:BK/AF_:AE_:BK/I_:AE_:BK/BC_:AF_L:RD/AB_:I_L:RD", 5));

    (void)mpAuxSearchTree->removeNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AE_:ROOT:BK/AF_:AE_:BK/AB_:AE_:BK/BC_:AF_L:RD", 4));

    (void)mpAuxSearchTree->removeNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "AF_:ROOT:BK/BC_:AF_:BK/AE_:AF_:BK", 3));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"BC_", "AI", "AF_", "AG", "AE_", "AF", "AB_", "AD", "I_", "H", "E_", "G", "B_", "B", "A_", "Z"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->removeNode("AD");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AF_:AB_:BK/A_:AB_:RD/BC_:AF_:BK/AE_:AF_:BK/E_:A_:BK/AF:A_:BK/I_:E_:BK/B_:E_:BK/G:AF:RD/AG:AF:BK/B:G:BK/H:G:BK/AI:AGR:RD/Z:BL:RD", 15));

    (void)mpSearchTree->removeNode("I_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AF_:AB_:BK/G:AB_:BK/BC_:AF_:BK/AE_:AF_:BK/A_:G:RD/AF:G:RD/E_:A_:BK/B:A_:BK/H:AF:BK/AG:AF:BK/B_:E_R:RD/Z:BL:RD/AI:AGR:RD", 14));

    (void)mpSearchTree->removeNode("H");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "AB_:ROOT:BK/AF_:AB_:BK/G:AB_:BK/BC_:AF_:BK/AE_:AF_:BK/A_:G:RD/AG:G:RD/E_:A_:BK/B:A_:BK/AF:AG:BK/AI:AG:BK/B_:E_R:RD/Z:BL:RD", 13));

    (void)mpSearchTree->removeNode("AB_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT:BK/AF_:E_:BK/G:E_:BK/BC_:AF_:BK/AE_:AF_:BK/A_:G:RD/AG:G:RD/B_:A_:BK/B:A_:BK/AF:AG:BK/AI:AG:BK/Z:BL:RD", 12));

    (void)mpSearchTree->removeNode("AF");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "E_:ROOT:BK/AF_:E_:BK/G:E_:BK/BC_:AF_:BK/AE_:AF_:BK/A_:G:RD/AG:G:BK/B_:A_:BK/B:A_:BK/AI:AGR:RD/Z:BL:RD", 11));

    (void)mpSearchTree->removeNode("E_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT:BK/AF_:B_:BK/G:B_:BK/BC_:AF_:BK/AE_:AF_:BK/Z:G:BK/AG:G:BK/A_:Z:RD/B:Z:RD/AI:AGR:RD", 10));

    (void)mpSearchTree->removeNode("G");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT:BK/AF_:B_:BK/AG:B_:BK/BC_:AF_:BK/AE_:AF_:BK/Z:AG:BK/AI:AG:BK/A_:Z:RD/B:Z:RD", 9));

    (void)mpSearchTree->removeNode("AE_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT:BK/AF_:B_:BK/AG:B_:RD/BC_:AF_L:RD/Z:AG:BK/AI:AG:BK/A_:Z:RD/B:Z:RD", 8));

    (void)mpSearchTree->removeNode("AG");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B_:ROOT:BK/AF_:B_:BK/Z:B_:RD/BC_:AF_L:RD/A_:Z:BK/AI:Z:BK/B:AIL:RD", 7));

    (void)mpSearchTree->removeNode("B_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AF_:A_:BK/B:A_:BK/BC_:AF_L:RD/Z:B:RD/AI:B:RD", 6));

    (void)mpSearchTree->removeNode("B");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/AF_:A_:BK/AI:A_:BK/BC_:AF_L:RD/Z:AIL:RD", 5));

    (void)mpSearchTree->removeNode("AF_");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/BC_:A_:BK/AI:A_:BK/Z:AIL:RD", 4));

    (void)mpSearchTree->removeNode("AI");
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "A_:ROOT:BK/BC_:A_:BK/Z:A_:BK", 3));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"C_", "E_", "B"}, scDefaultValue);

    (void)mpAuxSearchTree->removeNode("E_"); // root and two red children, erase left child
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "C_:ROOT:BK/B:C_R:RD", 2));

    (void)mpAuxSearchTree->removeNode("B"); // root and right child, erase right child
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "C_:ROOT:BK", 1));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"C_", "E_", "B"}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->removeNode("B"); // root and two red children, erase right child
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "C_:ROOT:BK/E_:C_L:RD", 2));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"C_", "E_", "B"}, scDefaultValue);

    (void)mpAuxSearchTree->removeNode("C_"); // root and two red children, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "B:ROOT:BK/E_:BL:RD", 2));

    // deleting null node from custom null value tree
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue, scCustomNullValue);

    nodeDeleted = mpSearchTree->removeNode("E_");
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "B:ROOT:BK/A_:B:BK/C:B:BK/B_:A_L:RD/D:CR:RD", 5));

    // deleting same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"A_", "C", "B", "D", "B_"}, scDefaultValue);

    nodeDeleted = mpSearchTree->removeNode("C");
    QVERIFY(nodeDeleted &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeDeleted = mpAuxSearchTree->removeNode("C");
    QVERIFY(nodeDeleted &&
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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1:A_:BK/G:f6:A_:BK/AF_:m13:E_:RD/B_:e5:E_:BK/B:d4:G:BK/AD:j10:G:RD/BC_:k11:AF_:BK/I_:h8:AF_:BK/Z:g7:BL:RD/H:b2:AD:BK/AF:i9:AD:BK/AB_:n14:I_L:RD/AG:l12:AFR:RD", 14));

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

    (void)mpSearchTree->addOrUpdateNode("E_", "a1");
    (void)mpSearchTree->addOrUpdateNode("H", "b2");
    (void)mpSearchTree->addOrUpdateNode("A_", scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode("B", "d4");
    (void)mpSearchTree->addOrUpdateNode("B_", "e5");
    (void)mpSearchTree->addOrUpdateNode("G", "f6");
    (void)mpSearchTree->addOrUpdateNode("H_", scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode("Z", "g7");
    (void)mpSearchTree->addOrUpdateNode("I_", "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_::ROOT:BK/E_:a1:A_:RD/G:f6:A_:RD/H_::E_:BK/B_:e5:E_:BK/B:d4:G:BK/H:b2:G:BK/I_:h8:H_L:RD/Z:g7:BL:RD", 9));
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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "Z:g7:ROOT:BK/E_:a1:Z:RD/G:f6:Z:BK/G_:i9:E_:BK/B_::E_:BK/H:b2:GR:RD/I_:h8:G_L:RD/A_::B_R:RD", 8));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(*mpSearchTree);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode("AG", "df");
    QVERIFY(newNodeAdded &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    newNodeAdded = mpSearchTree->addOrUpdateNode("G", scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue("G"));

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode("D", scCustomNullValue);
    QVERIFY(!newNodeAdded &&
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

    (void)mpSearchTree->addOrUpdateNode("E_", "a1");
    (void)mpSearchTree->addOrUpdateNode("H", "b2");
    (void)mpSearchTree->addOrUpdateNode("A_", "c3");
    (void)mpSearchTree->addOrUpdateNode("B", "d4");
    (void)mpSearchTree->addOrUpdateNode("B_", "e5");

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1:A_:BK/H:b2:A_:BK/B_:e5:E_R:RD/B:d4:HL:RD", 5));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->addOrUpdateNode("AG", "f6");
    mpSearchTree->addOrUpdateNode("Z", "g7");
    mpSearchTree->addOrUpdateNode("B_", "e5_1");
    mpSearchTree->addOrUpdateNode("C", "h8");

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

    (void)mpSearchTree->addOrUpdateNode("E", scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode("D", "newval");

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "D:newval:ROOT:BK/A_:DF:D:BK/E::D:BK/B_:DF:A_:RD/Z:DF:A_:RD", 5));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B_", "E", "D", "Z", "A_"}, scDefaultValue);

    (void)mpSearchTree->addOrUpdateNode("G", scDefaultNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode("D", scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "D:NULL:ROOT:BK/A_:DF:D:BK/E:DF:D:BK/B_:DF:A_:RD/Z:DF:A_:RD", 5));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeyRedBlackTreesTests::testMergeTrees()
{
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    (void)mpSearchTree->addOrUpdateNode("E_", "a1_1");
    (void)mpSearchTree->addOrUpdateNode("B", "d4");
    (void)mpSearchTree->addOrUpdateNode("G", "f6");
    (void)mpSearchTree->addOrUpdateNode("BC_", "k11");
    (void)mpSearchTree->addOrUpdateNode("AG", "l12");
    (void)mpSearchTree->addOrUpdateNode("AB_", "n14");
    (void)mpSearchTree->addOrUpdateNode("AF", "i9_1");
    (void)mpSearchTree->addOrUpdateNode("Z", "g7_1");

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    (void)mpAuxSearchTree->addOrUpdateNode("H", "b2");
    (void)mpAuxSearchTree->addOrUpdateNode("A_", "c3");
    (void)mpAuxSearchTree->addOrUpdateNode("B_", "e5");
    (void)mpAuxSearchTree->addOrUpdateNode("Z", "g7_2");
    (void)mpAuxSearchTree->addOrUpdateNode("E_", "a1_2");
    (void)mpAuxSearchTree->addOrUpdateNode("AF", "i9_2");
    (void)mpAuxSearchTree->addOrUpdateNode("I_", "h8");
    (void)mpAuxSearchTree->addOrUpdateNode("AD", "j10");
    (void)mpAuxSearchTree->addOrUpdateNode("AF_", "m13");

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

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_"}, scDefaultValue, scCustomNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode("I_", scDefaultNullValue);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1:A_:BK/G:DF:A_:BK/I_::E_:RD/B_::E_:BK/Z:g7:G:BK/AD:DF:G:RD/AF_:DF:I_:BK/G_:i9:I_:BK/H:b2:AD:BK/AF:DF:AD:BK/BC_:DF:AF_:RD/AB_:DF:AF_:RD/AG:DF:AFR:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. default)

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"E", "AZ", "B_", "I", "D", "B", "G", "H_"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"AF", "I_", "AD", "G", "BC_", "AG", "AF_", "AB_", "A"}, scDefaultValue, scDefaultNullValue);
    mpSearchTree->addOrUpdateNode("I", "abc");
    mpAuxSearchTree->addOrUpdateNode("G", "xyz");

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
    mpSearchTree->addOrUpdateNode("I", "abc");
    mpAuxSearchTree->addOrUpdateNode("G", "xyz");

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
