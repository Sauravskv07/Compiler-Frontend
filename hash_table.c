/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include <math.h>
#include "hash_table.h"

ht_item* ht_insert_term_item(ht_hash_table* ht, const char* key, int index, Type tag) {

    term_item* i = (term_item *)malloc(sizeof(term_item));
    i->index=index;
    i->tag=tag;
    ht_item* j = (ht_item *)malloc(sizeof(ht_item));
    j->key = strdup(key);
    ht_data* k = (ht_data *)malloc(sizeof(ht_data));
    k->t_item = i;
    j->data = k;
    j->ct = 0;
    //printf("%s",key);
    return ht_insert(ht,j);
}

ht_item* ht_insert_func_item(ht_hash_table* ht, const char* key, int isDef) {

    func_item* i = (func_item *)malloc(sizeof(func_item));
    i->isDef=isDef;
    ht_item* j = (ht_item *)malloc(sizeof(ht_item));
    j->key = strdup(key);
    ht_data* k = (ht_data *)malloc(sizeof(ht_data));
    k->f_item = i;
    j->data = k;
    j->ct = 1;
    return ht_insert(ht,j);
}
ht_item* ht_insert_label_item(ht_hash_table* ht, const char* key, bool def) {

    label_item* i = (label_item *)malloc(sizeof(label_item));
    i->def=def;
    ht_item* j = (ht_item *)malloc(sizeof(ht_item));
    j->key = strdup(key);
    ht_data* k = (ht_data *)malloc(sizeof(ht_data));
    k->f_item = i;
    j->data = k;
    j->ct = 1;
    return ht_insert(ht,j);
}

ht_item* ht_insert_var_item(ht_hash_table* ht, const char* key, int offset, VarType baseType, VarType eleType,int low,int high) {

	var_item* i = (var_item *)malloc(sizeof(var_item));
	i->offset=offset;
	i->baseType=baseType;
	i->eleType=eleType;
	i->low=low;
	i->high=high;
	i->highNode=NULL;
	i->lowNode=NULL;
	ht_item* j = (ht_item *)malloc(sizeof(ht_item));
	j->key = strdup(key);
	ht_data* k = (ht_data *)malloc(sizeof(ht_data));
	k->v_item = i;
	j->data = k;
	j->ct = 2;
	return ht_insert(ht,j);
}


static int ht_hashing(const char* s, const int a, const int m) {

    	long int hash = 0;

    	const int len_s = strlen(s);

    	for (int i = 0; i < len_s; i++) {
        	hash += (long)pow(a, len_s - (i+1)) * s[i];
       	 	hash = hash % m;
    	}

    	return (int)hash;
}

static int ht_get_hash(const char* s, const int num_buckets, const int attempt) 
{
	const int hash_1 = ht_hashing(s, HT1, num_buckets);

	const int hash_2 = ht_hashing(s, HT2, num_buckets);

    	return (hash_1 + (attempt * (hash_2 + 1))) % num_buckets;
}

ht_item* ht_insert(ht_hash_table* ht, ht_item *item) {

    	int index = ht_get_hash(item->key, ht->size, 0);

    	ht_item* current_item = ht->items[index];

    	int i = 1;

    	while (current_item != NULL) {

        	index = ht_get_hash(item->key, ht->size, i);

        	current_item = ht->items[index];

     		i++;

    		} 

    	ht->items[index] = item;

    	ht->count++;

    	return item;
}


ht_item* ht_search(ht_hash_table* ht, const char* key) {

	int index = ht_get_hash(key, ht->size, 0);

	ht_item* item = ht->items[index];

	int i = 1;

	while (item != NULL) {

        	if (strcmp(item->key,key) == 0) {

            	return item;
        	}

        	index = ht_get_hash(key, ht->size, i);

        	item = ht->items[index];

        	i++;
    }
    return NULL;
}



ht_hash_table* ht_new() {

	ht_hash_table* ht = malloc(sizeof(ht_hash_table));

	ht->size = MAX_SIZE_MAPPING_TABLE;

	ht->count = 0;
	
	ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

	return ht;
}

