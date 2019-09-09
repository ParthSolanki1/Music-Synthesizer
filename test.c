#include<stdio.h>
#include<stdlib.h>
#include"BSTs.c"

int main()
{
    BST_Node *temp = NULL;
    BST_Node *root = NULL;

    //testing the creation and insertion of nodes
    printf("------------BST Insert and Node creation------------");
    temp = newBST_Node(82.41, 3, 0.0);
    printf("Expected: %d, actal: %d\n", 3, temp->bar);
    printf("Expected: %lf, actal: %lf\n", 82.41, temp->freq);
    printf("Expected: %lf, actal: %lf\n", 0.0, temp->index);
    root = BST_insert(root, temp);
    temp = newBST_Node(43.65, 0, 0.9);
    root = BST_insert(root, temp);   
    temp = newBST_Node(16.35, 6, 0.3);
    root = BST_insert(root, temp);
    temp = newBST_Node(23.12, 1, 0.8);
    root = BST_insert(root, temp);
    printf("Expected %lf, actual %lf\n", 23.12, root->left->right->freq);
    temp = newBST_Node(36.71, 0, 0);
    root = BST_insert(root, temp); 
    temp = newBST_Node(41.2, 4, 0.2);
    root = BST_insert(root, temp);
    temp = newBST_Node(164.81, 6, 0.7);
    root = BST_insert(root, temp);  
    temp = newBST_Node(233.08, 0, 0.95);
    root = BST_insert(root, temp);
    temp = newBST_Node(146.83, 3, 0.6);
    root = BST_insert(root, temp);
    temp = newBST_Node(207.65, 5, 0.5);
    root = BST_insert(root, temp);
    temp = newBST_Node(36.71, 6, 0.5);
    root = BST_insert(root, temp);
    printf("Expected %lf, actual %lf\n", 36.71, root->right->right->left->freq);
    temp = newBST_Node(36.71, 7, 0);
    root = BST_insert(root, temp);

    //testing search
    printf("------------BST Search------------");
    temp = BST_search(root, 3, 0);
    printf("Expected: %lf Actual: %lf\n", 82.41, temp->freq);
    temp = BST_search(root, 4, 0.2);
    printf("Expected: %lf Actual: %lf\n", 41.2, temp->freq);
    temp = BST_search(root, 0, 0.95);
    printf("Expected: %lf Actual: %lf\n", 233.08, temp->freq);

    //deleting nodes (all 3 cases)
    printf("------------Deleting BST Nodes------------");
    root = BST_delete(root, 0, 0);
    temp = BST_search(root, 0, 0);
    printf("Expected: (nill) Actual: %p\n", temp);
    temp = BST_search(root, 0, 0.9);
    printf("Expected: (nill) Actual: %p\n", temp->left);
    root = BST_delete(root, 1, 0.8);
    temp = BST_search(root, 1, 0.8);
    printf("Expected: (nill) Actual: %p\n", temp);
    temp = BST_search(root, 0, 0.9);
    printf("Expected: 0.95 Actual: %lf\n", temp->right->index);
    root = BST_delete(root, 3, 0);
    temp = BST_search(root, 3, 0);
    printf("Expected: (nill) Actual: %p\n", temp);
    printf("Expected: 0.6 Actual: %lf\n", root->index);
    
    //in order printing
    printf("------------In-order Traversal Expected------------\n");
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 1, 0, 0.9, 43.65);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 0, 0.95, 223.08);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 0, 3, 0.6, 146.83);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 4, 0.2, 41.2);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 5, 0.5, 207.65);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 1, 6, 0.3, 16.35);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 6, 0.5, 36.71);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 6, 0.7, 164.81);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 7, 0.0, 36.71);
    printf("------------In-order Traversal actual------------\n");
    BST_inOrder(root, 0);

    //preorder printing
    printf("------------Pre-order Traversal Expected------------\n");
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 0, 3, 0.6, 146.83);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 1, 0, 0.9, 43.65);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 0, 0.95, 223.08);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 1, 6, 0.3, 16.35);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 4, 0.2, 41.2);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 5, 0.5, 207.65);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 6, 0.7, 164.81);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 6, 0.5, 36.71);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 7, 0.0, 36.71);
    printf("------------Pre-order Traversal actual------------\n");
    BST_preOrder(root, 0);

    //postorder printing
    printf("------------Post-order Traversal Expected------------\n");
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 0, 0.95, 223.08);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 1, 0, 0.9, 43.65);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 5, 0.5, 207.65);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 4, 0.2, 41.2);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 6, 0.5, 36.71);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 3, 7, 0.0, 36.71);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 2, 6, 0.7, 164.81);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 1, 6, 0.3, 16.35);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", 0, 3, 0.6, 146.83);
    printf("------------Pre-order Traversal actual------------\n");
    BST_postOrder(root, 0);

    //testing delete BST
    printf("------------Delete BST------------\n");
    delete_BST(root);
    printf("Expected (nill) Got: %p\n", root);
}