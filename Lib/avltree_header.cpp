/* This class implements an AVL binary search tree
   The AVL is a balanced tree that has following property: the height difference of any two leaf nodes is maximum 1
*/

module;

#include <memory>
#include <vector>

export module avltree:avltree_header;
export import binarysearchtree;

export template <typename K, typename V> class AVLTree final : public BinarySearchTree<K, V>
{
public:
    AVLTree(const V& nullValue = {});
    AVLTree(const std::vector<K>& inputKeys, const V& defaultValue, const V& nullValue = {});
    AVLTree(const AVLTree& sourceTree);
    AVLTree(AVLTree&& sourceTree);

    AVLTree& operator=(const AVLTree& sourceTree);
    AVLTree& operator=(AVLTree&& sourceTree);

private:
    class AVLNode : public BinarySearchTree<K, V>::Node
    {
    public:
        using spAVLNode = std::shared_ptr<AVLNode>;

        AVLNode(const K& key, const V& value);

        void updateHeight();

        bool isBalanced() const;
        spAVLNode getGreaterHeightChild() const;

    private:
        short m_Height;
    };

    using spAVLNode = typename AVLNode::spAVLNode;

    // design decision: any assignment operator to work only between trees of same type
    using BinarySearchTree<K, V>::operator=;

    typename BinarySearchTree<K, V>::spNode _removeSingleChildedOrLeafNode(
        typename BinarySearchTree<K, V>::spNode nodeToRemove) override;
    typename BinarySearchTree<K, V>::spNode _createNode(const K& key, const V& value) override;
    void _insertNode(typename BinarySearchTree<K, V>::spNode nodeToInsert,
                     const typename BinarySearchTree<K, V>::InsertionPoint& insertionPoint) override;

    // update all ancestors of a specific node up to the root
    void _updateAncestorHeights(spAVLNode node);

    spAVLNode _balanceSubtree(spAVLNode grandparent, spAVLNode parent, spAVLNode child);
};
