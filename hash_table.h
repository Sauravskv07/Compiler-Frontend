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


typedef struct {
    char* key;
    int index;
    Type tag;
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

ht_item* ht_insert(ht_hash_table* ht, const char* key, int index, Type tag);
ht_item* ht_search(ht_hash_table* ht, const char* key);
ht_hash_table* ht_new();

