/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/

typedef union node
{
	Token *token;
	ht_item *nonterm;
}node;

typedef struct treenode
{
	node *data;
	struct treenode *child;
	struct treenode *right;
	struct treenode *parent;
	Type tag;
}treenode;

typedef struct stack
{
	ht_item *data;
	struct stack *next;
	struct stack *prev;
}stack;

stack* pop(stack *st);
stack* push(stack *st,ht_item *b);
ht_item* peek(stack *st);

treenode* root;

treenode* insertAsChild(treenode *parent, node *child, Type tag);
treenode* insertAsNextRightSibling(treenode *child, node *right, Type tag);
treenode* getRoot(treenode *child);
void printTraversal(treenode *root,FILE* fp);
