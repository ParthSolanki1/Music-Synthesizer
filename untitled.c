
#include "BSTs.c"

int main()
{
    BST_Node *root=NULL;
    BST_Node *new_note=NULL;
    BST_Node *t=NULL;
    
    new_note = newBST_Node(330.0, 1, 1.0);
    root = BST_insert(root, new_note);
    new_note = newBST_Node(320.0, 1, 2.0);
    root = BST_insert(root, new_note);
    new_note = newBST_Node(330.0, 1, 3.0);
    root = BST_insert(root, new_note);
    new_note = newBST_Node(330.0, 1, 1.2);
    root = BST_insert(root, new_note);
    new_note = newBST_Node(330.0, 1, 0);
    root = BST_insert(root, new_note);
    new_note = newBST_Node(330.0, 0, 1.0);
    root = BST_insert(root, new_note);

    t = BST_search(root, 1, 1.2);

    if(t==NULL){
        printf("I HATE CS\n");
    }

    printf("%lf\n", root->right->left->key);

    return 0;
}