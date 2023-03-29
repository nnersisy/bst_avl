#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void moveLeft(AVLNode<Key, Value>* num);
    void moveRight(AVLNode<Key, Value>* num);
    void helpInsert(AVLNode<Key, Value>* i, AVLNode<Key, Value>* j);
    void helpRemove(AVLNode<Key, Value>* num, int diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* newp = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);

    if(this->root_ == NULL)
    {
        this->root_ = newp; //do i need this??????
        newp->setBalance(0);
        return;
    }

    while(true)
    {
        if(newp->getKey() < temp->getKey()) //left
        {
            if(temp->getLeft() == NULL)
            {
                temp->setLeft(newp);
                newp->setParent(temp);
                break;
            }
            else
            {
                temp = temp->getLeft();
            }
        }
        else if(newp->getKey() > temp->getKey())//right
        {
            if(temp->getRight() == NULL)
            {
                temp->setRight(newp);
                newp->setParent(temp);
                break;
            }
            else
            {
                temp = temp->getRight();
            }

        }
        else
        {
					  temp->setValue(new_item.second); // added jjust nowwwwwwwwwwwwwwwwwwwwwwww
            delete newp;
            return;
        }
    }

    //point to parent ?????????????????????????????????
    if(temp->getBalance() == -1 || temp->getBalance() == 1)
    {
        temp->setBalance(0);
        return;
    }
    else
    {
        if(temp->getLeft() == newp) temp->setBalance(-1);
        else temp->setBalance(1);

        helpInsert(temp, newp);
    }

}

//INSERT HELPER
template<class Key, class Value>
void AVLTree<Key, Value>:: helpInsert(AVLNode<Key, Value>* i, AVLNode<Key, Value>* j)
{
    // TODO
    if(i == NULL || i->getParent() == NULL) return;

    AVLNode<Key, Value>* x = i->getParent();

    if(i == x->getLeft())
    {
        x->updateBalance(-1);

        if(x->getBalance() == 0) return;
        else if(x->getBalance() == -1) helpInsert(x, i);
        else if(x->getBalance() == -2)
        {
            if(i->getLeft() == j)
            {
                moveRight(x);
                i->setBalance(0);
                x->setBalance(0);
            }
            else if(i->getLeft() != j)
            {
                moveLeft(i);
                moveRight(x);

                if(j->getBalance() == -1)
                {
                    i->setBalance(0);
                    x->setBalance(1);
                }
                else if(j->getBalance() == 0)
                {
                    i->setBalance(0);
                    x->setBalance(0);  
                }
                else if(j->getBalance() == 1)
                {
                    i->setBalance(-1);
                    x->setBalance(0);
                }
                j->setBalance(0);
            }
        }
    }
    else if(i == x->getRight())
    {
        x->setBalance(x->getBalance() + 1);

        if(x->getBalance() == 0) return;

        else if(x->getBalance() == 1) helpInsert(x, i);
        else if(x->getBalance() == 2)
        {
            if(i->getRight() == j)
            {
                moveLeft(x);
                i->setBalance(0);
                x->setBalance(0);
            }
            else if(i->getRight() != j)
            {
                moveRight(i);
                moveLeft(x);

                if(j->getBalance() == -1)
                {
                    i->setBalance(1);
                    x->setBalance(0);
                }
                else if(j->getBalance() == 0)
                {
                    i->setBalance(0);
                    x->setBalance(0);  
                }
                else if(j->getBalance() == 1)
                {
                    i->setBalance(0);
                    x->setBalance(-1);
                }
                j->setBalance(0);
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    AVLNode<Key, Value>* newp;

    if(temp == NULL) return;

    if(temp->getLeft() != NULL && temp->getRight() != NULL)
    {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));
        nodeSwap(temp, pred);
    }
    AVLNode<Key, Value>* parent = temp->getParent(); //get what parent????????


    if(temp->getLeft() != NULL) newp = temp->getLeft();
    else newp = temp->getRight();

    int diff;

    if(newp != NULL) newp->setParent(parent);
    if(parent == NULL) this->root_ = newp;
    else if(temp != parent->getLeft())
    {
        parent->setRight(newp);
        diff = -1;
    }
    else
    {
        parent->setLeft(newp);
        diff = 1;
    }

    delete temp;
    helpRemove(parent, diff);

}

//REMOVE HELPER
template<class Key, class Value>
void AVLTree<Key, Value>:: helpRemove(AVLNode<Key, Value>* num, int diff)
{
    // TODO
    if(num == NULL) return;

    AVLNode<Key, Value>* x = num->getParent();
    int d = -1;

    if(x != NULL)
    {
        if(num->getKey() < x->getKey()) d = 1;
    }
 
    if(num->getBalance() + diff == -2)
    {
        AVLNode<Key, Value>* newp = num->getLeft();

        if(newp->getBalance() == 0)
        {
            moveRight(num);
            num->setBalance(-1);
            newp->setBalance(1);
            //return;  
        }
        else if(newp->getBalance() == -1)
        {
            moveRight(num);
            num->setBalance(0);
            newp->setBalance(0);
            helpRemove(x,d);
        }
        else if(newp->getBalance() == 1)
        {
            AVLNode<Key, Value>* temp = newp->getRight();
            moveLeft(newp);
            moveRight(num);

            if(temp->getBalance() == 0)
            {
                num->setBalance(0);
                newp->setBalance(0);
                temp->setBalance(0);
            }
            else if(temp->getBalance() == 1)
            {
                num->setBalance(0);
                newp->setBalance(-1);
                temp->setBalance(0);
            }
            else if(temp->getBalance() == -1)
            {
                num->setBalance(1);
                newp->setBalance(0);
                temp->setBalance(0);
            }
						helpRemove(x,d);
        }
                
    }

    else if(num->getBalance() + diff == 2)
    {
        AVLNode<Key, Value>* newp = num->getRight();

        if(newp->getBalance() == 0)
        {
            moveLeft(num);
            num->setBalance(1);
            newp->setBalance(-1);
           // return;  
        }
        else if(newp->getBalance() == 1)
        {
            moveLeft(num);
            num->setBalance(0);
            newp->setBalance(0);
            helpRemove(x,d);
        }
        else if(newp->getBalance() == -1)
        {
            AVLNode<Key, Value>* temp = newp->getLeft();
            moveRight(newp);
            moveLeft(num);

            if(temp->getBalance() == 0)
            {
                num->setBalance(0);
                newp->setBalance(0);
                temp->setBalance(0);
            }
            else if(temp->getBalance() == 1)
            {
                num->setBalance(-1);
                newp->setBalance(0);
                temp->setBalance(0);
            }
            else if(temp->getBalance() == -1)
            {
                num->setBalance(0);
                newp->setBalance(1);
                temp->setBalance(0);
            }
        }
    }

    else if(num->getBalance() + diff == -1)
    {
        num->setBalance(-1);
    }

    else if(num->getBalance() + diff == 0)
    {
        num->setBalance(0);
        helpRemove(x,d);
    }
}


//MOVE LEFT----------------------------------------
template<class Key, class Value>
void AVLTree<Key, Value>:: moveLeft(AVLNode<Key, Value>* num)
{
    // TODO
    AVLNode<Key, Value>* parent = num->getParent();
    AVLNode<Key, Value>* child1 = num->getRight();
    AVLNode<Key, Value>* child2 = child1->getLeft();

    child1->setParent(parent);
		
		if(num->getParent() == NULL)
    {
        this->root_ = num->getRight();
        child1->setLeft(num);
        num->setParent(child1);
        num->setRight(child2);
    }
    else if(num == parent->getLeft())
    {
        parent->setLeft(child1);
        child1->setLeft(num);
        num->setParent(child1);
        num->setRight(child2);
    }
    else if(num == parent->getRight())
    {
        parent->setRight(child1);
        child1->setLeft(num);
        num->setParent(child1);
        num->setRight(child2); 
    }



    if(child2 != NULL) child2->setParent(num);


}

//MOVE RIGHT----------------------------------------
template<class Key, class Value>
void AVLTree<Key, Value>:: moveRight(AVLNode<Key, Value>* num)
{
    // TODO
    AVLNode<Key, Value>* parent = num->getParent();
    AVLNode<Key, Value>* child1 = num->getLeft();
		AVLNode<Key, Value>* child2 = child1->getRight();

    child1->setParent(parent);

	  if(num->getParent() == NULL)
    {
        this->root_ = child1;
        child1->setRight(num);
        num->setParent(child1);
        num->setLeft(child2);
    }
    else if(num == parent->getLeft())
    {
        parent->setLeft(child1);
        child1->setRight(num);
        num->setParent(child1);
        num->setLeft(child2);
    }
    else if(num == parent->getRight())
    {
        parent->setRight(child1);
        child1->setRight(num);
        num->setParent(child1);
        num->setLeft(child2); 
    }

    if(child2 != NULL) child2->setParent(num);


}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
