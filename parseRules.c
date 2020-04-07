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
#include <string.h>
#include <stdlib.h>

int parse(char *fileName)
{

	int i=-1;

	FILE* rules_file=fopen(fileName,"r");	
	char* line;

    	if (rules_file == NULL)
	{
		printf("UNABLE TO OPEN FILE RULES.TXT");
        	exit(EXIT_FAILURE);
	}

	while (fgets(line,200, rules_file)!=NULL) {        	
		line[strcspn(line, "\n")] = 0;
		//printf("%s\n", line);
		i = addLine(line,i);
    	}
	
	fclose(rules_file);

	printf("Rules parsing Successful\n");
	
	return 0;
}
int addLine(char line[], int index)
{	
	char *ld;
	ld = strtok (line," <>");

	rules[++index].lhs = ht_search(mapping_table, ld);
	rule_count++;

	if(rules[index].lhs==NULL)
	{
		return index;
	}

	rule_rhs* t=NULL;
	rule_rhs* term;
	ht_item* temp;
	strtok (NULL," <>");
	char *nd = strtok (NULL," <>");

	//printf("%s -> \n",rules[index].lhs->key);
	while(nd!=NULL)
	{
		//printf("%s\n",nd);
		if(strlen(nd)==1&&nd[0]=='|')
		{
			//printf("REPEATED NONTERMINAL\n");
			rules[++index].lhs = ht_search(mapping_table, ld);
			rule_count++;
			t=NULL;
		}
		else
		{
			temp = ht_search(mapping_table, nd);
			if(temp==NULL)
			{return index++;}
			term = malloc(sizeof(rule_rhs));
			term->node = temp;
			term->next = NULL;
			if(t==NULL)
			{
				rules[index].key = term;
				t=rules[index].key;
			}
			else
			{
				t->next = term;
				t=term;			
			}
		}
		nd = strtok (NULL," <>");
	}
	
	return index;
}
void printRules()
{
	printf("LETS PRINT \n");
	for(int i=0;i<rule_count;i++)
	{
		printf("%s -> ",rules[i].lhs->key);
		rule_rhs *temp=rules[i].key;		
		while(temp!=NULL)
		{
			printf("%s ",temp->node->key);
			temp=temp->next;
		}
		printf("\n");
	}
}

void printRule(int i)
{
	printf("%s -> ",rules[i].lhs->key);
	rule_rhs *temp=rules[i].key;	
	while(temp!=NULL)
	{
		printf("%s ",temp->node->key);
		temp=temp->next;
	}
	printf("\n");
}
	
