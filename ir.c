/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "hash_table.h"
#include "parseRules.h"
#include "utils.h"
#include "adt.h"
#include "ast.h"
#include "symbol_table.h"
#include "ir.h"

const char *OPNames[] = {"START","NOP","CALL","RET","DEC","PRINT","INP","ARR_GET","ARR_ASSIGN","BRANCH", "JUMP","ASSIGN","INC","EQUATE","UNARY","PLUS","MINUS","MUL","DIV,","AND","OR","LT","LE","GT","GE","EQ","NE","PUSH","POP"};
void printIR(quad_row *q)
{
	if(q!=NULL)
	{
		printf("%d \t\t",q->srno);
		printf("%s \t\t",OPNames[q->op]);
		for(int i=0;i<3;i++)
		{
			if(q->tag[i]==0) printf("%d \t\t",q->val[i].v_item->offset);
			else if(q->tag[i]==1) printf("%s \t\t",q->val[i].tk_item->lexeme);
			else if(q->tag[i]==2) printf("%d \t\t",q->val[i].qr_item->srno);
			else if(q->tag[i]==3) printf("%d \t\t",q->val[i].t_item->offset);
			else printf("\t\t");
		}
		printf("\n");
		printIR(q->next);
	}
}


void genIRTable(astnode *t,symnode* sym)
{
	if(t!=NULL)
	{
		if(t->child==NULL)
		{
			if(t->tag==1)
			{
				return;
			}
		}
		else
		{
			int i=t->data->nonterm->data->t_item->index;
			bool default_flag = true;
			if(i==ht_search(mapping_table, "moduleDeclarations")->data->t_item->index)
			{
				default_flag = false;
				genIRTable(t->child->right,sym);
			}
			else if(i==ht_search(mapping_table, "input_plist")->data->t_item->index || i==ht_search(mapping_table, "output_plist")->data->t_item->index)
			{
				default_flag = false;
				astnode* rt=t;
				while(rt!=NULL)
				{
				quad_row *p = getEmptyRow();
				if(i==ht_search(mapping_table, "input_plist")->data->t_item->index) 
				{p->op=POP; p->val[0].v_item = ht_search(ht_search(sym_root->symbol_table,t->parent->child->data->token->lexeme)->data->f_item->pr->input_list, rt->child->data->token->lexeme)->data->v_item;}
				else {p->op=PUSH; p->val[0].v_item = ht_search(ht_search(sym_root->symbol_table,t->parent->parent->child->data->token->lexeme)->data->f_item->pr->output_list, rt->child->data->token->lexeme)->data->v_item;}
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				rt = rt->child->right->right;
				}
			}
			else if(i==ht_search(mapping_table, "declareStmt")->data->t_item->index)
			{
				default_flag = false;
				astnode* rt=t->child;
				while(rt!=NULL)
				{
				quad_row *p = getEmptyRow();
				p->op=DEC;
				p->next = NULL;
				p->tag[0] = 0;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->val[0].v_item = getVar(sym, rt->child->data->token)->data->v_item;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				rt = rt->child->right;
				}
			}
			else if(i==ht_search(mapping_table, "ioStmt")->data->t_item->index)
			{
				default_flag = false;
				quad_row *p = getEmptyRow();
				p->next = NULL;
				p->tag[1] = -1;
				p->tag[2] = -1;
				if(t->child->child==NULL)
				{
					ht_item* temp = getVar(sym, t->child->data->token);
					if(temp!=NULL)
					{
					p->val[0].v_item = temp->data->v_item;
					p->op=INP;
					p->tag[0] = 0;
					}
					else
					{
					p->val[0].tk_item = t->child->data->token;
					p->op=PRINT;
					p->tag[0] = 1;
					}
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
				}
				else
				{
					ht_item* temp = getVar(sym, t->child->child->data->token);
					if(t->child->child->right==NULL)
					{
					p->val[0].v_item = temp->data->v_item;
					p->op=PRINT;
					p->tag[0] = 0;
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
					}
					else
					{
					quad_row *p2 = getEmptyRow();
					p2->srno = p->srno;
					p->srno = p->srno + 1;
					p2->prev = quad_row_head;
					p2->next = p;
					p2->op=ARR_GET;
					p2->tag[0] = 0;
					p2->tag[2] = 3;
					p2->val[0].v_item = temp->data->v_item;
					ht_item* temp2 = getVar(sym, t->child->child->right->child->data->token);
					if(temp2!=NULL){p2->tag[1] = 0;p2->val[1].v_item = temp2->data->v_item;}
					else {p2->tag[1] = 1;p2->val[1].tk_item = t->child->child->right->child->data->token;}
					p2->val[2].t_item = getTempVariable(INTEGER);
					p->val[0].t_item = p2->val[2].t_item;
					p->op=PRINT;
					p->tag[0] = 3;
					p->prev = p2;
					quad_row_head->next = p2;
					quad_row_head = p;
					}
				}

			}
			else if(i==ht_search(mapping_table, "iterativeStmt")->data->t_item->index)
			{
				if(t->child->right->right==NULL)
				{
				default_flag = false;
				sym=getNextSymbolTable();
				quad_row *temp1 = quad_row_head;
				quad_row *temp2 = quad_row_head;
				quad_row *block = getEmptyRow(); srno = srno - 1;
				quad_row *loopexit = getEmptyRow();  srno = srno - 1;
				genIRTable(t->child,sym);

				quad_row *p = getEmptyRow();
				p->op=BRANCH;
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = 2;
				p->tag[2] = 2;
				p->val[0].t_item = quad_row_head->val[2].t_item;
				p->val[1].qr_item = block;
				p->val[2].qr_item = loopexit;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				block->prev = quad_row_head;
				quad_row_head->next=block;
				quad_row_head=block;
				block->srno = srno; srno = srno + 1;

				genIRTable(t->child->right,sym);
				p = getEmptyRow();
				p->op=JUMP;
				p->next = NULL;
				p->tag[0] = 2;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->val[0].qr_item = temp2->next;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				loopexit->srno = srno; srno = srno + 1;
				loopexit->prev = quad_row_head;
				quad_row_head->next=loopexit;
				quad_row_head = loopexit;
				}
				else
				{
				default_flag = false;
				quad_row *temp1 = quad_row_head;
				quad_row *block = getEmptyRow(); srno = srno - 1;
				quad_row *loopexit = getEmptyRow(); srno = srno - 1;
				var_item* loop_var = getVar(sym, t->child->data->token)->data->v_item;

				quad_row *p = getEmptyRow();
				p->op=ASSIGN;
				p->next = NULL;
				p->tag[0] = -1;
				p->tag[1] = 1;
				p->tag[2] = -1;
				p->val[0].v_item = loop_var;
				p->val[1].tk_item = t->child->right->child->data->token;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				
				p = getEmptyRow();
				p->op=LT;
				p->next = NULL;
				p->tag[0] = 0;
				p->tag[1] = 1;
				p->tag[2] = 3;
				p->val[0].v_item = loop_var;
				p->val[1].tk_item = t->child->right->child->right->data->token;
				p->val[2].t_item = getTempVariable(BOOLEAN);
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;

				p = getEmptyRow();
				p->op=BRANCH;
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = 2;
				p->tag[2] = 2;
				p->val[0].t_item = quad_row_head->val[2].t_item;
				p->val[1].qr_item = block;
				p->val[2].qr_item = loopexit;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;

				quad_row_head->next=block;
				block->prev = quad_row_head;
				quad_row_head=block;
				block->srno = srno; srno = srno + 1;
				sym=getNextSymbolTable();
				genIRTable(t->child->right->right,sym);
				p = getEmptyRow();
				p->op=INC;
				p->next = NULL;
				p->tag[0] = 0;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->val[0].v_item = loop_var;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;

				p = getEmptyRow();
				p->op=JUMP;
				p->next = NULL;
				p->tag[0] = 2;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->val[0].qr_item = temp1->next->next;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				loopexit->srno = srno; srno = srno + 1;
				quad_row_head->next=loopexit;
				loopexit->prev = quad_row_head;
				quad_row_head = loopexit;
				}

			}
			else if(i==ht_search(mapping_table, "conditionalStmt")->data->t_item->index)
			{
				default_flag = false;
				quad_row *switchexit = getEmptyRow(); srno = srno - 1;
				var_item* case_var = getVar(sym, t->child->data->token)->data->v_item;
				sym=getNextSymbolTable();

				astnode* rt=t->child->right;
				while(rt!=NULL)
				{
				quad_row *caseexit = getEmptyRow(); srno = srno - 1;
				quad_row *p = getEmptyRow();
				p->op=EQUATE;
				p->next = NULL;
				p->tag[0] = 0;
				p->tag[1] = 1;
				p->tag[2] = 3;
				p->val[0].v_item = case_var;
				p->val[1].tk_item = rt->child->data->token;
				p->val[2].t_item = getTempVariable(BOOLEAN);
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				p = getEmptyRow();
				p->op=BRANCH;
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = 2;
				p->tag[2] = 2;
				p->val[0].t_item = quad_row_head->val[2].t_item;
				p->val[1].qr_item = NULL;
				p->val[2].qr_item = caseexit;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;

				sym=getNextSymbolTable();
				genIRTable(rt->child->right,sym);
				quad_row *p2 = getEmptyRow();
				p2->op=JUMP;
				p2->next = NULL;
				p2->tag[0] = 2;
				p2->tag[1] = -1;
				p2->tag[2] = -1;
				p2->val[0].qr_item = switchexit;
				p2->prev = quad_row_head;
				quad_row_head->next = p2;
				quad_row_head = p2;

				p->val[1].qr_item = p->next;
				caseexit->srno = srno; srno = srno + 1;
				quad_row_head->next=caseexit;
				caseexit->prev = quad_row_head;
				quad_row_head = caseexit;
				rt = rt->child->right->right;
				}
				rt=t->child->right->right;
				if(rt!=NULL)
				{
				sym=getNextSymbolTable();
				genIRTable(rt->child,sym);
				quad_row *p2 = getEmptyRow();
				p2->op=JUMP;
				p2->next = NULL;
				p2->tag[0] = 2;
				p2->tag[1] = -1;
				p2->tag[2] = -1;
				p2->val[0].qr_item = switchexit;
				p2->prev = quad_row_head;
				quad_row_head->next = p2;
				quad_row_head = p2;
				}
				switchexit->srno = srno; srno = srno + 1;
				quad_row_head->next=switchexit;
				switchexit->prev = quad_row_head;
				quad_row_head = switchexit;
			}
			else if(i==ht_search(mapping_table, "driverModule")->data->t_item->index)
			{
				sym=getNextSymbolTable();
				setNextSymbolTable(NULL);
				quad_row *driverstart = getEmptyRow();
				quad_row *p = getEmptyRow(); srno = srno - 1;
				p->srno = 0;
				p->op=JUMP;
				p->next = NULL;
				p->tag[0] = 2;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->val[0].qr_item = driverstart;
				p->next = quad_row_tail->next;
				p->prev = quad_row_tail;
				if(quad_row_tail->next!=NULL) quad_row_tail->next->prev = p;
				quad_row_tail->next = p;
				driverstart->prev = quad_row_head;
				quad_row_head->next = driverstart;
				quad_row_head = driverstart;
			}
			else if(i==ht_search(mapping_table, "module")->data->t_item->index)
			{
				default_flag = false;
				sym=getNextSymbolTable();
				setNextSymbolTable(ht_search(sym_root->symbol_table,t->child->data->token->lexeme)->data->f_item);
				quad_row *funcstart = getEmptyRow();
				ht_item* h = ht_search(label_table,t->child->data->token->lexeme);
				if(h==NULL)
				{
				h = ht_insert_label_item(label_table, t->child->data->token->lexeme, false);
				h->data->l_item->label = funcstart;
				}
				else 
				{
				quad_row *temp = h->data->l_item->label;
				temp->srno = funcstart->srno;
				funcstart = temp;
				}

				funcstart->prev = quad_row_head;
				quad_row_head->next = funcstart;
				quad_row_head = funcstart;

				genIRTable(t->child->right,sym);
				genIRTable(t->child->right->right->right,sym);
				genIRTable(t->child->right->right,sym);
				quad_row *p = getEmptyRow();
				p->op=RET;
				p->next = NULL;
				p->tag[0] = -1;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
			}
			else if(i==ht_search(mapping_table, "optional")->data->t_item->index)
			{
				default_flag = false;
				astnode* rt=t->child;
				quad_row *p;
				while(rt->child->right!=NULL) rt = rt->child->right;
				while(rt!=t->child->parent)
				{
				p = getEmptyRow();
				p->op=POP;
				p->next = NULL;
				p->tag[0] = 0;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->val[0].v_item = getVar(sym, rt->child->data->token)->data->v_item;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				rt = rt->parent;
				}
			}
			else if(i==ht_search(mapping_table, "moduleReuseStmt")->data->t_item->index)
			{
				default_flag = false;
				astnode* rt;
				quad_row *funcstart;
				if(t->child->right->right!=NULL) rt=t->child->right;
				else rt=t->child;
				ht_item* h = ht_search(label_table,rt->data->token->lexeme);
				if(h==NULL)
				{
				funcstart = getEmptyRow(); srno = srno - 1;
				ht_item* h = ht_insert_label_item(label_table, rt->data->token->lexeme, false);
				h->data->l_item->label = funcstart;
				}
				else {funcstart = h->data->l_item->label;}

				rt=rt->right;
				quad_row *p;
				while(rt->child->right!=NULL) rt = rt->child->right;
				while(rt!=t->child->parent)
				{
				p = getEmptyRow();
				p->op=PUSH;
				p->next = NULL;
				p->tag[0] = 0;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->val[0].v_item = getVar(sym, rt->child->data->token)->data->v_item;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				rt = rt->parent;
				}

				p = getEmptyRow();
				p->op=CALL;
				p->next = NULL;
				p->tag[0] = 2;
				p->tag[1] = -1;
				p->tag[2] = -1;
				p->val[0].qr_item = funcstart;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;

				if(t->child->right->right!=NULL) genIRTable(t->child,sym);
			}
			else if(i==ht_search(mapping_table, "assignmentStmt")->data->t_item->index)
			{
				default_flag = false;
				astnode* rt=t->child->right->child;
				if(rt->right==NULL)
				{
				genIRTable(rt,sym);
				quad_row *p = getEmptyRow();
				p->op=ASSIGN;
				p->next = NULL;
				p->tag[0] = 3; 
				p->tag[1] = -1;
				p->tag[2] = 0;
				p->val[0].t_item = quad_row_head->val[2].t_item;
				p->val[2].v_item = getVar(sym, t->child->data->token)->data->v_item;
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				}
				else
				{
				genIRTable(rt->right,sym);
				quad_row *p = getEmptyRow();
				p->op=ARR_ASSIGN;
				p->next = NULL;
				p->tag[0] = 3; 
				p->tag[1] = -1;
				p->tag[2] = 0;
				p->val[0].t_item = quad_row_head->val[2].t_item;
				p->val[2].v_item = getVar(sym, t->child->data->token)->data->v_item;
				ht_item* temp2 = getVar(sym, rt->data->token);
				if(temp2!=NULL){p->tag[1] = 0;p->val[1].v_item = temp2->data->v_item;}
				else {p->tag[1] = 1;p->val[1].tk_item = rt->data->token;}
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				}
			}
			else if(i==ht_search(mapping_table, "U")->data->t_item->index)
			{
				default_flag = false;
				astnode* rt=t->child->right;
				if(rt->child==NULL)
				{
					quad_row *p = getEmptyRow();
					p->val[1].tk_item = rt->data->token;
					p->op=UNARY;
					p->val[0].tk_item = t->child->data->token;
					p->tag[0] = 1;
					p->tag[1] = 1;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(rt->attr->baseType);
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
				}
				else if(rt->child->child==NULL)
				{
					ht_item* temp = getVar(sym, rt->child->data->token);
					quad_row *p = getEmptyRow();
					if(rt->child->right==NULL)
					{
					p->val[1].v_item = temp->data->v_item;
					p->op=UNARY;
					p->val[0].tk_item = t->child->data->token;
					p->tag[0] = 1;
					p->tag[1] = 0;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(rt->child->attr->baseType);
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
					}
					else
					{
					quad_row *p2 = getEmptyRow();
					p2->srno = p->srno;
					p->srno = p->srno + 1;
					p2->prev = quad_row_head;
					p2->next = p;
					p2->op=ARR_GET;
					p2->tag[0] = 0;
					p2->tag[2] = 3;
					p2->val[0].v_item = temp->data->v_item;
					ht_item* temp2 = getVar(sym, rt->child->right->child->data->token);
					if(temp2!=NULL){p2->tag[1] = 0;p2->val[1].v_item = temp2->data->v_item;}
					else {p2->tag[1] = 1;p2->val[1].tk_item = rt->child->right->child->data->token;}
					p2->val[2].t_item = getTempVariable(rt->attr->baseType);
					p->val[0].tk_item = t->child->data->token;
					p->tag[0] = 1;
					p->val[2].t_item = getTempVariable(rt->attr->baseType);
					p->tag[2] = 3;
					p->val[1].t_item = p2->val[2].t_item;
					p->op=UNARY;
					p->tag[1] = 3;
					p->prev = p2;
					quad_row_head->next = p2;
					quad_row_head = p;
					}
				}
				else
				{
					genIRTable(rt->child,sym);
					quad_row *p = getEmptyRow();
					p->val[1].t_item = quad_row_head->val[2].t_item;
					p->op=UNARY;
					p->val[0].tk_item = t->child->data->token;
					p->tag[0] = 1;
					p->tag[1] = 3;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(rt->attr->baseType);
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
				}
			}
			else if(i==ht_search(mapping_table, "arithmeticExpr")->data->t_item->index)
			{
				default_flag = false;
				genIRTable(t->child,sym);
				genIRTable(t->child->right,sym);
			}
			else if(i==ht_search(mapping_table, "N4")->data->t_item->index)
			{
				default_flag = false;
				quad_row *op1 = quad_row_head;
				genIRTable(t->child->right,sym);
				quad_row *op2 = quad_row_head;
				quad_row *p = getEmptyRow();
				if(t->child->data->token->index == ht_search(mapping_table, "PLUS")->data->t_item->index)
				{p->op=PLUS;}
				else {p->op=MINUS;}
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = 3;
				p->tag[2] = 3;
				p->val[0].t_item = op1->val[2].t_item;
				p->val[1].t_item = op2->val[2].t_item;
				p->val[2].t_item = getTempVariable(t->attr->baseType);
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				genIRTable(t->child->right->right,sym);
			}
			else if(i==ht_search(mapping_table, "term")->data->t_item->index)
			{
				default_flag = false;
				if(t->child->right!=NULL)
				{
					if(t->child->child!=NULL) {genIRTable(t->child,sym);}
					else
					{
					quad_row *p = getEmptyRow();
					p->next = NULL;
					p->tag[1] = -1;
					p->tag[2] = -1;
					ht_item* temp = getVar(sym, t->child->data->token);
					if(temp!=NULL)
					{
					p->val[0].v_item = temp->data->v_item;
					p->op=ASSIGN;
					p->tag[0] = 0;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					}
					else
					{
					p->val[0].tk_item = t->child->data->token;
					p->op=ASSIGN;
					p->tag[0] = 1;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					}
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
					}
					genIRTable(t->child->right,sym);
					
				}
				else if(t->child->child!=NULL)
				{
					genIRTable(t->child,sym);
				}
				else
				{
					quad_row *p = getEmptyRow();
					p->next = NULL;
					p->tag[1] = -1;
					p->tag[2] = -1;
					ht_item* temp = getVar(sym, t->child->data->token);
					if(temp!=NULL)
					{
					p->val[0].v_item = temp->data->v_item;
					p->op=ASSIGN;
					p->tag[0] = 0;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					}
					else
					{
					p->val[0].tk_item = t->child->data->token;
					p->op=ASSIGN;
					p->tag[0] = 1;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					}
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
				}
			}
			else if(i==ht_search(mapping_table, "N5")->data->t_item->index)
			{
				default_flag = false;
				quad_row *op1 = quad_row_head;
				if(t->child->right->child!=NULL) 
				{genIRTable(t->child->right,sym);}
				else
				{
					quad_row *p = getEmptyRow();
					p->next = NULL;
					p->tag[1] = -1;
					p->tag[2] = -1;
					ht_item* temp = getVar(sym, t->child->right->data->token);
					if(temp!=NULL)
					{
					p->val[0].v_item = temp->data->v_item;
					p->op=ASSIGN;
					p->tag[0] = 0;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					}
					else
					{
					p->val[0].tk_item = t->child->right->data->token;
					p->op=ASSIGN;
					p->tag[0] = 1;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					}
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
				}
				quad_row *op2 = quad_row_head;
				quad_row *
				p = getEmptyRow();
				if(t->child->data->token->index == ht_search(mapping_table, "MUL")->data->t_item->index)
				{p->op=MUL;}
				else {p->op=DIV;}
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = 3;
				p->tag[2] = 3;
				p->val[0].t_item = op1->val[2].t_item;
				p->val[1].t_item = op2->val[2].t_item;
				p->val[2].t_item = getTempVariable(t->attr->baseType);
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				genIRTable(t->child->right->right,sym);
			}
			else if(i==ht_search(mapping_table, "var_id_num")->data->t_item->index)
			{
				default_flag = false;
				quad_row *p = getEmptyRow();
				p->next = NULL;
				p->tag[1] = -1;
				p->tag[2] = -1;
				if(t->child->right==NULL)
				{
					ht_item* temp = getVar(sym, t->child->data->token);
					if(temp!=NULL)
					{
					p->val[0].v_item = temp->data->v_item;
					p->op=ASSIGN;
					p->tag[0] = 0;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					}
					else
					{
					p->val[0].tk_item = t->child->data->token;
					p->op=ASSIGN;
					p->tag[0] = 1;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					}
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
				}
				else
				{
					ht_item* temp = getVar(sym, t->child->data->token);
					p->prev = quad_row_head;
					p->op=ARR_GET;
					p->tag[0] = 0;
					p->tag[2] = 3;
					p->val[0].v_item = temp->data->v_item;
					ht_item* temp2 = getVar(sym, t->child->right->child->data->token);
					if(temp2!=NULL){p->tag[1] = 0;p->val[1].v_item = temp2->data->v_item;}
					else {p->tag[1] = 1;p->val[1].tk_item = t->child->right->child->data->token;}
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					quad_row_head->next = p;
					quad_row_head = p;
				}
			}
			else if(i==ht_search(mapping_table, "arithmeticOrBooleanExpr")->data->t_item->index)
			{
				default_flag = false;
				genIRTable(t->child,sym);
				genIRTable(t->child->right,sym);
			}
			else if(i==ht_search(mapping_table, "N7")->data->t_item->index)
			{
				default_flag = false;
				quad_row *op1 = quad_row_head;
				if(t->child->data->token->index == ht_search(mapping_table, "AND")->data->t_item->index)
				{
				quad_row *p2 = getEmptyRow();
				p2->op=BRANCH;
				p2->next = NULL;
				p2->tag[0] = 3;
				p2->tag[1] = 2;
				p2->tag[2] = 2;
				p2->val[0].t_item = quad_row_head->val[2].t_item;
				p2->prev = quad_row_head;
				quad_row_head->next = p2;
				quad_row_head = p2;

				genIRTable(t->child->right,sym);
				quad_row *op2 = quad_row_head;
				p2->val[1].qr_item = p2->next;

				quad_row *p3 = getEmptyRow();
				p3->op=JUMP;
				p3->next = NULL;
				p3->tag[0] = 2;
				p3->tag[1] = -1;
				p3->tag[2] = -1;
				p3->prev = quad_row_head;
				quad_row_head->next = p3;
				quad_row_head = p3;

				quad_row *p4 = getEmptyRow();
				p4->next = NULL;
				p4->tag[1] = -1;
				p4->val[0].tk_item = p2->prev->val[2].t_item;
				p4->op=ASSIGN;
				p4->tag[0] = 3;
				p4->tag[2] = 3;
				p4->val[2].t_item = p3->prev->val[2].t_item;
				p4->prev = quad_row_head;
				quad_row_head->next = p4;
				quad_row_head = p4;
				p2->val[2].qr_item = p4;

				quad_row *p = getEmptyRow();
				p->op=AND;
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = 3;
				p->tag[2] = 3;
				p->val[0].t_item = op1->val[2].t_item;
				p->val[1].t_item = op2->val[2].t_item;
				p->val[2].t_item = getTempVariable(BOOLEAN);
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				p3->val[0].qr_item = p;
				}
				else
				{
				quad_row *p2 = getEmptyRow();
				p2->op=BRANCH;
				p2->next = NULL;
				p2->tag[0] = 3;
				p2->tag[1] = 2;
				p2->tag[2] = 2;
				p2->val[0].t_item = quad_row_head->val[2].t_item;
				p2->prev = quad_row_head;
				quad_row_head->next = p2;
				quad_row_head = p2;

				genIRTable(t->child->right,sym);
				quad_row *op2 = quad_row_head;
				p2->val[2].qr_item = p2->next;

				quad_row *p3 = getEmptyRow();
				p3->op=JUMP;
				p3->next = NULL;
				p3->tag[0] = 2;
				p3->tag[1] = -1;
				p3->tag[2] = -1;
				p3->prev = quad_row_head;
				quad_row_head->next = p3;
				quad_row_head = p3;

				quad_row *p4 = getEmptyRow();
				p4->next = NULL;
				p4->tag[1] = -1;
				p4->val[0].tk_item = p2->prev->val[2].t_item;
				p4->op=ASSIGN;
				p4->tag[0] = 3;
				p4->tag[2] = 3;
				p4->val[2].t_item = p3->prev->val[2].t_item;
				p4->prev = quad_row_head;
				quad_row_head->next = p4;
				quad_row_head = p4;
				p2->val[1].qr_item = p4;

				quad_row *p = getEmptyRow();
				p->op=OR;
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = 3;
				p->tag[2] = 3;
				p->val[0].t_item = op1->val[2].t_item;
				p->val[1].t_item = op2->val[2].t_item;
				p->val[2].t_item = getTempVariable(BOOLEAN);
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				p3->val[0].qr_item = p;
				}
				genIRTable(t->child->right->right,sym);
			}
			else if(i==ht_search(mapping_table, "AnyTerm")->data->t_item->index)
			{
				default_flag = false;
				if(t->child->right!=NULL)
				{
					genIRTable(t->child,sym);
					genIRTable(t->child->right,sym);
					
				}
				else if(t->child->child!=NULL)
				{
					genIRTable(t->child,sym);
				}
				else
				{
					quad_row *p = getEmptyRow();
					p->next = NULL;
					p->tag[1] = -1;
					p->tag[2] = -1;
					p->val[0].tk_item = t->child->data->token;
					p->op=ASSIGN;
					p->tag[0] = 1;
					p->tag[2] = 3;
					p->val[2].t_item = getTempVariable(t->attr->baseType);
					p->prev = quad_row_head;
					quad_row_head->next = p;
					quad_row_head = p;
				}
			}
			else if(i==ht_search(mapping_table, "N8")->data->t_item->index)
			{
				default_flag = false;
				quad_row *op1 = quad_row_head;
				genIRTable(t->child->right,sym);
				quad_row *op2 = quad_row_head;
				quad_row *p = getEmptyRow();
				if(t->child->data->token->index == ht_search(mapping_table, "LT")->data->t_item->index)
				{p->op=LT;}
				else if(t->child->data->token->index == ht_search(mapping_table, "LE")->data->t_item->index)
				{p->op=LE;}
				else if(t->child->data->token->index == ht_search(mapping_table, "GT")->data->t_item->index)
				{p->op=GT;}
				else if(t->child->data->token->index == ht_search(mapping_table, "GE")->data->t_item->index)
				{p->op=GE;}
				else if(t->child->data->token->index == ht_search(mapping_table, "EQ")->data->t_item->index)
				{p->op=EQ;}
				else {p->op=NE;}
				p->next = NULL;
				p->tag[0] = 3;
				p->tag[1] = 3;
				p->tag[2] = 3;
				p->val[0].t_item = op1->val[2].t_item;
				p->val[1].t_item = op2->val[2].t_item;
				p->val[2].t_item = getTempVariable(t->attr->baseType);
				p->prev = quad_row_head;
				quad_row_head->next = p;
				quad_row_head = p;
				genIRTable(t->child->right->right,sym);
			}
			if(default_flag)
			{
				genIRTable(t->child,sym);
				astnode* rt=t->child->right;
				while(rt!=NULL)
				{
					genIRTable(rt,sym);
					rt=rt->right;
				}

			}
		}
	}
}
symnode* getNextSymbolTable()
{
	if(vis->child!=NULL) vis = vis->child;
	else if(vis->right!=NULL) vis = vis->right;
	else {
		while(vis->right==NULL) vis = vis->parent; 
		vis = vis->right;}
	return vis;
}
void setNextSymbolTable(func_item* fsym)
{
	funcsym = fsym;
}
ht_item* getVar(symnode* sym, Token *t)
{
	var_item* x;
	while(sym!=sym_root)
	{
		x = ht_search(sym->symbol_table,t->lexeme);
		if(x!=NULL) return x;
		sym = sym->parent;
	}
	if(funcsym!=NULL)
	{
		x = ht_search(funcsym->pr->input_list, t->lexeme);
		if(x!=NULL) return x;
	}
	if(funcsym!=NULL)
	{
		x = ht_search(funcsym->pr->output_list, t->lexeme);
		if(x!=NULL) return x;
	}
	return NULL;
}
quad_row * getEmptyRow()
{
	quad_row *p = malloc(sizeof(quad_row));
	p->srno = srno;
	srno = srno + 1;
	p->prev = NULL;
	p->next = NULL;
	p->op=NOP;
	p->tag[0] = -1;
	p->tag[1] = -1;
	p->tag[2] = -1;
}
void resetIRParams()
{
	label_table=ht_new();
	vis = sym_root;
	tempLabel = 0;
	srno = -1;
	quad_row_head = getEmptyRow();
	srno = 1;
	quad_row_head->op=START;
	quad_row_head->next = NULL;
	quad_row_head->prev = NULL;
	quad_row_head->tag[0] = -1;
	quad_row_head->tag[1] = -1;
	quad_row_head->tag[2] = -1;
	quad_row_tail = quad_row_head;
}
temp_item* getTempVariable(VarType baseType)
{
	temp_item* x = malloc(sizeof(temp_item));
	x->offset = tempLabel;
	x->baseType = baseType;
	tempLabel = tempLabel + 1;
	return x;
}
