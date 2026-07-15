module;

#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#ifdef PRINT_TREE
#include <sstream>
#endif

module binarysearchtree:binarysearchtree_impl;
import :binarysearchtree_header;

template <BSTKey K, BSTValue V>
BinarySearchTree<K, V>::BinarySearchTree(const V& nullValue)
    : m_Root{nullptr}
    , m_NullValue{nullValue}
    , m_Size{0}
{
}

template <BSTKey K, BSTValue V>
BinarySearchTree<K, V>::BinarySearchTree(const std::vector<K>& inputKeys, const V& defaultValue, const V& nullValue)
    : BinarySearchTree{nullValue}
{
    if (!inputKeys.empty() && defaultValue != nullValue)
    {
        // temporary object is required in order to avoid directly calling _createTreeStructure() which contains calls
        // to virtual methods
        BinarySearchTree temp{nullValue};
        temp._createTreeStructure(inputKeys, defaultValue, nullValue);

        // move temporary object to current object
        *this = std::move(temp);
    }
}
template <BSTKey K, BSTValue V>
BinarySearchTree<K, V>::BinarySearchTree(const BinarySearchTree& sourceTree)
    : BinarySearchTree{sourceTree.m_NullValue}
{
    // temporary object is required in order to avoid directly calling _copyTreeNodes() which contains calls to virtual
    // methods
    BinarySearchTree temp{sourceTree.m_NullValue};
    temp = sourceTree;

    // move temporary object to current object
    *this = std::move(temp);
}

template <BSTKey K, BSTValue V> BinarySearchTree<K, V>::BinarySearchTree(BinarySearchTree&& sourceTree)
{
    _moveAssignTree(sourceTree);
}

template <BSTKey K, BSTValue V> bool BinarySearchTree<K, V>::tryInsertNode(const K& key, const V& value)
{
    bool newNodeAdded{false};

    if (value != m_NullValue)
    {
        spNode const addedNode{_doInsertOrUpdateNode(key, value)};
        newNodeAdded = addedNode != nullptr;
    }

    return newNodeAdded;
}

template <BSTKey K, BSTValue V> bool BinarySearchTree<K, V>::tryRemoveNode(const K& key)
{
    bool removed{false};
    spNode nodeToRemove{_findNode(key)};

    if (nodeToRemove)
    {
        // for two-children node to be removed: the in-order successor content will be copied to the node; then the
        // successor will be recursively removed
        if (nodeToRemove->getLeftChild() && nodeToRemove->getRightChild())
        {
            nodeToRemove->copyInOrderSuccessorKeyAndValue();
            nodeToRemove = nodeToRemove->getInOrderSuccessor();
        }

        // no need for replacing node here, it's only required by (some of the) derived classes
        (void)_removeSingleChildedOrLeafNode(nodeToRemove);

        removed = true;
    }

    return removed;
}

template <BSTKey K, BSTValue V> bool BinarySearchTree<K, V>::mergeTree(BinarySearchTree& sourceTree)
{
    bool merged{false};

    if (m_NullValue == sourceTree.m_NullValue && this != &sourceTree)
    {
        _moveTreeNodes(sourceTree);
        merged = true;
    }

    return merged;
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::clear()
{
    m_Root.reset();
    m_Size = 0;
}

template <BSTKey K, BSTValue V>
BinarySearchTree<K, V>& BinarySearchTree<K, V>::operator=(const BinarySearchTree& sourceTree)
{
    if (this != &sourceTree)
    {
        clear();

        m_NullValue = sourceTree.m_NullValue;
        _copyTreeNodes(sourceTree);
    }

    return *this;
}

/* Cannot use _moveAssignTree() here because the nodes should be moved one by one in base BSTs.
   This is because a base BST pointer might point to a derived BST class and hence the source/destination root node
   pointers might have different node types behind. This would corrupt the data in case of a move operation -> see
   MixedTreeTypesTests::testMoveAssignmentOfMixedTreeTypes() for more details
*/
template <BSTKey K, BSTValue V> BinarySearchTree<K, V>& BinarySearchTree<K, V>::operator=(BinarySearchTree&& sourceTree)
{
    if (this != &sourceTree)
    {
        clear();

        m_NullValue = sourceTree.m_NullValue;
        _moveTreeNodes(sourceTree);
    }

    return *this;
}

template <BSTKey K, BSTValue V> bool BinarySearchTree<K, V>::operator==(const BinarySearchTree& tree) const
{
    bool areEqualTrees{true};

    if (this != &tree)
    {
        std::vector<spNode> nodes;
        std::vector<spNode> treeNodes;

        _convertTreeToArray(nodes);
        tree._convertTreeToArray(treeNodes);

        areEqualTrees = std::equal(nodes.cbegin(), nodes.cend(), treeNodes.cbegin(), treeNodes.cend(),
                                   [](const spNode& node, const spNode& treeNode) { return *node == *treeNode; });
    }

    return areEqualTrees;
}

template <BSTKey K, BSTValue V> V BinarySearchTree<K, V>::getNodeValue(const K& key) const
{
    spNode const foundNode{_findNode(key)};
    const V c_Result{foundNode ? foundNode->getValue() : m_NullValue};

    return c_Result;
}

template <BSTKey K, BSTValue V> V BinarySearchTree<K, V>::getNullValue() const
{
    return m_NullValue;
}

template <BSTKey K, BSTValue V> size_t BinarySearchTree<K, V>::getSize() const
{
    return m_Size;
}

#ifdef PRINT_TREE
template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::printTree() const
{
    std::vector<spNode> nodesArray;
    _convertTreeToArray(nodesArray);

    for (const auto& node : nodesArray)
    {
        std::cout << "Node: " << node->getKey();
        std::cout << " / Is child: " << (node->isLeftChild() ? "L" : node->isRightChild() ? "R" : "N");

        _printNodeRelatives(node);

        std::cout << std::endl;
    }
}

template <BSTKey K, BSTValue V> std::string BinarySearchTree<K, V>::getTreeAsString(bool areNodeValuesRequired) const
{
    std::string result;

    std::vector<spNode> nodesArray;
    _convertTreeToArray(nodesArray);

    for (const auto& node : nodesArray)
    {
        result += _getNodeAsString(node, areNodeValuesRequired);
        result += "/";
    }

    // trim the resulting string
    if (result.size() > 0)
    {
        result.pop_back();
    }

    return result;
}
#endif

template <BSTKey K, BSTValue V>
void BinarySearchTree<K, V>::_createTreeStructure(const std::vector<K>& inputKeys, const V& defaultValue,
                                                  const V& nullValue)
{
    if (defaultValue != nullValue)
    {
        for (const auto& key : inputKeys)
        {
            (void)_doInsertOrUpdateNode(key, defaultValue);
        }
    }
    else
    {
        assert(false && "Null value set as default value for tree keys");
    }
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::_copyTreeNodes(const BinarySearchTree& sourceTree)
{
    if (m_NullValue == sourceTree.m_NullValue)
    {
        std::vector<spNode> sourceTreeArray;
        sourceTree._convertTreeToArray(sourceTreeArray);

        for (const auto& node : sourceTreeArray)
        {
            (void)_doInsertOrUpdateNode(node->getKey(), node->getValue());
        }
    }
    else
    {
        assert(false && "Null values of trees don't match");
    }
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::_moveTreeNodes(BinarySearchTree& sourceTree)
{
    if (m_NullValue == sourceTree.m_NullValue)
    {
        if (this != &sourceTree)
        {
            _copyTreeNodes(sourceTree);
            sourceTree.clear();
        }
        else
        {
            assert(false && "The tree cannot move nodes within itself!");
        }
    }
    else
    {
        assert(false && "Null values of trees don't match");
    }
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::_moveAssignTree(BinarySearchTree& sourceTree)
{
    m_Root = sourceTree.m_Root;
    m_NullValue = sourceTree.m_NullValue;
    m_Size = sourceTree.m_Size;

    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::_setNullValue(const V& nullValue)
{
    m_NullValue = nullValue;
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::spNode BinarySearchTree<K, V>::_doInsertOrUpdateNode(const K& key, const V& value)
{
    spNode nodeToInsert{nullptr};

    // if the node doesn't exist (and therefore cannot be updated) an "insertion point" is created
    // a new node can then be created and inserted using the insertion point information
    // a non-existent insertion point (std::nullopt) means the node value update was successful
    const InsertionPoint newNodeInsertionPoint{_tryUpdateNode(key, value)};

    if (newNodeInsertionPoint)
    {
        nodeToInsert = _createNode(key, value);
        assert(nodeToInsert);

        _insertNode(nodeToInsert, newNodeInsertionPoint);
    }

    if (nodeToInsert)
    {
        ++m_Size;
    }

    return nodeToInsert;
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::spNode BinarySearchTree<K, V>::_removeSingleChildedOrLeafNode(spNode nodeToRemove)
{
    spNode replacingNode{nullptr};

    if (nodeToRemove)
    {
        spNode const leftChild{nodeToRemove->getLeftChild()};
        spNode const rightChild{nodeToRemove->getRightChild()};

        assert(!leftChild || !rightChild && "Attempt to use the remove function on a node with two children");

        // determine replacing node (null if leaf node is removed)
        replacingNode = leftChild ? leftChild : rightChild ? rightChild : nullptr;

        if (nodeToRemove->isLeftChild())
        {
            nodeToRemove->getParent()->setLeftChild(replacingNode);
        }
        else if (nodeToRemove->isRightChild())
        {
            nodeToRemove->getParent()->setRightChild(replacingNode);
        }
        else
        {
            if (replacingNode)
            {
                if (replacingNode->isLeftChild())
                {
                    nodeToRemove->setLeftChild(nullptr);
                }
                else
                {
                    nodeToRemove->setRightChild(nullptr);
                }

                replacingNode->decoupleFromParent();
            }

            m_Root = replacingNode;
        }

        nodeToRemove->decoupleFromParent(); // decouple removed node from parent
        --m_Size;
    }
    else
    {
        assert(false && "Attempt to remove a null node!");
    }

    return replacingNode;
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::spNode BinarySearchTree<K, V>::_createNode(const K& key, const V& value)
{
    return std::make_shared<Node>(key, value);
}

template <BSTKey K, BSTValue V>
void BinarySearchTree<K, V>::_insertNode(spNode nodeToInsert, const InsertionPoint& insertionPoint)
{
    if (nodeToInsert && insertionPoint)
    {
        auto& [parent, insertionSide]{*insertionPoint};

        switch (insertionSide)
        {
        case InsertionSide::LEFT: {
            assert(parent);
            if (parent)
            {
                parent->setLeftChild(nodeToInsert);
            }
            break;
        }
        case InsertionSide::RIGHT: {
            assert(parent);
            if (parent)
            {
                parent->setRightChild(nodeToInsert);
            }
            break;
        }
        case InsertionSide::ROOT: {
            assert(!parent);
            m_Root = nodeToInsert;
            break;
        }
        default:
            assert(false);
            break;
        }
    }
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::spNode BinarySearchTree<K, V>::_findNode(const K& key) const
{
    spNode foundNode{nullptr};
    spNode currentNode{m_Root};

    while (currentNode)
    {
        const K c_CurrentNodeKey{currentNode->getKey()};

        if (key < c_CurrentNodeKey)
        {
            currentNode = currentNode->getLeftChild();
        }
        else if (key > c_CurrentNodeKey)
        {
            currentNode = currentNode->getRightChild();
        }
        else
        {
            foundNode = currentNode;
            break;
        }
    }

    return foundNode;
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::_convertTreeToArray(std::vector<spNode>& nodes) const
{
    nodes.clear();

    if (m_Root)
    {
        nodes.push_back(m_Root);
        std::vector<spNode> parentsToCheck{m_Root};

        while (parentsToCheck.size() != 0)
        {
            std::vector<spNode> childrenToCheck;

            for (const auto& parent : parentsToCheck)
            {
                if (spNode const leftChild{parent->getLeftChild()}; leftChild)
                {
                    childrenToCheck.push_back(leftChild);
                }

                if (spNode const rightChild{parent->getRightChild()}; rightChild)
                {
                    childrenToCheck.push_back(rightChild);
                }
            }

            nodes.insert(nodes.end(), childrenToCheck.begin(), childrenToCheck.end());
            parentsToCheck = std::move(childrenToCheck);
        }
    }
}

// a right child node is required for left rotation
template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::_rotateNodeLeft(spNode node)
{
    if (node)
    {
        spNode const rightChild{node->getRightChild()};

        if (rightChild)
        {
            spNode const rightLeftChild{rightChild->getLeftChild()};
            spNode const parent{node->getParent()};

            // current node becomes left child of its actual right child
            rightChild->setLeftChild(node);

            // left child of actual right child becomes right child of current node
            node->setRightChild(rightLeftChild);

            // parent of current node (if any) becomes parent of actual right child (the new child remains same type of
            // child for parent as before)
            if (parent)
            {
                if (parent->getLeftChild() == node)
                {
                    parent->setLeftChild(rightChild);
                }
                else
                {
                    parent->setRightChild(rightChild);
                }
            }
            else
            {
                rightChild->decoupleFromParent();
            }

            if (m_Root == node)
            {
                m_Root = rightChild;
            }
        }
    }
}

// a left child node is required for right rotation
template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::_rotateNodeRight(spNode node)
{
    if (node)
    {
        spNode const leftChild{node->getLeftChild()};

        if (leftChild)
        {
            spNode const leftRightChild{leftChild->getRightChild()};
            spNode const parent{node->getParent()};

            // current node becomes right child of its actual left child
            leftChild->setRightChild(node);

            // right child of actual left child becomes left child of current node
            node->setLeftChild(leftRightChild);

            // parent of current node (if any) becomes parent of actual left child (the new child remains same type of
            // child for parent as before)
            if (parent)
            {
                if (parent->getLeftChild() == node)
                {
                    parent->setLeftChild(leftChild);
                }
                else
                {
                    parent->setRightChild(leftChild);
                }
            }
            else
            {
                leftChild->decoupleFromParent();
            }

            if (m_Root == node)
            {
                m_Root = leftChild;
            }
        }
    }
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::_printNodeRelatives(spNode node) const
{
    if (node)
    {
        auto printNodeRelativeInfo = [](const spNode node, const std::string& relativeName) {
            std::cout << " / " << relativeName << ": ";

            if (node)
            {
                std::cout << node->getKey();
            }
            else
            {
                std::cout << "N";
            }
        };

        printNodeRelativeInfo(node->getLeftChild(), "Left child");
        printNodeRelativeInfo(node->getRightChild(), "Right child");
        printNodeRelativeInfo(node->getParent(), "Parent");
        printNodeRelativeInfo(node->getSibling(), "Sibling");
        printNodeRelativeInfo(node->getUncle(), "Uncle");
        printNodeRelativeInfo(node->getGrandparent(), "Grandparent");
    }
    else
    {
        assert(false);
    }
}

#ifdef PRINT_TREE
template <BSTKey K, BSTValue V>
std::string BinarySearchTree<K, V>::_getNodeAsString(spNode node, bool isValueRequired) const
{
    std::string result{"NULL"};

    if (node)
    {
        std::ostringstream nodeData;
        spNode const parent{node->getParent()};

        nodeData << node->getKey() << ":";

        if (isValueRequired)
        {
            nodeData << node->getValue() << ":";
        }

        if (parent)
        {
            nodeData << parent->getKey();

            if (!node->getSibling())
            {
                nodeData << (node->isLeftChild() ? "L" : "R");
            }
        }
        else
        {
            nodeData << "ROOT";
        }

        result = nodeData.str();
    }

    return result;
}
#endif

template <BSTKey K, BSTValue V> typename BinarySearchTree<K, V>::spNode BinarySearchTree<K, V>::_getRoot() const
{
    return m_Root;
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::InsertionPoint BinarySearchTree<K, V>::_tryUpdateNode(const K& key,
                                                                                       const V& value) const
{
    InsertionPoint insertionPoint;

    if (m_Root)
    {
        for (spNode currentNode{m_Root};;)
        {
            if (const K c_CurrentNodeKey{currentNode->getKey()}; key < c_CurrentNodeKey)
            {
                if (spNode const leftChild{currentNode->getLeftChild()}; leftChild)
                {
                    currentNode = leftChild;
                    continue;
                }

                insertionPoint = {currentNode, InsertionSide::LEFT};
            }
            else if (key > c_CurrentNodeKey)
            {
                if (spNode const rightChild{currentNode->getRightChild()}; rightChild)
                {
                    currentNode = rightChild;
                    continue;
                }

                insertionPoint = {currentNode, InsertionSide::RIGHT};
            }
            else
            {
                currentNode->setValue(value);
            }

            break;
        }
    }
    else
    {
        insertionPoint = {nullptr, InsertionSide::ROOT};
    }

    return insertionPoint;
}

template <BSTKey K, BSTValue V> typename BinarySearchTree<K, V>::InOrderForwardIterator BinarySearchTree<K, V>::begin()
{
    spNode startingNode{m_Root};

    if (startingNode)
    {
        spNode currentLeftChild{startingNode->getLeftChild()};

        while (currentLeftChild)
        {
            startingNode = currentLeftChild;
            currentLeftChild = startingNode->getLeftChild();
        }
    }

    return InOrderForwardIterator{startingNode, m_NullValue};
}

template <BSTKey K, BSTValue V> typename BinarySearchTree<K, V>::InOrderForwardIterator BinarySearchTree<K, V>::end()
{
    return InOrderForwardIterator{nullptr, m_NullValue};
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::InOrderForwardIterator BinarySearchTree<K, V>::find(const K& key)
{
    spNode const currentNode{_findNode(key)};
    return InOrderForwardIterator{currentNode, m_NullValue};
}

template <BSTKey K, BSTValue V> typename BinarySearchTree<K, V>::InOrderForwardIterator BinarySearchTree<K, V>::root()
{
    return InOrderForwardIterator{m_Root, m_NullValue};
}

template <BSTKey K, BSTValue V>
BinarySearchTree<K, V>::Node::Node(const K& key, const V& value)
    : m_LeftChild{nullptr}
    , m_RightChild{nullptr}
    , m_Key{key}
    , m_Value{value}
{
    m_Parent.reset();
}

template <BSTKey K, BSTValue V> K BinarySearchTree<K, V>::Node::getKey() const
{
    return m_Key;
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::Node::setValue(const V& value)
{
    m_Value = value;
}

template <BSTKey K, BSTValue V> V BinarySearchTree<K, V>::Node::getValue() const
{
    return m_Value;
}

template <BSTKey K, BSTValue V> bool BinarySearchTree<K, V>::Node::isLeftChild() const
{
    spNode const parent{m_Parent.lock()};
    const bool c_IsNodeLeftChild{parent && parent->m_LeftChild.get() == this};

    return c_IsNodeLeftChild;
}

template <BSTKey K, BSTValue V> bool BinarySearchTree<K, V>::Node::isRightChild() const
{
    spNode const parent{m_Parent.lock()};
    const bool c_IsNodeRightChild{parent && parent->m_RightChild.get() == this};

    return c_IsNodeRightChild;
}

/* It is the responsibility of the tree object to ensure that the correct node is added as left child and that the tree
   rules are followed (e.g. don't add root as left child of a sub-node prior to decoupling it from its children */
template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::Node::setLeftChild(spNode leftChild)
{
    m_LeftChild = leftChild;

    if (m_LeftChild)
    {
        m_LeftChild->m_Parent = this->shared_from_this();
    }
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::Node::spNode BinarySearchTree<K, V>::Node::getLeftChild() const
{
    return m_LeftChild;
}

/* It is the responsibility of the tree object to ensure that the correct node is added as right child and that the tree
   rules are followed (e.g. don't add root as right child of a sub-node prior to decoupling it from its children */
template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::Node::setRightChild(spNode rightChild)
{
    m_RightChild = rightChild;

    if (m_RightChild)
    {
        m_RightChild->m_Parent = this->shared_from_this();
    }
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::Node::spNode BinarySearchTree<K, V>::Node::getRightChild() const
{
    return m_RightChild;
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::Node::copyInOrderSuccessorKeyAndValue()
{
    spNode const successor{getInOrderSuccessor()};

    if (successor)
    {
        m_Key = successor->m_Key;
        m_Value = successor->m_Value;
    }
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::Node::spNode BinarySearchTree<K, V>::Node::getInOrderSuccessor() const
{
    spNode inOrderSuccessor{m_RightChild};

    if (m_RightChild)
    {
        spNode successorChild{inOrderSuccessor->m_LeftChild};

        while (successorChild)
        {
            inOrderSuccessor = successorChild;
            successorChild = inOrderSuccessor->m_LeftChild;
        }
    }

    return inOrderSuccessor;
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::Node::decoupleFromParent()
{
    m_Parent = spNode{nullptr};
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::Node::spNode BinarySearchTree<K, V>::Node::getParent() const
{
    return m_Parent.lock();
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::Node::spNode BinarySearchTree<K, V>::Node::getSibling() const
{
    spNode result{nullptr};

    if (spNode const parent{m_Parent.lock()}; parent)
    {
        result = parent->m_LeftChild.get() != this ? parent->m_LeftChild : parent->m_RightChild;
    }

    return result;
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::Node::spNode BinarySearchTree<K, V>::Node::getUncle() const
{
    spNode result{nullptr};

    if (spNode const grandParent{getGrandparent()}; grandParent)
    {
        result = grandParent->m_LeftChild.get() != m_Parent.lock().get() ? grandParent->m_LeftChild
                                                                         : grandParent->m_RightChild;
    }

    return result;
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::Node::spNode BinarySearchTree<K, V>::Node::getGrandparent() const
{
    spNode result{nullptr};

    if (spNode const parent{m_Parent.lock()}; parent)
    {
        result = parent->getParent();
    }

    return result;
}

/* Two nodes are equal if all below conditions are fulfilled:
- they have the same key and value
- they are both in the same ordering position: root, left child or right child
- if they are both parented: the parent has the same key (not necessarily the same value)
*/
template <BSTKey K, BSTValue V> bool BinarySearchTree<K, V>::Node::operator==(const BinarySearchTree::Node& node) const
{
    bool areEqualNodes{false};

    if (m_Key == node.m_Key && m_Value == node.m_Value)
    {
        spNode const parent{m_Parent.lock()};
        spNode const nodeParent{node.m_Parent.lock()};

        areEqualNodes = (!parent && !nodeParent) || (parent && nodeParent && (parent->m_Key == nodeParent->m_Key) &&
                                                     (isLeftChild() == node.isLeftChild()));
    }

    return areEqualNodes;
}

template <BSTKey K, BSTValue V> BinarySearchTree<K, V>::InOrderForwardIterator::InOrderForwardIterator()
{
    m_Node.reset();
}

template <BSTKey K, BSTValue V>
BinarySearchTree<K, V>::InOrderForwardIterator::InOrderForwardIterator(spNode node, const V& nullValue)
    : m_NullValue{nullValue}
{
    m_Node = node;
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::InOrderForwardIterator::next()
{
    if (spNode const node{m_Node.lock()}; node)
    {
        spNode nextNode{nullptr};
        spNode currentChild{node->getRightChild()};

        // first check downwards by moving to the right child and then checking on the left side until no more left
        // child is found
        while (currentChild)
        {
            nextNode = currentChild;
            currentChild = nextNode->getLeftChild();
        }

        // if no suitable child was found check upwards until a suitable parent (to which current node is right child)
        // is found
        if (!nextNode)
        {
            spNode currentNode{node};
            spNode currentParent{node->getParent()};

            // if no suitable parent is found then the end of the tree has been reached
            while (currentParent)
            {
                if (currentNode->isLeftChild())
                {
                    nextNode = currentParent;
                    break;
                }

                currentNode = currentParent;
                currentParent = currentNode->getParent();
            }
        }

        m_Node = nextNode;
    }
}

template <BSTKey K, BSTValue V> std::optional<K> BinarySearchTree<K, V>::InOrderForwardIterator::getKey() const
{
    std::optional<K> result;

    if (spNode const node{m_Node.lock()}; node)
    {
        result = node->getKey();
    }

    return result;
}

template <BSTKey K, BSTValue V> void BinarySearchTree<K, V>::InOrderForwardIterator::setValue(const V& value)
{
    if (spNode const node{m_Node.lock()}; node)
    {
        node->setValue(value);
    }
}

template <BSTKey K, BSTValue V> V BinarySearchTree<K, V>::InOrderForwardIterator::getValue() const
{
    V result{m_NullValue};

    if (spNode const node{m_Node.lock()}; node)
    {
        result = node->getValue();
    }

    return result;
}

template <BSTKey K, BSTValue V>
typename BinarySearchTree<K, V>::InOrderForwardIterator& BinarySearchTree<K, V>::InOrderForwardIterator::operator=(
    const InOrderForwardIterator& other)
{
    m_Node = other.m_Node;
    m_NullValue = other.m_NullValue;

    return *this;
}

template <BSTKey K, BSTValue V>
bool BinarySearchTree<K, V>::InOrderForwardIterator::operator==(const InOrderForwardIterator& other) const
{
    return m_Node.lock() == other.m_Node.lock() && m_NullValue == other.m_NullValue;
}
