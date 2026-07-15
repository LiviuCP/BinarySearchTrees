module;

#include <memory>
#include <optional>
#include <string>
#include <vector>

export module binarysearchtree:binarysearchtree_header;

#ifdef PRINT_TREE
template <typename T>
concept OutStreamable = requires(std::ostream ostr, T data) {
    {
        ostr << data
    } -> std::same_as<std::ostream&>;
};

template <typename T>
concept BSTKey = std::copy_constructible<T> && std::totally_ordered<T> && OutStreamable<T>;
template <typename T>
concept BSTValue =
    std::default_initializable<T> && std::copy_constructible<T> && std::is_copy_assignable_v<T> && OutStreamable<T>;
#else
template <typename T>
concept BSTKey = std::copy_constructible<T> && std::totally_ordered<T>;
template <typename T>
concept BSTValue = std::default_initializable<T> && std::copy_constructible<T> && std::is_copy_assignable_v<T>;
#endif

export template <BSTKey K, BSTValue V> class BinarySearchTree
{
public:
    BinarySearchTree(const V& nullValue = {});
    BinarySearchTree(const std::vector<K>& inputKeys, const V& defaultValue, const V& nullValue = {});
    BinarySearchTree(const BinarySearchTree& sourceTree);
    BinarySearchTree(BinarySearchTree&& sourceTree);

    bool tryInsertNode(const K& key, const V& value); // in actual implementation(s) true is returned if new node is
                                                      // inserted (number of nodes increases)
    bool tryRemoveNode(const K& key); // in actual implementation(s) true is returned if the node with the given key
                                      // exists within tree structure (and thus is removed)

    bool mergeTree(BinarySearchTree& sourceTree);
    void clear();

    BinarySearchTree& operator=(const BinarySearchTree& sourceTree);
    BinarySearchTree& operator=(BinarySearchTree&& sourceTree);

    bool operator==(const BinarySearchTree& tree) const;

    V getNodeValue(const K& key) const;
    V getNullValue() const;
    size_t getSize() const;
#ifdef PRINT_TREE
    virtual void printTree() const;
    virtual std::string getTreeAsString(bool areNodeValuesRequired = false) const;
#endif

protected:
    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        using spNode = std::shared_ptr<Node>;
        using wpNode = std::weak_ptr<Node>;

        Node(const K& key, const V& value);
        virtual ~Node() = default;

        K getKey() const;

        void setValue(const V& value);
        V getValue() const;

        bool isLeftChild() const;
        bool isRightChild() const;

        void setLeftChild(spNode leftChild);
        spNode getLeftChild() const;

        void setRightChild(spNode rightChild);
        spNode getRightChild() const;

        void copyInOrderSuccessorKeyAndValue();
        spNode getInOrderSuccessor() const;

        void decoupleFromParent();

        spNode getParent() const;
        spNode getSibling() const;
        spNode getUncle() const;
        spNode getGrandparent() const;

        bool operator==(const Node& node) const;

    private:
        wpNode m_Parent;
        spNode m_LeftChild;
        spNode m_RightChild;
        K m_Key;
        V m_Value;
    };

    using spNode = typename Node::spNode;
    using wpNode = typename Node::wpNode;

    enum class InsertionSide
    {
        LEFT,
        RIGHT,
        ROOT
    };

    using InsertionPoint = std::optional<std::pair<spNode, InsertionSide>>;

    void _createTreeStructure(const std::vector<K>& inputKeys, const V& defaultValue, const V& nullValue);
    void _copyTreeNodes(const BinarySearchTree& sourceTree);
    void _moveTreeNodes(BinarySearchTree& sourceTree);
    void _moveAssignTree(BinarySearchTree& sourceTree);
    void _setNullValue(const V& nullValue);

    spNode _doInsertOrUpdateNode(const K& key, const V& value);
    virtual spNode _removeSingleChildedOrLeafNode(spNode nodeToRemove);

    virtual spNode _createNode(const K& key, const V& value);
    virtual void _insertNode(spNode nodeToInsert, const InsertionPoint& insertionPoint);

    spNode _findNode(const K& key) const;
    void _convertTreeToArray(std::vector<spNode>& nodes) const;

    void _rotateNodeLeft(spNode node);
    void _rotateNodeRight(spNode node);

    void _printNodeRelatives(spNode node) const;
#ifdef PRINT_TREE
    virtual std::string _getNodeAsString(spNode node, bool isValueRequired) const;
#endif
    spNode _getRoot() const; // used by derived classes only

private:
    // tries to update the node value; if the node doesn't exist it returns an insertion point for a new node with the
    // given key; the new node will subsequently be created by using this point info
    InsertionPoint _tryUpdateNode(const K& key, const V& value) const;

    spNode m_Root;
    V m_NullValue; // value that each key that is NOT contained within tree corresponds to
    size_t m_Size; // used for easy retrieval of the number of nodes (to avoid tree traversal)

public:
    class InOrderForwardIterator final
    {
        friend class BinarySearchTree<K, V>;

    public:
        InOrderForwardIterator();

        void next();
        std::optional<K> getKey() const;

        void setValue(const V& value);
        V getValue() const;

        InOrderForwardIterator& operator=(const InOrderForwardIterator& other);
        bool operator==(const InOrderForwardIterator& other) const;

    private:
        InOrderForwardIterator(spNode node, const V& nullValue = {});

        wpNode m_Node;
        V m_NullValue;
    };

    InOrderForwardIterator begin();
    InOrderForwardIterator end();
    InOrderForwardIterator find(const K& key);
    InOrderForwardIterator root();
};
