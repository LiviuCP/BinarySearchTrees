// clang-format off
#include <QTest>
#include <algorithm>

#include "testutils.h"

import binarysearchtree;

using namespace TestUtils;
using IntStrBinarySearchTree = BinarySearchTree<int, std::string>;
using IntStrBSTIterator = IntStrBinarySearchTree::InOrderForwardIterator;
using upIntStrBinarySearchTree = std::unique_ptr<IntStrBinarySearchTree>;

class IntKeySimpleBSTTests : public QObject
{
    Q_OBJECT

public:
    IntKeySimpleBSTTests();

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
    upIntStrBinarySearchTree mpSearchTree;
    upIntStrBinarySearchTree mpAuxSearchTree;
};

IntKeySimpleBSTTests::IntKeySimpleBSTTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

void IntKeySimpleBSTTests::init()
{
    QVERIFY(!mpSearchTree && !mpAuxSearchTree);
}

void IntKeySimpleBSTTests::cleanup()
{
    mpSearchTree.reset();
    mpAuxSearchTree.reset();
}

void IntKeySimpleBSTTests::testAddNodes()
{
    bool nodeInserted{false};
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeInserted = mpSearchTree->tryInsertNode(-5, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT", 1));

    nodeInserted = mpSearchTree->tryInsertNode(8, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/8:-5R", 2));

    nodeInserted = mpSearchTree->tryInsertNode(-1, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/8:-5R/-1:8L", 3));

    nodeInserted = mpSearchTree->tryInsertNode(2, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/8:-5R/-1:8L/2:-1R", 4));

    nodeInserted = mpSearchTree->tryInsertNode(-2, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/8:-5R/-1:8L/-2:-1/2:-1", 5));

    nodeInserted = mpSearchTree->tryInsertNode(7, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/8:-5R/-1:8L/-2:-1/2:-1/7:2R", 6));

    nodeInserted = mpSearchTree->tryInsertNode(0, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/8:-5R/-1:8L/-2:-1/2:-1/0:2/7:2", 7));

    nodeInserted = mpSearchTree->tryInsertNode(-9, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-1:8L/-2:-1/2:-1/0:2/7:2", 8));

    nodeInserted = mpSearchTree->tryInsertNode(16, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/0:2/7:2", 9));

    nodeInserted = mpSearchTree->tryInsertNode(14, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/14:16L/0:2/7:2", 10));

    nodeInserted = mpSearchTree->tryInsertNode(-23, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-2:-1/2:-1/14:16L/0:2/7:2", 11));

    nodeInserted = mpSearchTree->tryInsertNode(17, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-2:-1/2:-1/14:16/17:16/0:2/7:2", 12));

    nodeInserted = mpSearchTree->tryInsertNode(-16, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeInserted = mpSearchTree->tryInsertNode(-12, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/-12:-16R/0:2/7:2", 14));

    nodeInserted = mpSearchTree->tryInsertNode(19, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/-12:-16R/0:2/7:2/19:17R", 15));

    nodeInserted = mpSearchTree->tryInsertNode(-15, scDefaultValue);
    QVERIFY(nodeInserted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/-12:-16R/0:2/7:2/19:17R/-15:-12L", 16));

    // inserting nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    nodeInserted = mpSearchTree->tryInsertNode(25, scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            *mpAuxSearchTree == *mpSearchTree);

    nodeInserted = mpAuxSearchTree->tryInsertNode(-25, scCustomNullValue);
    QVERIFY(!nodeInserted &&
            *mpAuxSearchTree == *mpSearchTree);

    nodeInserted = mpSearchTree->tryInsertNode(25, scDefaultValue);
    QVERIFY(nodeInserted &&
            *mpAuxSearchTree != *mpSearchTree);

    nodeInserted = mpAuxSearchTree->tryInsertNode(25, scDefaultValue);
    QVERIFY(nodeInserted &&
            *mpAuxSearchTree == *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/-12:-16R/0:2/7:2/19:17R/-15:-12L/25:19R", 17));

    // some additional (corner) cases
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-23:ROOT/-16:-23R/-15:-16R/-12:-15R/-9:-12R/-5:-9R/-2:-5R/-1:-2R/0:-1R/2:0R/7:2R/8:7R/14:8R/16:14R/17:16R/19:17R", 16));

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "19:ROOT/17:19L/16:17L/14:16L/8:14L/7:8L/2:7L/0:2L/-1:0L/-2:-1L/-5:-2L/-9:-5L/-12:-9L/-15:-12L/-16:-15L/-23:-16L", 16));

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-23:ROOT/19:-23R/-16:19L/17:-16R/-15:17L/16:-15R/-12:16L/14:-12R/-9:14L/8:-9R/-5:8L/7:-5R/-2:7L/2:-2R/-1:2L/0:-1R", 16));

    mpSearchTree->clear();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 2, -3, 2, 4, 0, 1}, scDefaultValue, scDefaultNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, -3, 2, 4, 0, 1}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 2, -3, 4, 1, 0}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-3, -5, 2, 4, 0, 1}, scDefaultValue, scDefaultNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-3, 2, -5, 4, 0, 1}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{}, scDefaultValue, scDefaultNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
}
void IntKeySimpleBSTTests::testRemoveNodes()
{
    bool nodeRemoved{false};
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(*mpSearchTree);

    nodeRemoved = mpSearchTree->tryRemoveNode(-16);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-12:-23R/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeRemoved = mpSearchTree->tryRemoveNode(8);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/14:-5/-23:-9L/-1:14/16:14/-12:-23R/-2:-1/2:-1/17:16R/0:2/7:2", 12));

    nodeRemoved = mpSearchTree->tryRemoveNode(0);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/14:-5/-23:-9L/-1:14/16:14/-12:-23R/-2:-1/2:-1/17:16R/7:2R", 11));

    nodeRemoved = mpSearchTree->tryRemoveNode(-23);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/14:-5/-12:-9L/-1:14/16:14/-2:-1/2:-1/17:16R/7:2R", 10));

    nodeRemoved = mpSearchTree->tryRemoveNode(17);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT/-9:-5/14:-5/-12:-9L/-1:14/16:14/-2:-1/2:-1/7:2R", 9));

    nodeRemoved = mpSearchTree->tryRemoveNode(-5);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-2:ROOT/-9:-2/14:-2/-12:-9L/-1:14/16:14/2:-1R/7:2R", 8));

    nodeRemoved = mpSearchTree->tryRemoveNode(-9);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-2:ROOT/-12:-2/14:-2/-1:14/16:14/2:-1R/7:2R", 7));

    nodeRemoved = mpSearchTree->tryRemoveNode(14);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-2:ROOT/-12:-2/16:-2/-1:16L/2:-1R/7:2R", 6));

    nodeRemoved = mpSearchTree->tryRemoveNode(-1);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-2:ROOT/-12:-2/16:-2/2:16L/7:2R", 5));

    nodeRemoved = mpSearchTree->tryRemoveNode(-2);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT/-12:2/16:2/7:16L", 4));

    nodeRemoved = mpSearchTree->tryRemoveNode(-12);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT/16:2R/7:16L", 3));

    nodeRemoved = mpSearchTree->tryRemoveNode(16);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT/7:2R", 2));

    nodeRemoved = mpSearchTree->tryRemoveNode(2);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "7:ROOT", 1));

    nodeRemoved = mpSearchTree->tryRemoveNode(7);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeRemoved = mpSearchTree->tryRemoveNode(7);
    QVERIFY(!nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeRemoved = mpSearchTree->tryRemoveNode(25);
    QVERIFY(!nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // some additional (corner) cases
    nodeRemoved = mpAuxSearchTree->tryRemoveNode(-9);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-5:ROOT/-23:-5/8:-5/-16:-23R/-1:8/16:8/-12:-16R/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeRemoved = mpAuxSearchTree->tryRemoveNode(-1);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-5:ROOT/-23:-5/8:-5/-16:-23R/0:8/16:8/-12:-16R/-2:0/2:0/14:16/17:16/7:2R", 12));

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{2, -12}, scDefaultValue);

    nodeRemoved = mpSearchTree->tryRemoveNode(2);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-12:ROOT", 1));

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{2, -12, 7}, scDefaultValue);
    *mpAuxSearchTree = *mpSearchTree;

    nodeRemoved = mpSearchTree->tryRemoveNode(-12);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT/7:2R", 2));

    nodeRemoved = mpSearchTree->tryRemoveNode(7);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT", 1));

    nodeRemoved = mpAuxSearchTree->tryRemoveNode(7);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "2:ROOT/-12:2L", 2));

    nodeRemoved = mpAuxSearchTree->tryRemoveNode(-12);
    QVERIFY(nodeRemoved &&
            areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "2:ROOT", 1));

    // removing null node from custom null value tree
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue, scCustomNullValue);

    nodeRemoved = mpSearchTree->tryRemoveNode(-5);
    QVERIFY(!nodeRemoved &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT/-2:-1/3:-1/2:3/4:3", 5));

    // removing same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue);

    nodeRemoved = mpSearchTree->tryRemoveNode(3);
    QVERIFY(nodeRemoved &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeRemoved = mpAuxSearchTree->tryRemoveNode(3);
    QVERIFY(nodeRemoved &&
            *mpSearchTree == *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT/-2:-1/4:-1/2:4L", 4));
}

void IntKeySimpleBSTTests::testUpdateNodeValue()
{
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());

    (void)mpSearchTree->tryInsertNode(-5, "a1");
    (void)mpSearchTree->tryInsertNode(8, "b2");
    (void)mpSearchTree->tryInsertNode(-1, "c3");
    (void)mpSearchTree->tryInsertNode(2, "d4");
    (void)mpSearchTree->tryInsertNode(-2, "e5");
    (void)mpSearchTree->tryInsertNode(7, "f6");
    (void)mpSearchTree->tryInsertNode(0, "g7");
    (void)mpSearchTree->tryInsertNode(-9, "h8");
    (void)mpSearchTree->tryInsertNode(16, "i9");
    (void)mpSearchTree->tryInsertNode(14, "j10");
    (void)mpSearchTree->tryInsertNode(-23, "k11");
    (void)mpSearchTree->tryInsertNode(17, "l12");
    (void)mpSearchTree->tryInsertNode(-16, "m13");
    (void)mpSearchTree->tryInsertNode(-12, "n14");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1:ROOT/-9:h8:-5/8:b2:-5/-23:k11:-9L/-1:c3:8/16:i9:8/-16:m13:-23R/-2:e5:-1/2:d4:-1/14:j10:16/17:l12:16/-12:n14:-16R/0:g7:2/7:f6:2", 14));

    QVERIFY("a1" == mpSearchTree->getNodeValue(-5) &&
            "g7" == mpSearchTree->getNodeValue(0) &&
            "i9" == mpSearchTree->getNodeValue(16));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(25));

    // update by inserting/modifying node
    bool nodeInserted{false};

    nodeInserted = mpSearchTree->tryInsertNode(2, "d4_1");
    QVERIFY(!nodeInserted &&
            "d4_1" == mpSearchTree->getNodeValue(2));

    nodeInserted = mpSearchTree->tryInsertNode(-9, "b2");
    QVERIFY(!nodeInserted &&
            "b2" == mpSearchTree->getNodeValue(-9));

    nodeInserted = mpSearchTree->tryInsertNode(14, "j10");
    QVERIFY(!nodeInserted &&
            "j10" == mpSearchTree->getNodeValue(14));

    nodeInserted = mpSearchTree->tryInsertNode(17, "L12");
    QVERIFY(!nodeInserted &&
            "L12" == mpSearchTree->getNodeValue(17));

    nodeInserted = mpSearchTree->tryInsertNode(-12, scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            "n14" == mpSearchTree->getNodeValue(-12));

    nodeInserted = mpSearchTree->tryInsertNode(25, scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(25));

    nodeInserted = mpSearchTree->tryInsertNode(25, "o15");
    QVERIFY(nodeInserted &&
            "o15" == mpSearchTree->getNodeValue(25));

    nodeInserted = mpSearchTree->tryInsertNode(25, scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            "o15" == mpSearchTree->getNodeValue(25));

    // update by removing node
    bool nodeRemoved{false};

    nodeRemoved = mpSearchTree->tryRemoveNode(7);
    QVERIFY(nodeRemoved &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    nodeRemoved = mpSearchTree->tryRemoveNode(-28);
    QVERIFY(!nodeRemoved &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-28));

    // remove and re-insert node with different value
    nodeRemoved = mpSearchTree->tryRemoveNode(16);
    nodeInserted = mpSearchTree->tryInsertNode(16, "i9_new");

    QVERIFY(nodeRemoved &&
            nodeInserted &&
            "i9_new" == mpSearchTree->getNodeValue(16));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1:ROOT/-9:b2:-5/8:b2:-5/-23:k11:-9L/-1:c3:8/17:L12:8/-16:m13:-23R/-2:e5:-1/2:d4_1:-1/14:j10:17/25:o15:17/-12:n14:-16R/0:g7:2L/16:i9_new:14R", 14));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

    // test with same value for all nodes
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-5:DF:ROOT/-9:DF:-5/8:DF:-5/-23:DF:-9L/-1:DF:8/16:DF:8/-16:DF:-23R/-2:DF:-1/2:DF:-1/14:DF:16/17:DF:16/-12:DF:-16R/0:DF:2/7:DF:2", 14));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue(-5) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(0) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(16));

    // test updating custom null value tree node values
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);

    (void)mpSearchTree->tryInsertNode(-5, "a1");
    (void)mpSearchTree->tryInsertNode(8, "b2");
    (void)mpSearchTree->tryInsertNode(-1, scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode(2, "d4");
    (void)mpSearchTree->tryInsertNode(-2, "e5");
    (void)mpSearchTree->tryInsertNode(7, "f6");
    (void)mpSearchTree->tryInsertNode(-8, scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode(0, "g7");
    (void)mpSearchTree->tryInsertNode(-9, "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1:ROOT/-8::-5/8:b2:-5/-9:h8:-8L/-1::8L/-2:e5:-1/2:d4:-1/0:g7:2/7:f6:2", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());

    nodeInserted = mpSearchTree->tryInsertNode(2, "d4_1");
    QVERIFY(!nodeInserted &&
            "d4_1" == mpSearchTree->getNodeValue(2));

    nodeInserted = mpSearchTree->tryInsertNode(-1, "c3");
    QVERIFY(!nodeInserted &&
            "c3" == mpSearchTree->getNodeValue(-1));

    nodeInserted = mpSearchTree->tryInsertNode(7, scCustomNullValue);
    QVERIFY(!nodeInserted &&
            "f6" == mpSearchTree->getNodeValue(7));

    nodeInserted = mpSearchTree->tryInsertNode(-2, scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-2));

    nodeInserted = mpSearchTree->tryInsertNode(25, scCustomNullValue);
    QVERIFY(!nodeInserted &&
            scCustomNullValue == mpSearchTree->getNodeValue(25));

    nodeInserted = mpSearchTree->tryInsertNode(-7, "i9");
    QVERIFY(nodeInserted &&
            "i9" == mpSearchTree->getNodeValue(-7));

    nodeRemoved = mpSearchTree->tryRemoveNode(-25);
    QVERIFY(!nodeRemoved &&
            scCustomNullValue == mpSearchTree->getNodeValue(-25));

    nodeRemoved = mpSearchTree->tryRemoveNode(-8);
    QVERIFY(nodeRemoved &&
            scCustomNullValue == mpSearchTree->getNodeValue(-8));

    nodeRemoved = mpSearchTree->tryRemoveNode(2);
    QVERIFY(nodeRemoved &&
            scCustomNullValue == mpSearchTree->getNodeValue(2));

    nodeRemoved = mpSearchTree->tryRemoveNode(-1);
    nodeInserted = mpSearchTree->tryInsertNode(-1, scDefaultNullValue);

    QVERIFY(nodeRemoved &&
            nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-1));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1:ROOT/-7:i9:-5/8:b2:-5/-9:h8:-7L/0:g7:8L/-2::0/7:f6:0/-1::-2R", 8));
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(*mpSearchTree);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    nodeInserted = mpAuxSearchTree->tryInsertNode(17, "df");
    QVERIFY(nodeInserted &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue);

    nodeInserted = mpSearchTree->tryInsertNode(7, scDefaultNullValue);
    QVERIFY(!nodeInserted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    nodeInserted = mpAuxSearchTree->tryInsertNode(4, scCustomNullValue);
    QVERIFY(!nodeInserted &&
            scCustomNullValue == mpAuxSearchTree->getNodeValue(4));

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "16:DF:ROOT/-9:DF:16/17:DF:16/-23:DF:-9/14:DF:-9/-16:DF:-23R/7::14L/-12:DF:-16R", 8));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void IntKeySimpleBSTTests::testMoveSemantics()
{
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();

    (void)mpSearchTree->tryInsertNode(-5, "a1");
    (void)mpSearchTree->tryInsertNode(8, "b2");
    (void)mpSearchTree->tryInsertNode(-1, "c3");
    (void)mpSearchTree->tryInsertNode(2, "d4");
    (void)mpSearchTree->tryInsertNode(-2, "e5");
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-5:a1:ROOT/8:b2:-5R/-1:c3:8L/-2:e5:-1/2:d4:-1", 5));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->tryInsertNode(17, "f6");
    mpSearchTree->tryInsertNode(0, "g7");
    mpSearchTree->tryInsertNode(-2, "e5_1");
    mpSearchTree->tryInsertNode(3, "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "17:f6:ROOT/0:g7:17L/-2:e5_1:0/3:h8:0", 4));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "17:f6:ROOT/0:g7:17L/-2:e5_1:0/3:h8:0", 4));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "17:f6:ROOT/0:g7:17L/-2:e5_1:0/3:h8:0", 4));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // test move constructor for trees with custom null value
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->tryInsertNode(5, scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode(4, "newval");
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-2:DF:ROOT/5::-2R/4:newval:5L/0:DF:4L/-1:DF:0L", 5));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue);

    (void)mpSearchTree->tryInsertNode(7, scDefaultNullValue);
    (void)mpAuxSearchTree->tryInsertNode(4, scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-2:DF:ROOT/5:DF:-2R/4:NULL:5L/0:DF:4L/-1:DF:0L", 5));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void IntKeySimpleBSTTests::testMergeTrees()
{
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();

    (void)mpSearchTree->tryInsertNode(-5, "a1_1");
    (void)mpSearchTree->tryInsertNode(2, "d4");
    (void)mpSearchTree->tryInsertNode(7, "f6");
    (void)mpSearchTree->tryInsertNode(-23, "k11");
    (void)mpSearchTree->tryInsertNode(17, "l12");
    (void)mpSearchTree->tryInsertNode(-12, "n14");
    (void)mpSearchTree->tryInsertNode(16, "i9_1");
    (void)mpSearchTree->tryInsertNode(0, "g7_1");
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>();

    (void)mpAuxSearchTree->tryInsertNode(8, "b2");
    (void)mpAuxSearchTree->tryInsertNode(-1, "c3");
    (void)mpAuxSearchTree->tryInsertNode(-2, "e5");
    (void)mpAuxSearchTree->tryInsertNode(0, "g7_2");
    (void)mpAuxSearchTree->tryInsertNode(-5, "a1_2");
    (void)mpAuxSearchTree->tryInsertNode(16, "i9_2");
    (void)mpAuxSearchTree->tryInsertNode(-9, "h8");
    (void)mpAuxSearchTree->tryInsertNode(14, "j10");
    (void)mpAuxSearchTree->tryInsertNode(-16, "m13");
    
    const IntStrBinarySearchTree searchTreeCopy{*mpSearchTree};
    const IntStrBinarySearchTree searchTreeAuxCopy{*mpAuxSearchTree};

    QVERIFY(searchTreeCopy == *mpSearchTree &&      // just a(n additional) check that the copy constructor and == operator work correctly
            searchTreeAuxCopy == *mpAuxSearchTree);

    // first (main) merge
    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0L/17:l12:7R/-2:e5:-1L/16:i9_2:17L/8:b2:16L/14:j10:8R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    
    const IntStrBinarySearchTree mainTreeAfterFirstMerge{*mpSearchTree};

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
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2L/2:d4:0R/-9:h8:-5L/7:f6:2R/-16:m13:-9L/-23:k11:-16/-12:n14:-16", 14));
    QVERIFY(*mpAuxSearchTree != mainTreeAfterFirstMerge); // test the != operator too
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // merge trees with (same) custom null value
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);

    (void)mpSearchTree->tryInsertNode(-5, "a1");
    (void)mpSearchTree->tryInsertNode(8, "b2");
    (void)mpSearchTree->tryInsertNode(-1, "c3");
    (void)mpSearchTree->tryInsertNode(2, "d4");
    (void)mpSearchTree->tryInsertNode(-2, scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode(-8, "e5");
    (void)mpSearchTree->tryInsertNode(7, scDefaultNullValue);
    (void)mpSearchTree->tryInsertNode(0, "g7");
    (void)mpSearchTree->tryInsertNode(-9, "h8");
    (void)mpSearchTree->tryInsertNode(-7, "i9");
    (void)mpSearchTree->tryRemoveNode(-8);
    (void)mpSearchTree->tryRemoveNode(2);
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue);
    (void)mpAuxSearchTree->tryInsertNode(-9, scDefaultNullValue);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1:ROOT/-7:i9:-5/8:b2:-5/-9::-7L/-1:c3:8/16:DF:8/-23:DF:-9L/-2::-1/7:DF:-1/14:DF:16/17:DF:16/-16:DF:-23R/0:g7:7L/-12:DF:-16R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. default)
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{5, 10, -2, 9, 4, 2, 7, -8}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12, 1}, scDefaultValue, scDefaultNullValue);
    mpSearchTree->tryInsertNode(9, "abc");
    mpAuxSearchTree->tryInsertNode(7, "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "5:DF:ROOT/-2:DF:5/10:DF:5/-8:DF:-2/4:DF:-2/9:abc:10L/2:DF:4L/7:DF:9L", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "16:DF:ROOT/-9:DF:16/17:DF:16/-23:DF:-9/14:DF:-9/-16:DF:-23R/7:xyz:14L/-12:DF:-16R/1:DF:7L", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "16:DF:ROOT/-9:DF:16/17:DF:16/-23:DF:-9/14:DF:-9/-16:DF:-23R/7:xyz:14L/-12:DF:-16R/1:DF:7L", 9));

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
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{5, 10, -2, 9, 4, 2, 7, -8}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12, 1}, scDefaultValue, scCustomNullValue + "1");
    mpSearchTree->tryInsertNode(9, "abc");
    mpAuxSearchTree->tryInsertNode(7, "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "5:DF:ROOT/-2:DF:5/10:DF:5/-8:DF:-2/4:DF:-2/9:abc:10L/2:DF:4L/7:DF:9L", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "16:DF:ROOT/-9:DF:16/17:DF:16/-23:DF:-9/14:DF:-9/-16:DF:-23R/7:xyz:14L/-12:DF:-16R/1:DF:7L", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "16:DF:ROOT/-9:DF:16/17:DF:16/-23:DF:-9/14:DF:-9/-16:DF:-23R/7:xyz:14L/-12:DF:-16R/1:DF:7L", 9));

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

void IntKeySimpleBSTTests::testInOrderForwardIterators()
{
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();

    (void)mpSearchTree->tryInsertNode(-5, "b");
    (void)mpSearchTree->tryInsertNode(8, "z");
    (void)mpSearchTree->tryInsertNode(-1, "_ca");
    (void)mpSearchTree->tryInsertNode(2, "q1");
    (void)mpSearchTree->tryInsertNode(-2, "55");
    (void)mpSearchTree->tryInsertNode(7, "a");
    (void)mpSearchTree->tryInsertNode(0, "fq");
    (void)mpSearchTree->tryInsertNode(-9, scDefaultValue);
    (void)mpSearchTree->tryInsertNode(16, "cCc");
    (void)mpSearchTree->tryInsertNode(14, "abab");
    (void)mpSearchTree->tryInsertNode(-23, "-c");
    (void)mpSearchTree->tryInsertNode(17, "b");
    (void)mpSearchTree->tryInsertNode(-16, "qa");
    (void)mpSearchTree->tryInsertNode(-12, "dev");
    (void)mpSearchTree->tryInsertNode(19, "_ca");
    (void)mpSearchTree->tryInsertNode(-15, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true),
                                     mpSearchTree->getSize(),
                                     "-5:b:ROOT/-9:DF:-5/8:z:-5/-23:-c:-9L/-1:_ca:8/16:cCc:8/-16:qa:-23R/-2:55:-1/2:q1:-1/14:abab:16/17:b:16/-12:dev:-16R/0:fq:2/7:a:2/19:_ca:17R/-15:DF:-12L",
                                     16));

    IntStrBSTIterator it{mpSearchTree->begin()};
    QVERIFY(it.getKey() == -23 && it.getValue() == "-c");

    it = mpSearchTree->root();
    QVERIFY(it.getKey() == -5 && it.getValue() == "b");

    it = mpSearchTree->find(-2);
    QVERIFY(it.getKey() == -2 && it.getValue() == "55");

    QVERIFY(mpSearchTree->find(-23) == mpSearchTree->begin());
    QVERIFY(mpSearchTree->find(-5) == mpSearchTree->root());
    QVERIFY(mpSearchTree->find(12) == mpSearchTree->end());

    std::vector<std::pair<std::optional<int>, std::string>> traversedElements;
    const std::vector<std::pair<std::optional<int>, std::string>> c_TraversedElementsRef{{-23, "-c"}, {-16, "qa"}, {-15, "DF"}, {-12, "dev"}, {-9, "DF"}, {-5, "b"}, {-2, "55"}, {-1, "_ca"},
                                                                          {0, "fq"}, {2, "q1"}, {7, "a"}, {8, "z"}, {14, "abab"}, {16, "cCc"}, {17, "b"}, {19, "_ca"}};

    for (IntStrBSTIterator it{mpSearchTree->begin()}; it != mpSearchTree->end(); it.next())
    {
        traversedElements.push_back({it.getKey(), it.getValue()});
    }

    QVERIFY(std::equal(traversedElements.cbegin(), traversedElements.cend(), c_TraversedElementsRef.cbegin()));

    (void)mpSearchTree->tryInsertNode(14, "BaBa");
    it = mpSearchTree->find(14);

    QVERIFY(it.getValue() == "BaBa");

    it.next();
    QVERIFY(it.getKey() == 16);

    it.setValue("bCCC");
    QVERIFY(mpSearchTree->getNodeValue(16) == "bCCC");

    it = mpSearchTree->find(19);
    it.next();

    QVERIFY(it == mpSearchTree->end());
    QVERIFY(!it.getKey().has_value() && it.getValue().empty());

    QVERIFY(mpSearchTree->find(2) != mpSearchTree->end());
    (void)mpSearchTree->tryRemoveNode(2);
    QVERIFY(mpSearchTree->find(2) == mpSearchTree->end());
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>("NullVal");
    QVERIFY(mpAuxSearchTree->begin() == mpAuxSearchTree->end() && mpAuxSearchTree->root() == mpAuxSearchTree->end() && mpAuxSearchTree->find(14) == mpAuxSearchTree->end());

    IntStrBSTIterator itAux;
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue().empty() && itAux == mpSearchTree->end() && itAux != mpAuxSearchTree->end());

    itAux = mpAuxSearchTree->end();
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue() == "NullVal");

    // the last part of this test is only relevant for "simple" BSTs (for RB and AVL a balancing is being performed)
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-8, -4, -2, 0, 3, 5, 9, 12}, scDefaultValue);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-8:ROOT/-4:-8R/-2:-4R/0:-2R/3:0R/5:3R/9:5R/12:9R", 8));
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{12, 9, 5, 3, 0, -2, -4, -8}, scDefaultValue);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "12:ROOT/9:12L/5:9L/3:5L/0:3L/-2:0L/-4:-2L/-8:-4L", 8));

    std::vector<std::optional<int>> traversedKeys;
    const std::vector<std::optional<int>> c_TraversedKeysRef{-8, -4, -2, 0, 3, 5, 9, 12};

    for (IntStrBSTIterator it{mpSearchTree->begin()}; it != mpSearchTree->end(); it.next())
    {
        traversedKeys.push_back(it.getKey());
    }

    QVERIFY(std::equal(traversedKeys.cbegin(), traversedKeys.cend(), c_TraversedKeysRef.cbegin()));

    traversedKeys.clear();

    for (IntStrBSTIterator it{mpAuxSearchTree->begin()}; it != mpAuxSearchTree->end(); it.next())
    {
        traversedKeys.push_back(it.getKey());
    }

    QVERIFY(std::equal(traversedKeys.cbegin(), traversedKeys.cend(), c_TraversedKeysRef.cbegin()));
}

void IntKeySimpleBSTTests::testPrintTree()
{
    qInfo("Creating new tree");
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue);

    QVERIFY(14 == mpSearchTree->getSize());

    qInfo("The tree nodes are: ");
    mpSearchTree->printTree();

    qInfo("Clearing tree content");
    mpSearchTree->clear();

    QVERIFY(0 == mpSearchTree->getSize());

    mpSearchTree->printTree();
    qInfo("The tree has no nodes");
}

QTEST_APPLESS_MAIN(IntKeySimpleBSTTests)

#include "tst_intkeysimplebsttests.moc"
// clang-format on
