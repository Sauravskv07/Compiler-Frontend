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
	struct symnode *child;
	struct symnode *right;
	struct symnode *parent;
}symnode;

symnode* sym_root;
symnode* create_new_symnode();
symnode* insert_as_symchild(symnode* parent,symnode* child);
