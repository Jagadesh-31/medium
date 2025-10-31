#include "bst.h"
#include <stdlib.h>

// Creates a new BST node with given value
TreeNode* create_bst_node(int val) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Inserts value into BST (maintains BST property)
TreeNode* insert_bst(TreeNode* root, int val) {
    // If tree is empty, create new node as root
    if (root == NULL) {
        return create_bst_node(val);
    }
    // If value is less than current node, insert in left subtree
    if (val < root->val) {
        root->left = insert_bst(root->left, val);
    }
    // If value is greater or equal, insert in right subtree
    else {
        root->right = insert_bst(root->right, val);
    }
    return root;
}

// Performs inorder traversal (left, root, right) - gives sorted order
void inorder_traversal(TreeNode* root, int* result, int* index) {
    if (root == NULL) return;
    // Traverse left subtree
    inorder_traversal(root->left, result, index);
    // Visit current node
    result[*index] = root->val;
    (*index)++;
    // Traverse right subtree
    inorder_traversal(root->right, result, index);
}

// Frees all memory used by BST
void free_bst(TreeNode* root) {
    if (root == NULL) return;
    // Free left and right subtrees first
    free_bst(root->left);
    free_bst(root->right);
    // Then free current node
    free(root);
}
