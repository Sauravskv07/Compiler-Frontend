/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/
# define HT1 2
# define HT2 3

#include <stdbool.h>

typedef enum{TERM,FUNC,VAR} Category;

typedef struct{
	int eleType;
	int baseType;
	int low;
	int high;
}dataType;

typedef struct{
	struct ht_hash_table *input_list;
	struct ht_hash_table *output_list;
	int num_inputs;
	int num_outputs;
	dataType input_types[MAX_SIZE_MAPPING_TABLE];
	int output_types[MAX_SIZE_MAPPING_TABLE];
}parameters;

typedef struct {
    int index;
    Type tag;
} term_item;

typedef struct {
	int isDef;		// Add other function attributes also accordingly here.
	parameters *pr;  
} func_item;

typedef struct quad_row quad_row;
typedef struct {
	bool def;
	quad_row *label;  
} label_item;

typedef enum{INTEGER,REAL,BOOLEAN,ARRAY,UNDEFINED} VarType;

typedef struct {
	int offset;		// Add other variables attributes also accordingly here.
	int low;
	int high;
	VarType eleType;
	VarType baseType;
} var_item;

typedef union {
    var_item* v_item;
    func_item* f_item;
    term_item* t_item;
    label_item* l_item;
} ht_data;

typedef struct {
    char* key;
    ht_data* data;
    Category ct;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;


ht_item *tokensList[MAX_SIZE_MAPPING_TABLE];

int num_tokens;
int num_nonterminals;

ht_hash_table* mapping_table;
ht_hash_table* keyword_table;

ht_item* ht_insert_term_item(ht_hash_table* ht, const char* key, int index, Type tag);
ht_item* ht_insert_func_item(ht_hash_table* ht, const char* key, int isDef);
ht_item* ht_insert_label_item(ht_hash_table* ht, const char* key, bool def);
ht_item* ht_insert_var_item(ht_hash_table* ht, const char* key, int offset, VarType baseType, VarType eleType,int low,int high);
ht_item* ht_insert(ht_hash_table* ht, ht_item *item);
ht_item* ht_search(ht_hash_table* ht, const char* key);
ht_hash_table* ht_new();

