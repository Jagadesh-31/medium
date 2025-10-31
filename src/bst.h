#ifndef BST_H
#define BST_H

// Binary Search Tree node structure
typedef struct TreeNode {
    int val;                 // Value stored in node
    struct TreeNode* left;   // Pointer to left child
    struct TreeNode* right;  // Pointer to right child
} TreeNode;

// Function declarations
TreeNode* create_bst_node(int val);
TreeNode* insert_bst(TreeNode* root, int val);
void inorder_traversal(TreeNode* root, int* result, int* index);
void free_bst(TreeNode* root);

#endif