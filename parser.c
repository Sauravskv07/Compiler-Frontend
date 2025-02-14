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
#include "adt.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>

Token* nextToken;

void validateLexError()
{
		while(nextToken!=NULL && nextToken->index==-1)
		{
				num_errors++;
				printf("INVALID TOKEN. LEXICAL ERROR.\n");
				printf("LEXEME = %s LINE NUMBER = %d\n",nextToken->lexeme,nextToken->LN);
				error_list* new_error=(error_list*)malloc(sizeof(error_list));
				new_error->tk=nextToken;
				new_error->next=errors;
				errors=new_error;
				nextToken=getNextToken();
		}
}

error_list* parseTree(){

	num_errors=0;

	root=NULL;

	errors=NULL;

	stack *st=(stack*)malloc(sizeof(stack));

	ht_item* rhs_rev[MAX_RHS];

	ht_item* bottom=NULL;

	st->data=bottom;
	st->next=NULL;
	st->prev=NULL;

	ht_item* start=ht_search(mapping_table,"program");

	ht_item* end_marker=(ht_item *)malloc(sizeof(ht_item));
	ht_data* k = malloc(sizeof(ht_data));
	term_item* i = malloc(sizeof(term_item));
	k->t_item = i;
	end_marker->data = k;

	end_marker->data->t_item->index=-2;

	end_marker->key=NULL;

	st=push(st,start);

	ht_item *top;

	nextToken=getNextToken();
	validateLexError();

	node* temp;
	
	treenode* currentNode = NULL;
	
	int isFirst=1;

	while(peek(st)!=NULL)
	{
/*
		if(root!=NULL)
			if(root->tag==1)
				printf("Root = %s\n",root->data->token->lexeme);
			else
				printf("Root = %s\n",root->data->nonterm->key);
*/
		top=peek(st);

		//printf("Popped Element = %s\n",top->key);

		st=pop(st);

		if(top->data->t_item->index==ht_search(mapping_table,"e")->data->t_item->index)
		{
			temp=(node *)malloc(sizeof(node));
			temp->nonterm = top;
			currentNode=insertAsChild(currentNode, temp, 2);
			isFirst=0;
			continue;
		}
		
		if(top->data->t_item->index==-2)
		{
			currentNode=currentNode->parent;
			continue;
		}
		
		if(nextToken==NULL)
		{
			printf("REACHED END OF PROGRAM WITHOUT COMPLETE PARSE TREE GENERATION\n");

			error_list* new_error=(error_list*)malloc(sizeof(error_list));
			
			new_error->tk=nextToken;

			new_error->next=errors;
	
			errors=new_error;

			return errors;

		}
	
		if(top->data->t_item->tag==2)
		{
			temp=(node *)malloc(sizeof(node));
			temp->nonterm = top;

			int rule_index=parse_table[top->data->t_item->index][nextToken->index];
			
			//printf("rule index = %d and token name = %s\n",rule_index,nextToken->lexeme);	
			if(rule_index==-1)
			{			

				num_errors++;
				printf("UNEXPECTED TOKEN!!");
				printf("LINE NUMBER =  %d  LEXEME = %s  TOKEN_NAME =  %s\n",nextToken->LN,nextToken->lexeme,tokensList[nextToken->index]->key);
				error_list* new_error=(error_list*)malloc(sizeof(error_list));
				new_error->tk=nextToken;
				new_error->next=errors;
				errors=new_error;

				nextToken=getNextToken();
				validateLexError();

				while(nextToken!=NULL)
				{
					rule_index=parse_table[top->data->t_item->index][nextToken->index];
					if(rule_index!=-1)
					{
						break;
					}
					num_errors++;
					printf("UNEXPECTED TOKEN!!");
					printf("LINE NUMBER =  %d  LEXEME = %s  TOKEN_NAME =  %s\n",nextToken->LN,nextToken->lexeme,tokensList[nextToken->index]->key);
					nextToken=getNextToken();
					validateLexError();
				}
				
				if(rule_index==-1)
					continue;
			}

			if(rule_index==-3)
			{

					printf("UNEXPECTED TOKEN!!");
					printf("LINE NUMBER =  %d  LEXEME = %s  TOKEN_NAME =  %s\n",nextToken->LN,nextToken->lexeme,tokensList[nextToken->index]->key);
				continue;
			}

			if(isFirst)
			{
				currentNode=insertAsChild(currentNode, temp,top->data->t_item->tag );
				isFirst=0;
			}
			else
			{
				currentNode=insertAsNextRightSibling(currentNode, temp,top->data->t_item->tag);
			}

			st=push(st,end_marker);

			//printf("Rule used = %d \n",rule_index);
			rule_rhs* rule = rules[rule_index].key;
			int i=0;
			
			while(rule)
			{
				rhs_rev[i]=rule->node;
				i++;	
				rule=rule->next;
			}
			
			//printf("NUmber of rules of RHS = %d\n",i);
			for(int j=i-1;j>=0;j--)
			{
				st=push(st,rhs_rev[j]);
			}

			isFirst=1;

		}
	
		else if(top->data->t_item->tag==1)
		{
			temp=(node *)malloc(sizeof(node));
			temp->token=nextToken;

			if(top->data->t_item->index==nextToken->index)
			{
				if(isFirst)
				{
					currentNode=insertAsChild(currentNode, temp, 1);
					isFirst=0;
				}
				else
				{
					currentNode=insertAsNextRightSibling(currentNode, temp, 1);
				}

				nextToken=getNextToken();
				validateLexError();
			}
			else
			{

				num_errors++;
				printf("UNEXPECTED TOKEN!!");
				printf("LINE NUMBER =  %d  LEXEME = %s  TOKEN_NAME =  %s\n",nextToken->LN,nextToken->lexeme,tokensList[nextToken->index]->key);
				nextToken=getNextToken();
				validateLexError();

				continue;
			}
			
		}
	}
	
	if((nextToken->index)!=(ht_search(mapping_table,"$")->data->t_item->index))
	{
			printf("EXTRA TOKENS FOUND \n");
			error_list* new_error=(error_list*)malloc(sizeof(error_list));
			new_error->tk=nextToken;
			new_error->next=errors;
			errors=new_error;
	}
/*
	printf("DONE PARSING ..\n");
	if(root!=NULL)
		if(root->tag==1)
			printf("Root = %s\n",root->data->token->lexeme);
		else
			printf("Root = %s\n",root->data->nonterm->key);
	printTraversal(root);
*/

	printParseTree(root);

	printf("TOTAL NUMBER OF ERRORS ENCOUNTERED = %d\n",num_errors);

	return errors;
}

error_list* parseTree2(){

	num_errors=0;

	root=NULL;

	errors=NULL;

	stack *st=(stack*)malloc(sizeof(stack));

	ht_item* rhs_rev[MAX_RHS];

	ht_item* bottom=ht_search(mapping_table,"$");

	st->data=bottom;
	st->next=NULL;
	st->prev=NULL;

	ht_item* start=ht_search(mapping_table,"program");
	st = push(st,start);

	ht_item* end_marker=(ht_item *)malloc(sizeof(ht_item));
	ht_data* k = malloc(sizeof(ht_data));
	term_item* i = malloc(sizeof(term_item));
	k->t_item = i;
	end_marker->data = k;

	end_marker->data->t_item->index=-2;

	end_marker->key=NULL;

	ht_item *top;

	node* temp=(node *)malloc(sizeof(node));
	temp->nonterm = start;
	
	root = insertAsChild(NULL,temp,2);
	treenode* currentNode = root;
	treenode* tempNode = NULL;
	
	nextToken=getNextToken();
	validateLexError();

	while(peek(st)!=NULL && peek(st)!=bottom)
	{
		//printf("q %s\n",peek(st)->key);
		//printf("test at %s\n", currentNode->data->nonterm->key);
		//printf("test at %s\n", peek(st)->key);
		//printf("test at %sppp\n", nextToken->lexeme);
		top=peek(st);
		if(top==end_marker){
			while(peek(st)==end_marker)
			{
				st=pop(st);
				tempNode = currentNode;
				if(currentNode!=NULL)
				currentNode=currentNode->parent;
			}
			if(tempNode!=NULL && tempNode->right!=NULL)
			currentNode = tempNode->right;
			continue;}
		//printf("Popped Element = %s\n",top->key);	
		if(top!=NULL && top->data->t_item->index==ht_search(mapping_table,"e")->data->t_item->index)
		{
			if(currentNode==NULL)
			printf("w");
			while(currentNode!=NULL && currentNode->right==NULL){currentNode = currentNode->parent;st=pop(st);}
			//printf("test at %s", currentNode->data->nonterm->key);
			//printf("test at %s", peek(st)->key);
			//printf("test at %sppp", nextToken->lexeme);
			continue;
		}

		
		while(1)
		{
			if(nextToken==NULL){break;}
			if(top->data->t_item->tag==1 && top->data->t_item->index == nextToken->index)
			{
				//printf("w %d \n",nextToken->index);
				st=pop(st);
				temp=(node *)malloc(sizeof(node));
				temp->token = nextToken;
				currentNode = insertAsChild(currentNode,temp,1);
				currentNode = currentNode->parent;
				while(currentNode!=NULL && currentNode->right==NULL){currentNode = currentNode->parent;st=pop(st);}
				currentNode = currentNode->right;
				nextToken=getNextToken();
				validateLexError();
				break;

				/*st=pop(st);
				if(currentNode!=NULL && currentNode->data!=NULL)
				{
				currentNode->data->token = nextToken;
				currentNode->tag = 1;
				while(currentNode!=NULL && currentNode->right==NULL){currentNode = currentNode->parent;st=pop(st);}
				currentNode = currentNode->right;
				}
				nextToken=getNextToken();
				validateLexError(nextToken);
				break;*/
			}
			else if(top->data->t_item->tag==2 && parse_table[top->data->t_item->index][nextToken->index]!=-1)
			{
				st=pop(st);
				rule_rhs* rule = rules[parse_table[top->data->t_item->index][nextToken->index]].key;
				int i=1;
				rhs_rev[0]=rule->node;
				temp=(node *)malloc(sizeof(node));
				temp->nonterm = rhs_rev[0];
				currentNode = insertAsChild(currentNode,temp,2);
				tempNode = currentNode;
				rule=rule->next;
				while(rule)
					{
						rhs_rev[i]=rule->node;
						temp=(node *)malloc(sizeof(node));
						temp->nonterm = rhs_rev[i];
						tempNode = insertAsNextRightSibling(tempNode,temp,2);
						i++;	
						rule=rule->next;
					}
			
					//printf("Number of rules of RHS = %d\n",i);
					st=push(st,end_marker);
					for(int j=i-1;j>=0;j--)
					{
						st=push(st,rhs_rev[j]);
					}
					break;
			}
			else if(top->data->t_item->tag==2 && parse_table[top->data->t_item->index][ht_search(mapping_table,"e")->data->t_item->index]!=-1)
			{
				st=pop(st);
				break;
			}
			else
			{
				num_errors++;
				printf("UNEXPECTED TOKEN.\n");
				printf("%s %d",nextToken->lexeme,nextToken->LN);
				error_list* new_error=(error_list*)malloc(sizeof(error_list));
				new_error->tk=nextToken;
				new_error->next=errors;
				errors=new_error;
				nextToken=getNextToken();
				validateLexError();
			}
		}
		if((nextToken==NULL) && peek(st)!=bottom)
		{
			printf("REACHED END OF PROGRAM WITHOUT COMPLETE PARSE TREE GENERATION\n");
			error_list* new_error=(error_list*)malloc(sizeof(error_list));
			new_error->tk=nextToken;
			new_error->next=errors;
			errors=new_error;
			return errors;
		}
	}

	
	if(nextToken->index!=0)
	{
			printf("Extra Tokens Found %s %d \n",nextToken->lexeme,nextToken->LN);
			error_list* new_error=(error_list*)malloc(sizeof(error_list));
			new_error->tk=nextToken;
			new_error->next=errors;
			errors=new_error;
	}
	else
	{
		printf("Parsing complete...");
	}

	printParseTree(root);

	printf("TOTAL NUMBER OF ERRORS ENCOUNTERED = %d\n",num_errors);

	return errors;
}


void printParseTree(treenode* root)
{
	FILE* fp=fopen("parse_tree.txt","w");
	
	fprintf(fp,"lexeme\t\tlineno\t\ttokenName\t\tvalueIfNumber\t\tparentNodeSymbol\tisLeafNode(yes/no)\tNodeSymbol\n");

	printTraversal(root,fp);

	fclose(fp);
}
void countparsetree(treenode *t)
{
	if(t!=NULL)
	{
		if(t->child==NULL)
		{
			c_pars = c_pars + 1;
		}
		else
		{
			countparsetree(t->child);
			c_pars = c_pars + 1;
			treenode* rt=t->child->right;
			while(rt!=NULL)
			{
				countparsetree(rt);
				rt=rt->right;
			}
		}
	}
}

