typedef struct symnode
{
	ht_hash_table* symbol_table;
	char module_name[21];
	struct symnode *child;
	struct symnode *right;
	struct symnode *parent;
}symnode;

symnode* sym_root;
symnode* create_new_symnode();
symnode* insert_as_symchild(symnode* parent,symnode* child);
