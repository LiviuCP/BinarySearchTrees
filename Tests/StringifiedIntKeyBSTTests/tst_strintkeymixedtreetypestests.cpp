// clang-format off
#include <QTest>

#include "testutils.h"
#include "stringifiedinteger.h"

import binarysearchtree;
import redblacktree;
import avltree;

using namespace TestUtils;
using StrIntStrBinarySearchTree = BinarySearchTree<StringifiedInteger, std::string>;
using StrIntStrRedBlackTree = RedBlackTree<StringifiedInteger, std::string>;
using StrIntStrAVLTree = AVLTree<StringifiedInteger, std::string>;
using upStrIntStrBinarySearchTree = std::unique_ptr<StrIntStrBinarySearchTree>;

class StrIntKeyMixedTreeTypesTests : public QObject
{
    Q_OBJECT

public:
    StrIntKeyMixedTreeTypesTests();

private slots:
    void init();
    void cleanup();

    void testMergeDifferentSearchTrees();
    void testFailToMergeWhenNullValuesAreDifferent();
    void testDifferentTreeTypesEquivalence();
    void testCopyAssignmentOfMixedTreeTypes();
    void testMoveAssignmentOfMixedTreeTypes();

private:
    void _buildPrimaryTestSearchTree(); // create content for the primary test tree (referenced by mpSearchTree)
    void _buildSecondaryTestSearchTree(); // create content for the secondary test tree (referenced by mpAuxSearchTree)
    void _buildPrimaryTestSearchTreeInDifferentOrder(); // same content, different node inserting order to the tree created by _buildPrimaryTestSearchTree() method (referenced by mpAuxSearchTree)
    void _buildSecondaryTestSearchTreeInDifferentOrder(); // same content, different node inserting order to the tree created by _buildSecondaryTestSearchTree() method (referenced by mpSearchTree)
    
    upStrIntStrBinarySearchTree mpSearchTree;
    upStrIntStrBinarySearchTree mpAuxSearchTree;
};

StrIntKeyMixedTreeTypesTests::StrIntKeyMixedTreeTypesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

void StrIntKeyMixedTreeTypesTests::init()
{
    QVERIFY(!mpSearchTree && !mpAuxSearchTree);
}

void StrIntKeyMixedTreeTypesTests::cleanup()
{
    mpSearchTree.reset();
    mpAuxSearchTree.reset();
}

void StrIntKeyMixedTreeTypesTests::testMergeDifferentSearchTrees()
{
    /* create all trees to be used in the merge operations */
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();
    
    const StrIntStrBinarySearchTree simpleTreeCopy{*mpSearchTree};
    const StrIntStrBinarySearchTree simpleTreeAuxCopy{*mpAuxSearchTree};

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    const StrIntStrRedBlackTree redBlackTreeCopy{*dynamic_cast<StrIntStrRedBlackTree*>(mpSearchTree.get())};
    const StrIntStrRedBlackTree redBlackTreeAuxCopy{*dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get())};

    mpSearchTree = std::make_unique<StrIntStrAVLTree>();
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    const StrIntStrAVLTree avlTreeCopy{*dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get())};
    const StrIntStrAVLTree avlTreeAuxCopy{*dynamic_cast<StrIntStrAVLTree*>(mpAuxSearchTree.get())};

    /* merge RED-BLACK tree into SIMPLE tree */
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(simpleTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(redBlackTreeAuxCopy);

    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1_2:ROOT/BC_:k11:E_/B:d4:E_/AB_:n14:BC_R/Z:g7_2:B/G:f6:B/AF_:m13:AB_/I_:h8:AB_/A_:c3:ZL/AG:l12:GR/B_:e5:A_L/AF:i9_2:AGL/H:b2:AFL/AD:j10:HR", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(redBlackTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(simpleTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "H:b2:ROOT/A_:c3:H/AF:i9_1:H/B_:e5:A_/Z:g7_1:A_/AD:j10:AF/AG:l12:AF/E_:a1_1:B_L/B:d4:ZR/I_:h8:E_L/G:f6:BR/AF_:m13:I_L/BC_:k11:AF_/AB_:n14:AF_", 14));

    /* merge AVL tree into SIMPLE tree */
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(simpleTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(avlTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1_2:ROOT/BC_:k11:E_/B:d4:E_/AB_:n14:BC_R/Z:g7_2:B/G:f6:B/AF_:m13:AB_/I_:h8:AB_/A_:c3:ZL/AG:l12:GR/B_:e5:A_L/AF:i9_2:AGL/H:b2:AFL/AD:j10:HR", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(avlTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(simpleTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "H:b2:ROOT/A_:c3:H/AF:i9_1:H/B_:e5:A_/Z:g7_1:A_/AD:j10:AF/AG:l12:AF/E_:a1_1:B_L/B:d4:ZR/I_:h8:E_L/G:f6:BR/AF_:m13:I_L/BC_:k11:AF_/AB_:n14:AF_", 14));

    /* merge AVL tree into RED-BLACK tree */

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(redBlackTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(avlTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:BK/AF:i9_2:B:BK/BC_:k11:AB_:BK/A_:c3:AB_:RD/H:b2:AF:BK/AG:l12:AF:BK/AF_:m13:BC_R:RD/E_:a1_2:A_:BK/Z:g7_2:A_:BK/G:f6:H:RD/AD:j10:H:RD/I_:h8:E_:RD/B_:e5:E_:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(avlTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(redBlackTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_1:A_:BK/H:b2:A_:BK/AB_:n14:E_:RD/B_:e5:E_:BK/B:d4:H:BK/AF:i9_1:H:BK/AF_:m13:AB_:BK/I_:h8:AB_:BK/Z:g7_1:B:RD/G:f6:B:RD/AD:j10:AF:RD/AG:l12:AF:RD/BC_:k11:AF_L:RD", 14));

    /* merge SIMPLE tree into RED-BLACK tree */

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(redBlackTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(simpleTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:BK/AF:i9_2:B:BK/BC_:k11:AB_:BK/A_:c3:AB_:RD/H:b2:AF:BK/AG:l12:AF:BK/AF_:m13:BC_R:RD/E_:a1_2:A_:BK/Z:g7_2:A_:BK/G:f6:H:RD/AD:j10:H:RD/I_:h8:E_:RD/B_:e5:E_:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(simpleTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(redBlackTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_1:A_:BK/H:b2:A_:BK/AF_:m13:E_:RD/B_:e5:E_:BK/B:d4:H:BK/AF:i9_1:H:BK/BC_:k11:AF_:BK/I_:h8:AF_:BK/Z:g7_1:B:RD/G:f6:B:RD/AD:j10:AF:RD/AG:l12:AF:RD/AB_:n14:I_L:RD", 14));

    /* merge SIMPLE tree into AVL tree */

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(avlTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(simpleTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/E_:a1_2:B/AF:i9_2:B/AB_:n14:E_/A_:c3:E_/H:b2:AF/AG:l12:AF/BC_:k11:AB_/I_:h8:AB_/B_:e5:A_/Z:g7_2:A_/G:f6:H/AD:j10:H/AF_:m13:BC_R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(simpleTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(avlTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/I_:h8:A_/H:b2:A_/AF_:m13:I_/E_:a1_1:I_/B:d4:H/AF:i9_1:H/BC_:k11:AF_/AB_:n14:AF_/B_:e5:E_R/Z:g7_1:B/G:f6:B/AD:j10:AF/AG:l12:AF", 14));

    /* merge RED-BLACK tree into AVL tree */

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(avlTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(redBlackTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/E_:a1_2:B/AF:i9_2:B/AB_:n14:E_/A_:c3:E_/H:b2:AF/AG:l12:AF/BC_:k11:AB_/I_:h8:AB_/B_:e5:A_/Z:g7_2:A_/G:f6:H/AD:j10:H/AF_:m13:BC_R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(redBlackTreeCopy);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(avlTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/AB_:n14:A_/H:b2:A_/AF_:m13:AB_/E_:a1_1:AB_/B:d4:H/AF:i9_1:H/BC_:k11:AF_L/I_:h8:E_/B_:e5:E_/Z:g7_1:B/G:f6:B/AD:j10:AF/AG:l12:AF", 14));
}

void StrIntKeyMixedTreeTypesTests::testFailToMergeWhenNullValuesAreDifferent()
{
    /* (attempt to) merge RED-BLACK tree into SIMPLE tree */
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1_1:ROOT/BC_:k11:E_/B:d4:E_/AB_:n14:BC_R/Z:g7_1:B/G:f6:B/AG:l12:GR/AF:i9_1:AGL", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_2:A_:RD/H:b2:A_:RD/I_:h8:E_:BK/B_:e5:E_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AF_:m13:I_L:RD/AD:j10:AFL:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge AVL tree into SIMPLE tree */
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1_1:ROOT/BC_:k11:E_/B:d4:E_/AB_:n14:BC_R/Z:g7_1:B/G:f6:B/AG:l12:GR/AF:i9_1:AGL", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge AVL tree into RED-BLACK tree */

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:RD/AF:i9_1:B:BK/BC_:k11:AB_:BK/E_:a1_1:AB_:BK/G:f6:AF:RD/AG:l12:AF:RD/Z:g7_1:E_R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge SIMPLE tree into RED-BLACK tree */

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:RD/AF:i9_1:B:BK/BC_:k11:AB_:BK/E_:a1_1:AB_:BK/G:f6:AF:RD/AG:l12:AF:RD/Z:g7_1:E_R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "H:b2:ROOT/A_:c3:H/AF:i9_2:H/B_:e5:A_/Z:g7_2:A_/AD:j10:AFL/E_:a1_2:B_L/I_:h8:E_L/AF_:m13:I_L", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge SIMPLE tree into AVL tree */

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "H:b2:ROOT/A_:c3:H/AF:i9_2:H/B_:e5:A_/Z:g7_2:A_/AD:j10:AFL/E_:a1_2:B_L/I_:h8:E_L/AF_:m13:I_L", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge RED-BLACK tree into AVL tree */

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_2:A_:RD/H:b2:A_:RD/I_:h8:E_:BK/B_:e5:E_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AF_:m13:I_L:RD/AD:j10:AFL:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeyMixedTreeTypesTests::testDifferentTreeTypesEquivalence()
{
    // Simple BST vs. Red-Black tree
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);
    _buildPrimaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:RD/AF:i9_1:B:BK/BC_:k11:AB_:BK/E_:a1_1:AB_:BK/G:f6:AF:RD/AG:l12:AF:RD/Z:g7_1:E_R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    (void)mpSearchTree->tryInsertNode("AB_", "n14_1");
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    (void)mpAuxSearchTree->tryInsertNode("AB_", "n14_1");
    QVERIFY(*dynamic_cast<StrIntStrRedBlackTree*>(mpSearchTree.get()) == *mpAuxSearchTree);

    (void)mpSearchTree->tryInsertNode("A", "abcd");
    (void)mpAuxSearchTree->tryInsertNode("A", "abcd");
    QVERIFY(*dynamic_cast<StrIntStrRedBlackTree*>(mpSearchTree.get()) != *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14_1:B:RD/AF:i9_1:B:BK/BC_:k11:AB_:BK/Z:g7_1:AB_:BK/G:f6:AF:RD/AG:l12:AF:RD/E_:a1_1:Z:RD/A:abcd:Z:RD", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT/AB_:n14_1:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R/A:abcd:ZR", 9));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();
    _buildSecondaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_2:A_:RD/H:b2:A_:RD/I_:h8:E_:BK/B_:e5:E_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AF_:m13:I_L:RD/AD:j10:AFL:RD", 9));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    (void)mpAuxSearchTree->tryRemoveNode("AD");
    QVERIFY(*mpSearchTree == *dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()));

    (void)mpSearchTree->tryRemoveNode("B_");
    (void)mpAuxSearchTree->tryRemoveNode("B_");
    QVERIFY(*mpSearchTree != *dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_L/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L", 7));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/I_:h8:A_:RD/H:b2:A_:RD/AF_:m13:I_:BK/E_:a1_2:I_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK", 7));

    // Red-Black tree vs. AVL tree

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::move(*dynamic_cast<StrIntStrRedBlackTree*>(mpSearchTree.get())));

    mpSearchTree = std::make_unique<StrIntStrAVLTree>();
    _buildPrimaryTestSearchTree();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:RD/AF:i9_1:B:BK/BC_:k11:AB_:BK/E_:a1_1:AB_:BK/G:f6:AF:RD/AG:l12:AF:RD/Z:g7_1:E_R:RD", 8));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(*mpSearchTree == *dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()));

    (void)mpSearchTree->tryInsertNode("AB_", "n14_1");
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);
    QVERIFY(*mpSearchTree != *dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()));

    (void)mpAuxSearchTree->tryInsertNode("AB_", "n14_1");
    QVERIFY(*dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()) == *dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()));
    QVERIFY(*dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()) == *mpAuxSearchTree);

    (void)mpAuxSearchTree->tryInsertNode("BE", "abcd");
    QVERIFY(*dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()) != *dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()));
    QVERIFY(*dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()) != *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/AB_:n14_1:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14_1:B:RD/AF:i9_1:B:RD/BC_:k11:AB_:BK/E_:a1_1:AB_:BK/G:f6:AF:BK/AG:l12:AF:BK/Z:g7_1:E_R:RD/BE:abcd:AGR:RD", 9));

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(*dynamic_cast<StrIntStrAVLTree*>(mpAuxSearchTree.get()));

    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>();
    _buildSecondaryTestSearchTree();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_2:A_:RD/H:b2:A_:RD/I_:h8:E_:BK/B_:e5:E_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AF_:m13:I_L:RD/AD:j10:AFL:RD", 9));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);
    QVERIFY(*mpAuxSearchTree == *dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()));

    (void)mpSearchTree->tryRemoveNode("B_");
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);
    QVERIFY(*mpAuxSearchTree != *dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()));

    (void)mpAuxSearchTree->tryRemoveNode("B_");
    QVERIFY(*dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()) == *dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()));
    QVERIFY(*dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()) == *mpSearchTree);

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(*dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()) != *dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()));
    QVERIFY(*dynamic_cast<StrIntStrRedBlackTree*>(mpAuxSearchTree.get()) != *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/I_:h8:A_/H:b2:A_/AF_:m13:I_/E_:a1_2:I_/Z:g7_2:H/AF:i9_2:H", 7));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT:BK/I_:h8:A_:RD/H:b2:A_:RD/AF_:m13:I_:BK/E_:a1_2:I_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AD:j10:AFL:RD", 8));

    // AVL tree vs. Simple BST

    mpSearchTree = std::make_unique<StrIntStrAVLTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>();
    _buildPrimaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    (void)mpSearchTree->tryInsertNode("AB_", "n14_1");
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);

    (void)mpAuxSearchTree->tryInsertNode("AB_", "n14_1");
    QVERIFY(*mpAuxSearchTree == *dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()));

    (void)mpSearchTree->tryInsertNode("A", "abcd");
    (void)mpAuxSearchTree->tryInsertNode("A", "abcd");
    QVERIFY(*mpAuxSearchTree != *dynamic_cast<StrIntStrAVLTree*>(mpSearchTree.get()));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT/AB_:n14_1:B/AF:i9_1:B/BC_:k11:AB_/Z:g7_1:AB_/G:f6:AF/AG:l12:AF/E_:a1_1:Z/A:abcd:Z", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT/AB_:n14_1:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R/A:abcd:ZR", 9));

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();
    _buildSecondaryTestSearchTree();
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    (void)mpSearchTree->tryRemoveNode("AD");
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);

    (void)mpAuxSearchTree->tryRemoveNode("AD");
    QVERIFY(*dynamic_cast<StrIntStrAVLTree*>(mpAuxSearchTree.get()) == *mpSearchTree);

    (void)mpSearchTree->tryRemoveNode("B_");
    (void)mpAuxSearchTree->tryRemoveNode("B_");
    QVERIFY(*dynamic_cast<StrIntStrAVLTree*>(mpAuxSearchTree.get()) != *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_L/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L", 7));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/I_:h8:A_/H:b2:A_/AF_:m13:I_/E_:a1_2:I_/Z:g7_2:H/AF:i9_2:H", 7));

    // additional tests
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "A_", "C"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"A", "A_", "C"}, scDefaultValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "D"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "E_"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "B_"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "H"}, scDefaultValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "D"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "E"}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "D_"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "E_"}, scDefaultValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "A_", "C"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"B", "A_", "C"}, scDefaultValue, scCustomNullValue);
    mpSearchTree->tryInsertNode("B", "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "D_"}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "D_"}, scDefaultValue);
    mpAuxSearchTree->tryInsertNode("D_", "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "E"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(std::vector<StringifiedInteger>{"B", "A_", "C", "E"}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree->tryInsertNode("E", "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);
}

void StrIntKeyMixedTreeTypesTests::testCopyAssignmentOfMixedTreeTypes()
{
    // AVL tree assigned to Red-Black tree (and vice-versa)
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_2:A_:RD/H:b2:A_:RD/I_:h8:E_:BK/B_:e5:E_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AF_:m13:I_L:RD/AD:j10:AFL:RD", 9));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));

    /* simulate how the destination tree would be built by inserting the keys of the source tree in (top-down/left-right) order
       (no need to take the values into consideration so default values are being used for simplicity)
    */
    QVERIFY("A_:ROOT:BK/E_:A_:RD/H:A_:RD/I_:E_:BK/B_:E_:BK/Z:H:BK/AF:H:BK/AF_:I_L:RD/AD:AFL:RD" ==
            StrIntStrRedBlackTree(std::vector<StringifiedInteger>{"A_", "E_", "H", "I_", "B_", "Z", "AF", "AF_", "AD"}, scDefaultValue).getTreeAsString(false));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:RD/AF:i9_1:B:BK/BC_:k11:AB_:BK/E_:a1_1:AB_:BK/G:f6:AF:RD/AG:l12:AF:RD/Z:g7_1:E_R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("B:ROOT/AB_:B/AF:B/BC_:AB_/E_:AB_/G:AF/AG:AF/Z:E_R" ==
            StrIntStrAVLTree(std::vector<StringifiedInteger>{"B", "AB_", "AF", "BC_", "E_", "G", "AG", "Z"}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    // Simple BST assigned to Red-Black tree (and vice-versa)

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_2:A_:RD/H:b2:A_:RD/I_:h8:E_:BK/B_:e5:E_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AF_:m13:I_L:RD/AD:j10:AFL:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "H:b2:ROOT/A_:c3:H/AF:i9_2:H/B_:e5:A_/Z:g7_2:A_/AD:j10:AFL/E_:a1_2:B_L/I_:h8:E_L/AF_:m13:I_L", 9));

    QVERIFY("A_:ROOT:BK/E_:A_:RD/H:A_:RD/I_:E_:BK/B_:E_:BK/Z:H:BK/AF:H:BK/AF_:I_L:RD/AD:AFL:RD" ==
            StrIntStrRedBlackTree(std::vector<StringifiedInteger>{"H", "A_", "AF", "B_", "Z", "AD", "E_", "I_", "AF_"}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "B:d4:ROOT:BK/AB_:n14:B:RD/AF:i9_1:B:BK/BC_:k11:AB_:BK/E_:a1_1:AB_:BK/G:f6:AF:RD/AG:l12:AF:RD/Z:g7_1:E_R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("B:ROOT/AB_:B/AF:B/BC_:AB_/E_:AB_/G:AF/AG:AF/Z:E_R" ==
            StrIntStrBinarySearchTree(std::vector<StringifiedInteger>{"B", "AB_", "AF", "BC_", "E_", "G", "AG", "Z"}, scDefaultValue).getTreeAsString(false));

    // AVL tree assigned to Simple BST (and vice-versa)
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));

    QVERIFY("A_:ROOT/E_:A_/H:A_/I_:E_/B_:E_/Z:H/AF:H/AF_:I_L/AD:AFL" ==
            StrIntStrBinarySearchTree(std::vector<StringifiedInteger>{"A_", "E_", "H", "I_", "B_", "Z", "AF", "AF_", "AD"}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "E_:a1_1:ROOT/BC_:k11:E_/B:d4:E_/AB_:n14:BC_R/Z:g7_1:B/G:f6:B/AG:l12:GR/AF:i9_1:AGL", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "E_:a1_1:ROOT/BC_:k11:E_/B:d4:E_/AB_:n14:BC_R/Z:g7_1:B/AF:i9_1:B/G:f6:AF/AG:l12:AF", 8));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("E_:ROOT/BC_:E_/B:E_/AB_:BC_R/Z:B/AF:B/G:AF/AG:AF" ==
            StrIntStrAVLTree(std::vector<StringifiedInteger>{"E_", "BC_", "B", "AB_", "Z", "G", "AG", "AF"}, scDefaultValue).getTreeAsString(false));
}

/* Same test base used as for the copy assignment test
   The destination tree should be the same as when copy assigning while the source tree should become empty
   No in-order simulation by using default values (see copy assignment test) is required any longer
*/
void StrIntKeyMixedTreeTypesTests::testMoveAssignmentOfMixedTreeTypes()
{
    // AVL tree assigned to Red-Black tree (and vice-versa)
    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_2:A_:RD/H:b2:A_:RD/I_:h8:E_:BK/B_:e5:E_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AF_:m13:I_L:RD/AD:j10:AFL:RD", 9));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>();
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    // Simple BST assigned to Red-Black tree (and vice-versa)

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT:BK/E_:a1_2:A_:RD/H:b2:A_:RD/I_:h8:E_:BK/B_:e5:E_:BK/Z:g7_2:H:BK/AF:i9_2:H:BK/AF_:m13:I_L:RD/AD:j10:AFL:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree = std::make_unique<StrIntStrRedBlackTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<StrIntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "B:d4:ROOT/AB_:n14:B/AF:i9_1:B/BC_:k11:AB_/E_:a1_1:AB_/G:f6:AF/AG:l12:AF/Z:g7_1:E_R", 8));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // AVL tree assigned to Simple BST (and vice-versa)
    
    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "A_:c3:ROOT/E_:a1_2:A_/H:b2:A_/I_:h8:E_/B_:e5:E_/Z:g7_2:H/AF:i9_2:H/AF_:m13:I_L/AD:j10:AFL", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree = std::make_unique<StrIntStrBinarySearchTree>();
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<StrIntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "E_:a1_1:ROOT/BC_:k11:E_/B:d4:E_/AB_:n14:BC_R/Z:g7_1:B/AF:i9_1:B/G:f6:AF/AG:l12:AF", 8));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void StrIntKeyMixedTreeTypesTests::_buildPrimaryTestSearchTree()
{
    (void)mpSearchTree->tryInsertNode("E_", "a1_1");
    (void)mpSearchTree->tryInsertNode("B", "d4");
    (void)mpSearchTree->tryInsertNode("G", "f6");
    (void)mpSearchTree->tryInsertNode("BC_", "k11");
    (void)mpSearchTree->tryInsertNode("AG", "l12");
    (void)mpSearchTree->tryInsertNode("AB_", "n14");
    (void)mpSearchTree->tryInsertNode("AF", "i9_1");
    (void)mpSearchTree->tryInsertNode("Z", "g7_1");
}

void StrIntKeyMixedTreeTypesTests::_buildSecondaryTestSearchTree()
{
    (void)mpAuxSearchTree->tryInsertNode("H", "b2");
    (void)mpAuxSearchTree->tryInsertNode("A_", "c3");
    (void)mpAuxSearchTree->tryInsertNode("B_", "e5");
    (void)mpAuxSearchTree->tryInsertNode("Z", "g7_2");
    (void)mpAuxSearchTree->tryInsertNode("E_", "a1_2");
    (void)mpAuxSearchTree->tryInsertNode("AF", "i9_2");
    (void)mpAuxSearchTree->tryInsertNode("I_", "h8");
    (void)mpAuxSearchTree->tryInsertNode("AD", "j10");
    (void)mpAuxSearchTree->tryInsertNode("AF_", "m13");
}

void StrIntKeyMixedTreeTypesTests::_buildPrimaryTestSearchTreeInDifferentOrder()
{
    (void)mpAuxSearchTree->tryInsertNode("B", "d4");
    (void)mpAuxSearchTree->tryInsertNode("AB_", "n14");
    (void)mpAuxSearchTree->tryInsertNode("AF", "i9_1");
    (void)mpAuxSearchTree->tryInsertNode("BC_", "k11");
    (void)mpAuxSearchTree->tryInsertNode("E_", "a1_1");
    (void)mpAuxSearchTree->tryInsertNode("G", "f6");
    (void)mpAuxSearchTree->tryInsertNode("AG", "l12");
    (void)mpAuxSearchTree->tryInsertNode("Z", "g7_1");
}

void StrIntKeyMixedTreeTypesTests::_buildSecondaryTestSearchTreeInDifferentOrder()
{
    (void)mpSearchTree->tryInsertNode("A_", "c3");
    (void)mpSearchTree->tryInsertNode("E_", "a1_2");
    (void)mpSearchTree->tryInsertNode("H", "b2");
    (void)mpSearchTree->tryInsertNode("I_", "h8");
    (void)mpSearchTree->tryInsertNode("B_", "e5");
    (void)mpSearchTree->tryInsertNode("Z", "g7_2");
    (void)mpSearchTree->tryInsertNode("AF", "i9_2");
    (void)mpSearchTree->tryInsertNode("AF_", "m13");
    (void)mpSearchTree->tryInsertNode("AD", "j10");
}

QTEST_APPLESS_MAIN(StrIntKeyMixedTreeTypesTests)

#include "tst_strintkeymixedtreetypestests.moc"
// clang-format on
