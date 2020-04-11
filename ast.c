/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "hash_table.h"
#include "parseRules.h"
#include "utils.h"
#include "adt.h"
#include "ast.h"

astnode* createAST(treenode* root)
{
	if(root!=NULL)
	{
		if(root->child==NULL)
		{
			if(root->tag==1)
			{
				return makeleaf(root->data->token);
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			if(root->child->right==NULL)
			{
				return createAST(root->child);
			}
			treenode* rt=root->child;
			astnode* rc[4];
			int i=0;
			for(i=0;i<4;i++){rc[i]=NULL;}
			i=0;
			while(rt!=NULL)
			{
				if(rt->tag==2 || (rt->tag==1 && (rt->data->token->index ==2||rt->data->token->index ==3||rt->data->token->index ==4))){rc[i] = createAST(rt);}
				if(rc[i]!=NULL){i++;}
				rt=rt->right;
			}
			if(rc[0]==NULL)
			{return rc[0];}
			else if(rc[1]==NULL)
			{return makenode1(root->data->nonterm, rc[0]);}
			else if(rc[2]==NULL)
			{return makenode2(root->data->nonterm, rc[0], rc[1]);}
			else if(rc[3]==NULL)
			{return makenode3(root->data->nonterm, rc[0], rc[1], rc[2]);}
			else
			{return makenode4(root->data->nonterm, rc[0], rc[1], rc[2], rc[3]);}
		}
	}
}

astnode* makeleaf(Token *tk)
{
	astnode * an = malloc(sizeof(astnode));
	node *d = malloc(sizeof(node));
	d->token = tk;
	an->attr=malloc(sizeof(attributes));
	an->attr->baseType=0;
	an->attr->eleType=0;
	an->attr->low=-1;
	an->attr->high=-1;
	an->data = d;
	an->child = NULL;
	an->right = NULL;
	an->parent = NULL;
	an->tag = 1;
	return an;
}
astnode* makeastnode(ht_item *t)
{
	astnode * an = malloc(sizeof(astnode));
	node *d = malloc(sizeof(node));
	d->nonterm = t;
	an->attr=malloc(sizeof(attributes));
	an->attr->baseType=0;
	an->attr->eleType=0;
	an->attr->low=-1;
	an->attr->high=-1;
	an->data = d;
	an->child = NULL;
	an->right = NULL;
	an->parent = NULL;
	an->tag = 3;
	return an;
}
astnode* makenode1(ht_item *t, astnode *n1)
{
	astnode * an = makeastnode(t);
	astnode *lc = insertChildInAst(an, n1);
	return an;
}
astnode* makenode2(ht_item *t, astnode *n1, astnode *n2)
{
	astnode * an = makeastnode(t);
	astnode *lc = insertChildInAst(an, n1);
	lc = insertNextRightSiblingInAst(lc, n2);
	return an;
}
astnode* makenode3(ht_item *t, astnode *n1, astnode *n2, astnode *n3)
{
	astnode * an = makeastnode(t);
	astnode *lc = insertChildInAst(an, n1);
	lc = insertNextRightSiblingInAst(lc, n2);
	lc = insertNextRightSiblingInAst(lc, n3);
	return an;
}
astnode* makenode4(ht_item *t, astnode *n1, astnode *n2, astnode *n3, astnode *n4)
{
	astnode * an = makeastnode(t);
	astnode *lc = insertChildInAst(an, n1);
	lc = insertNextRightSiblingInAst(lc, n2);
	lc = insertNextRightSiblingInAst(lc, n3);
	lc = insertNextRightSiblingInAst(lc, n4);
	return an;
}

astnode *insertChildInAst(astnode *parent, astnode *child)
{
	if(child==NULL)
	{return NULL;}
	if(parent==NULL)
	{return NULL;}
	parent->child = child;
	child->parent = parent;
	return child;
}
astnode *insertNextRightSiblingInAst(astnode *child, astnode *right)
{
	if(child==NULL)
	{return NULL;}
	if(right==NULL)
	{return NULL;}
	child->right = right;
	right->parent = child->parent;
	return right;
}
astnode *getRootInAst(astnode *child)
{
	astnode *t = child;
	if(t==NULL)
	{return NULL;}
	while(t->parent!=NULL)
	{
		t=t->parent;
	}
	return t;
}
void printTraversalAst(astnode *t,FILE* fp)
{
	if(t!=NULL)
	{
		if(t->child==NULL)
		{
			if(t->tag==1)
			{
			//	printf(" %d ",t->data->token->index);
				fprintf(fp,"%s\t\t%d\t\t%s\t\t\t\t%d\t\t%s\t\tYES\n",t->data->token->lexeme,t->data->token->LN,tokensList[t->data->token->index]->key,t->data->token->val.i_val,t->parent->data->nonterm->key);
			}
			else
			{
			//	printf(" %s ",t->data->nonterm->key);
				fprintf(fp,"%s\t\t\t\tEPSILON\t\t\t\t\t\t%s\t\tYES\t\n",t->data->nonterm->key,t->parent->data->nonterm->key);
			}
		}
		else
		{

			if(t->parent!=NULL)
				fprintf(fp,"----\t\t\t\t\t\t\t\t\t\t%s\t\t\tNO\t\t%s\n",t->parent->data->nonterm->key,t->data->nonterm->key);
			else
				fprintf(fp,"----\t\t\t\t\t\t\t\t\t\tROOT\t\t\tNO\t\t%s\n",t->data->nonterm->key);

			printTraversalAst(t->child,fp);
			//printf("%s --",t->data->nonterm->key);
			astnode* rt=t->child->right;
			while(rt!=NULL)
			{
				printTraversalAst(rt,fp);
				rt=rt->right;
			}
		}
	}
}
