#include <stdio.h>
#include <stdlib.h>

//Red-Black Tree Node structure
typedef struct tf* tp;
struct tf {
    int key;        // Data
    int col;        // 1 - RED, 0 - BLACK
    tp left, right, p; // Left-Child, Right-Child, Parent
};

tp TNULL;		// Sentinel node for leaf nodes
tp root = NULL; // Global root pointer

// Initializition of the the sentinel node (TNULL)
void initializeTNULL() {
    TNULL = (tp)malloc(sizeof(struct tf));
    TNULL->key = -1;       // Sentinel key value (can be any value that signifies null)
    TNULL->col = 0;        // Sentinel node is always black
    TNULL->left = NULL;
    TNULL->right = NULL;
    TNULL->p = NULL;
}

// Create a new node with the given key
tp createNode(int key) {
    tp newNode = (tp)malloc(sizeof(struct tf));
    newNode->key = key;
    newNode->col = 1; // New nodes are always red initially
    newNode->left = TNULL;
    newNode->right = TNULL;
    newNode->p = NULL;
    return newNode;
}

void leftRotate(tp *root, tp x) {
    tp y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->p = x;
    y->p = x->p;
    if (x->p == NULL)
        *root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}

void rightRotate(tp *root, tp x) {
    tp y = x->left;
    x->left = y->right;
    if (y->right != NULL)
        y->right->p = x;
    y->p = x->p;
    if (x->p == NULL)
        *root = y;
    else if (x == x->p->right)
        x->p->right = y;
    else
        x->p->left = y;
    y->right = x;
    x->p = y;
}

void fixup(tp *root, tp pt) {
    tp parent_pt = NULL;
    tp grand_parent_pt = NULL;

    while ((pt != *root) && (pt->col == 1) && (pt->p->col == 1)) {
        parent_pt = pt->p;
        grand_parent_pt = pt->p->p;

        if (parent_pt == grand_parent_pt->left) {	// If parent is a left child
            tp uncle_pt = grand_parent_pt->right;

            if (uncle_pt != NULL && uncle_pt->col == 1) { // Case 1: Uncle is RED
                grand_parent_pt->col = 1;
                parent_pt->col = 0;
                uncle_pt->col = 0;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->right) { // Case 2: pt is a right child of its parent
                    leftRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }

                rightRotate(root, grand_parent_pt); // Case 3: pt is a left child of its parent
                int temp = parent_pt->col;
                parent_pt->col = grand_parent_pt->col;
                grand_parent_pt->col = temp;
                pt = parent_pt;
            }
        } else {
            tp uncle_pt = grand_parent_pt->left;

            if ((uncle_pt != NULL) && (uncle_pt->col == 1)) { // Case 1: Uncle is RED
                grand_parent_pt->col = 1;
                parent_pt->col = 0;
                uncle_pt->col = 0;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->left) {  // Case 2: pt is a left child of its parent
                    rightRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }

                leftRotate(root, grand_parent_pt); // Case 3: pt is a right child of its parent
                int temp = parent_pt->col;
                parent_pt->col = grand_parent_pt->col;
                grand_parent_pt->col = temp;
                pt = parent_pt;
            }
        }
    }

    (*root)->col = 0; // Ensure the root is always black
}

void insert(tp *root) {
    int key;
    printf("\nEnter the key to insert: ");
    scanf("%d", &key);

    tp newNode = createNode(key);
    tp y = NULL;
    tp x = *root;

 	// Traverse the tree to find the correct insertion point
    while (x != TNULL) {
        y = x;
        if (newNode->key < x->key)
            x = x->left;
        else if (newNode->key > x->key)
            x = x->right;
        else {
            printf("\nDuplicate keys are not allowed.\n");
            free(newNode);
            return;
        }
    }

    newNode->p = y;
    if (y == NULL)
        *root = newNode;
    else if (newNode->key < y->key)
        y->left = newNode;
    else
        y->right = newNode;

    fixup(root, newNode);
}

void print(tp root) {
    if (root != TNULL) {
        print(root->left);

        printf("Key: %d\n", root->key);

        // Color
        if (root->col == 0) {
            printf("   Color: BLACK\n");
        }
        if (root->col == 1) {
            printf("   Color: RED\n");
        }

        // Left Child
        if (root->left != TNULL) {
            printf("   Left Child: %d\n", root->left->key);
        }
        if (root->left == TNULL) {
            printf("   Left Child: TNULL\n");
        }

        // Right Child
        if (root->right != TNULL) {
            printf("   Right Child: %d\n", root->right->key);
        }
        if (root->right == TNULL) {
            printf("   Right Child: TNULL\n");
        }

        // Parent
        if (root->p != NULL && root->p != TNULL) {
            printf("   Parent: %d\n", root->p->key);
        }
        if (root->p == NULL || root->p == TNULL) {
            printf("   Parent: TNULL (Root Node)\n");
        }

        print(root->right);
    }
}


void RBTransplant(tp *root, tp u, tp v) {
    if (u->p == NULL) {
        *root = v;  // Case 1: u is the root
    } else if (u == u->p->left) {
        u->p->left = v;  // Case 2: u is the left child
    } else {
        u->p->right = v; // Case 3: u is the right child
    }
    v->p = u->p;
}

tp TreeMinimum(tp x) {
    if (x == NULL || x == TNULL) return NULL;
    while (x->left != TNULL) {
        x = x->left;
    }
    return x;
}

void RBDeleteFixup(tp *root, tp x) {
    while (x != *root && x->col == 0) {
        tp w;
        if (x == x->p->left) {
            w = x->p->right;
            if (w->col == 1) {
                w->col = 0;
                x->p->col = 1;
                leftRotate(root, x->p);
                w = x->p->right;
            }
            if (w->left->col == 0 && w->right->col == 0) {
                w->col = 1;
                x = x->p;
            } else {
                if (w->right->col == 0) {
                    w->left->col = 0;
                    w->col = 1;
                    rightRotate(root, w);
                    w = x->p->right;
                }
                w->col = x->p->col;
                x->p->col = 0;
                w->right->col = 0;
                leftRotate(root, x->p);
                x = *root;
            }
        } else {
            w = x->p->left;
            if (w->col == 1) {
                w->col = 0;
                x->p->col = 1;
                rightRotate(root, x->p);
                w = x->p->left;
            }
            if (w->right->col == 0 && w->left->col == 0) {
                w->col = 1;
                x = x->p;
            } else {
                if (w->left->col == 0) {
                    w->right->col = 0;
                    w->col = 1;
                    leftRotate(root, w);
                    w = x->p->left;
                }
                w->col = x->p->col;
                x->p->col = 0;
                w->left->col = 0;
                rightRotate(root, x->p);
                x = *root;
            }
        }
    }
    x->col = 0;
}

void RBDelete(tp *root, tp z) {
    tp y = z;
    tp x = NULL;
    int yOriginalColor = y->col;

    if (z->left == TNULL) {
        x = z->right;
        RBTransplant(root, z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        RBTransplant(root, z, z->left);
    } else {
        y = TreeMinimum(z->right); // Successor
        yOriginalColor = y->col;
        x = y->right;
        if (y->p == z) {
            if (x != TNULL) {
                x->p = y;
            }
        } else {
            RBTransplant(root, y, y->right);
            y->right = z->right;
            if (y->right != TNULL) {
                y->right->p = y;
            }
        }
        RBTransplant(root, z, y);
        y->left = z->left;
        if (y->left != TNULL) {
            y->left->p = y;
        }
        y->col = z->col;
    }
    free(z);

    if (yOriginalColor == 0 && x != TNULL) {
        RBDeleteFixup(root, x);
    }
}

int main() {
    initializeTNULL();
    root = TNULL;

    char choice;
    printf("\nWelcome to the Red-Black Tree Program!\n");
    printf("\nOptions:\nI - Insert\nP - Print\nD - Delete\nQ - Quit\n");

    do {
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'I':
            case 'i':
                insert(&root);
                break;

            case 'P':
            case 'p':
                if (root == TNULL)
                    printf("\nThe tree is empty.\n");
                else
                    print(root);
                break;

            case 'D':
            case 'd': {
                if (root == TNULL) {
                    printf("\nThe tree is empty. Nothing to delete.\n");
                } else {
                    int delKey;
                    printf("\nEnter the key to delete: ");
                    scanf("%d", &delKey);

                    // Find the node to delete
                    tp current = root;
                    while (current != TNULL && current->key != delKey) {
                        if (delKey < current->key)
                            current = current->left;
                        else
                            current = current->right;
                    }

                    if (current == TNULL) {
                        printf("\nKey %d not found in the tree.\n", delKey);
                    } else {
                        RBDelete(&root, current);
                        printf("\nKey %d deleted successfully.\n", delKey);
                    }
                }
                break;
            }

            case 'Q':
            case 'q':
                printf("\nExiting program. Goodbye!\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 'Q' && choice != 'q');

    return 0;
}
