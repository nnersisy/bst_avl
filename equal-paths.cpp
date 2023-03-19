#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below
    bool r = false, l = false;

    if(root == NULL || ((root->left) == NULL && (root->right) == NULL)) return true;
    else if(root->left != NULL && root->right != NULL)
    {
        if(root->left->left == NULL && root->left->right == NULL) l = true;
        if(root->right->right == NULL && root->right->left == NULL) r = true;
        if(l != r) return false;
    }
    return equalPaths(root->left) && equalPaths(root->right);
}

