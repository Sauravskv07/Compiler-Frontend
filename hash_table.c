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

static ht_item* ht_new_item(const char* k, int index, Type tag) {

    ht_item* i = malloc(sizeof(ht_item));

    i->key = strdup(k);

    i->index=index;

    i->tag=tag;

    return i;
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

ht_item* ht_insert(ht_hash_table* ht, const char* key, int ind, Type tag) {

    	ht_item* item = ht_new_item(key, ind, tag);

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

