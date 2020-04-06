/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/
#include "lexer.h"
#include "hash_table.h"
#include "parseRules.h"
#include "utils.h"
#include "parseTable.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

void create_parse_table()
{
	for(int i=0;i<MAX_SIZE_MAPPING_TABLE;i++)
		for(int j=0;j<MAX_SIZE_MAPPING_TABLE;j++)
			parse_table[i][j]=-1;

	int index_e = ht_search(mapping_table,"e")->data->t_item->index;
	//printf("%s",ht_search(mapping_table,"e")->key);

	for(int i=0;i<rule_count;i++)
	{
		ht_item* nonterminal=rules[i].lhs;
		
		ht_items_list * first=computeFirstofRule(rules[i].key)->next;
		
		int contains_e=0;

		int contains_dollar=0;

		while(first!=NULL)
		{
			if(first->node->data->t_item->index==index_e)
			{
				contains_e=1;
			}

			parse_table[nonterminal->data->t_item->index][first->node->data->t_item->index]=i;
			
			first=first->next;

		}

		if(contains_e==1)
		{
			ht_items_list * follow=computeFollow(nonterminal)->next;

			while(follow!=NULL)
			{
				parse_table[nonterminal->data->t_item->index][follow->node->data->t_item->index]=i;
	
				follow=follow->next;
			}		
		}
	}
}

void print_parse_table()
{	
	printf("    ");
	for(int i=0;i<num_terminals;i++)
		printf("%s ",tokensList[i]->key);

	for(int i=0;i<num_nonterminals;i++)
	{
		printf("\n%s ",tokensList[i+num_terminals]->key);
		
		for(int j=0;j<num_terminals;j++)
		{
			printf("%d  ",parse_table[i+num_terminals][j]);
		}
	}
}
