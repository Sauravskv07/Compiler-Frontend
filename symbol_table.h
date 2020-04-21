/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/

typedef struct symnode
{
	ht_hash_table* symbol_table;
	int current_offset;
	char *module_name;
	char num_output_defined;
	bool isModuleScope;
	bool isForScope;
	char for_id[21];
	int switchStatus;
	int linstart;
	int linend;
	struct symnode *child;
	struct symnode *right;
	struct symnode *parent;
}symnode;

symnode* sym_root;
treenode *st;
treenode *en;
symnode* create_new_symnode();
symnode* insert_as_symchild(symnode* parent,symnode* child);
void printsymnode(symnode* root);
void printhashtable(char *module_name, int linstart, int linend, ht_hash_table* ht);
void printhashparams();
int getFunclinSt(ht_item* ht);
int getFunclinEn(ht_item* ht);
int str[20];
int nextSt(int i);
int nextEn();
void printsymarr(symnode* root);
void printsymnodearr(symnode* rt);
void printhashtablearr(char *module_name, int linstart, int linend, ht_hash_table* ht);
void printhashparamsarr();
