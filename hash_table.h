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

typedef struct {
    int index;
    Type tag;
} term_item;

typedef struct {
    int isDef;		// Add other function attributes also accordingly here.
} func_item;

typedef enum{INTEGER,REAL,BOOLEAN,ARRAY} VarType;

typedef struct {
    int offset;		// Add other variables attributes also accordingly here.
    VarType eleType;
    VarType baseType;
} var_item;

typedef union {
    var_item* v_item;
    func_item* f_item;
    term_item* t_item;
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
ht_item* ht_insert_var_item(ht_hash_table* ht, const char* key, int offset, VarType baseType, VarType eleType);
ht_item* ht_insert(ht_hash_table* ht, ht_item *item);
ht_item* ht_search(ht_hash_table* ht, const char* key);
ht_hash_table* ht_new();

