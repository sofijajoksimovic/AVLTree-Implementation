#include <iostream>
#include<stack>
using namespace std;
struct Node{
    int key;
    int height;
    Node*right;
    Node*left;
    Node*parent;
    Node(int key, Node*parent) {
        this->key = key;
        this->height = 1;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = parent;
    }
};
//returns height of the node
int height(Node*t){
    return t?t->height:0;
}
//balance of the node is height difference between left and right subtree
int balance(Node*t){
    return t?height(t->left) - height(t->right):0;
}

Node* rightRot(Node*x,Node*root,Node*fp) {
    Node *y = x->left;
    Node *temp = y->right;
    y->right = x;
    x->left = temp;
    y->parent=fp;
    if(fp!= nullptr){
        if(fp->right==x)fp->right=y;
        else fp->left=y;
    }
    x->height = max(height(x->left),height(x->right)) + 1;
    y->height = max(height(y->left),height(y->right)) + 1;
    if(root==x)root=y;
    return root;
}

Node* leftRot(Node*x,Node*root,Node*fp) {
    Node *y = x->right;
    Node *temp = y->left;
    y->left = x;
    x->right = temp;
    y->parent = fp;
    //parent should be updated
    if (fp != nullptr) {
        if (fp->right == x)fp->right = y;
        else fp->left = y;
    }
    //nodes changed their position so their height should be updated
    x->height = max(height(x->left),height(x->right)) + 1;
    y->height = max(height(y->left),height(y->right)) + 1;
    //case when root is x
    if (root == x)root = y;
    return root;
}
//iterative insert using parent pointer
//using rotations to balance the tree
Node* insert(Node* root,int k)
{
    if(root== nullptr){
        root=new Node(k,nullptr);
        return root;
    }
    Node*p=root,*q,*fp;

    while(p!= nullptr)
    {
        if(k==p->key)
            return root;
        if (k < p->key) {
            q = p->left;
        }
        else q = p->right;
        fp = p;
        p = q;
    }
    q=new Node(k,fp);
    if(k<fp->key)fp->left=q;
    else fp->right=q;

    Node*curr=q;
    Node *fparent=q->parent;
    while (fparent!=nullptr) {
        curr->height=max(height(curr->left),height(curr->right))+1;
        int b= balance(curr);
        if(b==2 and balance(curr->left)==1){
            root=rightRot(curr,root,fparent);
        }
        if(b==2 and balance(curr->left)==-1)
        {
            root=leftRot(curr->left,root,curr);
            root=rightRot(curr,root,fparent);
        }
        if(b==-2 and balance(curr->right)==-1)
        {
            root= leftRot(curr,root,fparent);
        }
        if(b==-2 and balance(curr->right)==1)
        {
            root=rightRot(curr->right,root,curr);
            root=leftRot(curr,root,fparent);
        }
        curr=fparent;
        fparent=fparent->parent;
    }
    return root;
}
Node* deleteKey(Node*root, int key){
    if(!root)return nullptr;

    Node*parent=nullptr;
    Node*curr=root;
    Node*last=nullptr;

    //searching for the node with key
    while(curr && curr->key!=key) {
        parent = curr;
        if (key < curr->key) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    //node not found
    if (!curr)return root;

    //node has 0 or 1 children
    if (!curr->left || !curr->right) {
        Node* t=curr->left?curr->left:curr->right;

        if (!parent)root=t;
        else if (parent->left==curr)parent->left=t;
        else parent->right=t;
        if (t)t->parent=parent;
        delete curr;
        last=parent;
    }

    //node has 2 children
    else {
        //replace the node with its successor (leftmost node in the right subtree)
        Node*succ=curr->right;
        Node*sParent=curr;
        while (succ->left) {
            sParent=succ;
            succ=succ->left;
        }
        curr->key=succ->key;
        //succ can only have right subtree
        if (sParent->left==succ) {
            sParent->left=succ->right;
        }
        else sParent->right=succ->right;
        if (succ->right)succ->right->parent = sParent;
        delete succ;
        last=sParent;
    }
    //balance the tree from the last node on the path to the root
    Node*f=last;
    Node *fparent=last->parent;
    while (fparent!=nullptr) {
        f->height=max(height(f->left),height(f->right))+1;
        int b= balance(f);
        if(b==2 and balance(f->left)==1){
            root=rightRot(f,root,fparent);
        }
        if(b==2 and balance(f->left)==-1)
        {
            root=leftRot(f->left,root,f);
            root=rightRot(f,root,fparent);
        }
        if(b==-2 and balance(f->right)==-1)
        {
            root= leftRot(f,root,fparent);
        }
        if(b==-2 and balance(f->right)==1)
        {
            root=rightRot(f->right,root,f);
            root=leftRot(f,root,fparent);
        }
        f=fparent;
        fparent=fparent->parent;
    }
    return root;
}
//recursive inorder traversal
void inorderTraversal(Node* root) {
    if (root == nullptr)
        return;

    inorderTraversal(root->left);
    cout << root->key << " ";
    inorderTraversal(root->right);
}
int main() {
    Node *root=nullptr;
    root=insert(root,2);
    root=insert(root,12);
    root=insert(root,5);
    root=insert(root,6);
    root=insert(root,1);
    root=insert(root,3);
    root=insert(root,17);
    inorderTraversal(root);
    cout<<endl;
    root=deleteKey(root,6);
    inorderTraversal(root);
}