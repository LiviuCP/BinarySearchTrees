/*
Red-black tree nodes rules:
1) Each node is either RED or BLACK
2) The root is ALWAYS black
3) No two consecutive red nodes are allowed
4) The number of black roots from root to each NULL leaf (including root node) should be the same

Other conventions:
- each newly added node has initial color red (it could change after in order to match the above rules)
- the NULL leaf nodes are always considered black

Notes:
- no node can have a single black child (rule 4 violation)
- if a black node has a single red child it cannot have any grandchildren (red grandchild(ren): rule 3 violation, black
grandchildren: rule 4 violation)
- a black node can have mixed children (one black, one red) if the red node has two black children (the black child
could also have for example 0, 1 or 2 red children)
*/

module;

#include <memory>
#include <vector>

export module redblacktree:redblacktree_header;
export import binarysearchtree;

export template <typename K, typename V> class RedBlackTree final : public BinarySearchTree<K, V>
{
public:
    RedBlackTree(const V& nullValue = {});
    RedBlackTree(const std::vector<K>& inputKeys, const V& defaultValue, const V& nullValue = {});
    RedBlackTree(const RedBlackTree& sourceTree);
    RedBlackTree(RedBlackTree&& sourceTree);

    RedBlackTree& operator=(const RedBlackTree& sourceTree);
    RedBlackTree& operator=(RedBlackTree&& sourceTree);
#ifdef PRINT_TREE
    void printTree() const override;
#endif
private:
    class RedBlackNode : public BinarySearchTree<K, V>::Node
    {
    public:
        using spRBNode = std::shared_ptr<RedBlackNode>;

        RedBlackNode(const K& key, const V& value);

        void setBlack(bool isBlackRequired);
        bool isBlack() const;

    private:
        bool m_IsBlack;
    };

    using spRBNode = typename RedBlackNode::spRBNode;

    // design decision: any assignment operator to work only between trees of same type
    using BinarySearchTree<K, V>::operator=;

    typename BinarySearchTree<K, V>::spNode _removeSingleChildedOrLeafNode(
        typename BinarySearchTree<K, V>::spNode nodeToRemove) override;
    typename BinarySearchTree<K, V>::spNode _createNode(const K& key, const V& value) override;
    void _insertNode(typename BinarySearchTree<K, V>::spNode nodeToInsert,
                     const typename BinarySearchTree<K, V>::InsertionPoint& insertionPoint) override;
#ifdef PRINT_TREE
    std::string _getNodeAsString(typename BinarySearchTree<K, V>::spNode node, bool isValueRequired) const override;
#endif
};
