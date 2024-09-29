#include <iostream>
#include <vector>

using namespace std;

template<typename Key, typename Value>
struct Node {
    Key key;
    Value value;
    Node *left;
    Node *right;
    int depth;
    int sign;
    Key minKey;
    Key maxKey;

    Node(Key key, Value value)
            : key(key), value(value), left(nullptr), right(nullptr), depth(1), sign(0), minKey(key), maxKey(key) {}
};

template<typename Key, typename Val = Key, typename Q = Key>
class BinaryTree {
public:
    Node<Key, Val> *root = nullptr;


    Node<Key, Val> *find(Key key) {
        return find(root, key);
    }


    Node<Key, Val> *min() {
        if (root == nullptr) {
            return nullptr;
        }
        return subtreeMin(root);
    }

    Node<Key, Val> *max() {
        if (root == nullptr) {
            return nullptr;
        }
        return subtreeMax(root);
    }

    Node<Key, Val> *nextBigger(Key key) {
        return nextBigger(root, key);

    }

    Node<Key, Val> *nextSmaller(Key key) {
        return nextSmaller(root, key);

    }

    virtual void insert(Key key, Val val = Val()) {
        root = insert(root, key, val);
    }

    void remove(Key key) {
        root = remove(root, key);
    }

    pair<bool, Q> rangeQuery(Key start, Key end) {
        return rangeQuery(root, start, end);
    }

    bool check_signs(Node<Key, Val> *node) {
        if (node == nullptr) {
            return true;
        }
        return check_signs(node->left) && (abs(node->sign) <= 1) && check_signs(node->right);
    }

    void printTree(Node<Key, Val> *node) {
        if (node != nullptr) {
            cout << node->key << " ";
            printTree(node->left);
            printTree(node->right);
        }
    }

    void refresh(Key key) {
        refresh(root, key);
    }

protected:
    pair<bool, Q> rangeQuery(Node<Key, Val> *node, Key start, Key end) {
        if (node == nullptr) {
            return {false, Q()};
        }
        if (!(start <= node->maxKey && node->minKey <= end))
            return {false, Q()};
        if (start <= node->minKey and node->maxKey <= end)
            return {true, evaluateSubtree(node)};
        pair<bool, Q> a = rangeQuery(node->left, start, end);
        pair<bool, Q> b = rangeQuery(node->right, start, end);
        if (start <= node->key && node->key <= end) {
            Q m = evaluateNode(node);
            if (a.first)
                m = queryOperator(a.second, m);
            if (b.first)
                m = queryOperator(m, b.second);
            return {true, m};
        }
        if (a.first)
            return {true, a.second};
        if (b.first)
            return {true, b.second};
        return {false, Q()};
    }

    virtual void updateValue(Node<Key, Val> *node) {
        // User can replace this with own desired function
    }

    virtual Q evaluateSubtree(Node<Key, Val> *node) {
        return Q();
    }

    virtual Q evaluateNode(Node<Key, Val> *node) {
        return Q();
    }

    virtual Q queryOperator(Q q1, Q q2) {
        return Q();
    }


    void updateSignAndDepth(Node<Key, Val> *node) {
        int rightDepth = 0;
        int leftDepth = 0;

        if (node->right != nullptr) {
            rightDepth = node->right->depth;
        }
        if (node->left != nullptr) {
            leftDepth = node->left->depth;
        }
        node->depth = leftDepth;
        if (leftDepth < rightDepth)
            node->depth = rightDepth;
        node->depth++;
        node->sign = rightDepth - leftDepth;
    }

    void updateAll(Node<Key, Val> *node) {
        updateSignAndDepth(node);
        updateValue(node);
        updateMinMaxKey(node);
    }

    void updateMinMaxKey(Node<Key, Val> *node) {
        node->maxKey = node->key;
        node->minKey = node->key;
        if (node->left != nullptr) {
            if (node->minKey > node->left->minKey) {
                node->minKey = node->left->minKey;
            }
        }
        if (node->right != nullptr) {
            if (node->maxKey < node->right->maxKey) {
                node->maxKey = node->right->maxKey;
            }
        }
    }

    void refresh(Node<Key, Val> *node, Key key) {
        if (node == nullptr) {
            return;
        }

        if (node->key < key) {
            refresh(node->right, key);
        } else if (key < node->key) {
            refresh(node->left, key);
        }
        updateValue(node);
    }

    Node<Key, Val> *find(Node<Key, Val> *node, Key key) const {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key == key) {
            return node;
        }
        if (node->key < key) {
            return find(node->right, key);
        } else {
            return find(node->left, key);
        }
    }

    Node<Key, Val> *insert(Node<Key, Val> *node, Key key, Val val = Val()) {
        if (node == nullptr) {
            Node<Key, Val> *newNode = new Node<Key, Val>{key, val};
            updateAll(newNode);
            return newNode;

        }
        if (key < node->key) {
            node->left = insert(node->left, key, val);
            updateAll(node);

            if (node->sign == -2) {
                if (node->left->sign == -1) {
                    return left_rotation(node);
                }
                if (node->left->sign == 1) {
                    return left_double_rotation(node);
                }
            }
        }
        if (node->key < key) {
            node->right = insert(node->right, key, val);
            updateAll(node);

            if (node->sign == 2) {
                if (node->right->sign == 1) {
                    return right_rotation(node);
                }
                if (node->right->sign == -1) {
                    return right_double_rotation(node);
                }
            }
        }
        return node;
    }

    Node<Key, Val> *remove(Node<Key, Val> *node, Key key, bool deleteNode = true) {
        if (node == nullptr) {
            return nullptr;
        }
        if (key < node->key) {
            node->left = remove(node->left, key);
            updateAll(node);
            if (node->sign == 2) {
                if (node->right->sign >= 0) {
                    return right_rotation(node);
                }
                if (node->right->sign == -1) {
                    return right_double_rotation(node);
                }
            }
            return node;
        }
        if (node->key < key) {
            node->right = remove(node->right, key);
            updateAll(node);
            if (node->sign == -2) {
                if (node->left->sign <= 0) {
                    return left_rotation(node);
                }
                if (node->left->sign == 1) {
                    return left_double_rotation(node);
                }
            }
            return node;
        } else {
            if (node->right == nullptr && node->left == nullptr) {
                delete node;

                return nullptr;
            }
            if (node->left != nullptr && node->right != nullptr) {
                Node<Key, Val> *s = subtreeMin(node->right);
                Node<Key, Val> *successor = new Node<Key, Val>{Key(s->key), Val(s->value)};
                node->right = remove(node->right, successor->key);
                successor->right = node->right;
                successor->left = node->left;
                updateAll(successor);
                if (deleteNode) {
                    delete node;
                }
                if (successor->sign == -2) {
                    if (successor->left->sign <= 0) {
                        return left_rotation(successor);
                    }
                    if (successor->left->sign == 1) {
                        return left_double_rotation(successor);
                    }
                }
                return successor;

            } else {
                Node<Key, Val> *successor;
                if (node->left == nullptr) {
                    successor = node->right;
                } else {
                    successor = node->left;
                }
                delete node;
                return successor;

            }
        }
    }

    Node<Key, Val> *subtreeMin(Node<Key, Val> *node) const {
        if (node->left == nullptr) {
            return node;
        }
        return subtreeMin(node->left);
    }

    Node<Key, Val> *subtreeMax(Node<Key, Val> *node) const {
        if (node->right == nullptr) {
            return node;
        }
        return subtreeMax(node->right);
    }

    Node<Key, Val> *nextBigger(Node<Key, Val> *node, Key key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key < key) {
            return nextBigger(node->right, key);
        } else if (key < node->key) {
            Node<Key, Val> *a = nextBigger(node->left, key);
            if (a != nullptr) {
                if (a->key < node->key) {
                    return a;
                }
            }
            return node;
        } else {
            if (node->right != nullptr) {
                return subtreeMin(node->right);
            }
            return nullptr;
        }

    }

    Node<Key, Val> *nextSmaller(Node<Key, Val> *node, Key key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key > key) {
            return nextSmaller(node->left, key);
        } else if (key > node->key) {
            Node<Key, Val> *a = nextSmaller(node->right, key);
            if (a != nullptr) {
                if (a->key > node->key) {
                    return a;
                }
            }
            return node;
        } else {
            if (node->left != nullptr) {
                return subtreeMax(node->left);
            }
            return nullptr;
        }

    }

    Node<Key, Val> *left_rotation(Node<Key, Val> *y) {
        Node<Key, Val> *x = y->left;
        Node<Key, Val> *B = x->right;

        x->right = y;
        y->left = B;

        updateAll(y);
        updateAll(x);
        return x;
    }

    Node<Key, Val> *left_double_rotation(Node<Key, Val> *z) {
        Node<Key, Val> *x = z->left;
        Node<Key, Val> *y = x->right;

        Node<Key, Val> *B = y->left;
        Node<Key, Val> *C = y->right;

        y->left = x;
        y->right = z;
        x->right = B;
        z->left = C;

        updateAll(x);
        updateAll(z);
        updateAll(y);
        return y;
    }

    Node<Key, Val> *right_rotation(Node<Key, Val> *x) {
        Node<Key, Val> *y = x->right;
        Node<Key, Val> *B = y->left;

        y->left = x;
        x->right = B;

        updateAll(x);
        updateAll(y);
        return y;
    }

    Node<Key, Val> *right_double_rotation(Node<Key, Val> *z) {
        Node<Key, Val> *x = z->right;
        Node<Key, Val> *y = x->left;

        Node<Key, Val> *B = y->left;
        Node<Key, Val> *C = y->right;

        y->left = z;
        y->right = x;
        x->left = C;
        z->right = B;

        updateAll(x);
        updateAll(z);
        updateAll(y);
        return y;
    }


};

