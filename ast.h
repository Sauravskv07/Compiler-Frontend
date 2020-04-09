/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/

typedef struct attributes{
	VarType baseType;
	VarType eleType;
	Token *module_name;
	bool typeCheckReq;
} attributes;
typedef struct astnode
{
	node *data;
	attributes* attr;
	struct astnode *child;
	struct astnode *right;
	struct astnode *parent;
	Type tag;
} astnode;


astnode* createAST(treenode* root);
astnode *insertChildInAst(astnode *parent, astnode *child);
astnode *insertNextRightSiblingInAst(astnode *child, astnode *right);
astnode* getRootInAst(astnode *child);
astnode* makenode1(ht_item *t, astnode *n1);
astnode* makenode2(ht_item *t, astnode *n1, astnode *n2);
astnode* makenode3(ht_item *t, astnode *n1, astnode *n2, astnode *n3);
astnode* makenode4(ht_item *t, astnode *n1, astnode *n2, astnode *n3, astnode *n4);
astnode* makeleaf(Token  *tk);
void printTraversalAst(astnode *root,FILE* fp);
