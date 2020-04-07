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
#include <time.h>
#include "lexer.h"
#include "hash_table.h"
#include "parseRules.h"
#include "utils.h"
#include "parseTable.h"
#include "adt.h"
#include "ast.h"
#include "parser.h"
#include "semCheck.h"


int main(int argc,char** argv)
{
	num_terminals=0;
	num_nonterminals=0;

	printf("Start..\n");

	mapping_table= ht_new();
	
	keyword_table= ht_new();

	func_table= ht_new();

	int i=0;
	char line[40];
	size_t len = 0;
	ssize_t read;
	clock_t start_time, end_time;

	double total_CPU_time, total_CPU_time_in_seconds;

	FILE* keyword_file=fopen("keywords.txt","r");	

    	if (keyword_file == NULL)
	{
		printf("UNABLE TO OPEN FILE KEYWORDS.TXT");
        	exit(EXIT_FAILURE);
	}

	while (fgets(line, 40, keyword_file)!=NULL) {
        	line[strcspn(line, "\n")] = 0;
		ht_insert_term_item(keyword_table, line, i++,KEYWORD);
		
        	//printf("%s", line);
    	}

	fclose(keyword_file);

	i=0;

	FILE* terminal_file=fopen("terminals.txt","r");
	
    	if (terminal_file == NULL)
	{
		printf("UNABLE TO OPEN FILE TERMINALS.TXT");
        	exit(EXIT_FAILURE);
	}

	while (fgets(line, 40, terminal_file)!=NULL) {
        	line[strcspn(line, "\n")] = 0;
		tokensList[i]=ht_insert_term_item(mapping_table,line, i,TERMINAL);
		num_terminals++;		
		i++;
		
        	//printf("%s", line);
    	}

	fclose(terminal_file);

	printf("Number of terminals = %d\n",num_terminals);
	FILE* nonterminal_file=fopen("nonterminals.txt","r");
	
    	if (nonterminal_file == NULL)
	{
		printf("UNABLE TO OPEN FILE NONTERMINALS.TXT");
        	exit(EXIT_FAILURE);
	}
	while (fgets(line, 40, nonterminal_file)!=NULL) {
        	line[strcspn(line, "\n")] = 0;
		tokensList[i]=ht_insert_term_item(mapping_table,line, i,NONTERMINAL);
		num_nonterminals++;
		i++;
        	
    	}

	fclose(nonterminal_file);


	printf("PARSING RULES\n");

	parse("rules.txt");
	
	printf("RULES PARSING COMPLETE \n");
			
	printf("CREATING PARSE TABLE\n");

	create_parse_table();
	//ht_insert_term_item(mapping_table, "qwerty", 1007,KEYWORD);
	//printf("%s %d",ht_search(mapping_table,"qwerty")->key,ht_search(mapping_table,"qwerty")->data->t_item->index);
	//printf("%s",tokensList[0]->key);

	printf("CREATION OF PARSE TABLE COMPLETE\n");

	while(1)
	{
		int choice=0;

		printf("\n\nDRIVER PROGRAM ..\n\n");

		printf("FIRST AND FOLLOW SETS CALCULATION AUTOMATED..\n");

		printf("BOTH LEXICAL ANALYSER AND PARSER IMPLEMENTED\n");
		
		printf("ALL TEST CASES WORKING, ERROR RECOVERY USING FIRST SET\n");
		
		printf("\nENTER: \n 0: FOR EXITING \n 1: FOR COMMENT REMOVAL \n 2: FOR LEXICAL ANALYSIS \n 3: FOR LEXICAL ANALYSIS AND PARSING \n 4: FOR LEXICAL ANALYSIS , PARSING AND SEMANTIC ANALYSIS\n");
		
		scanf("%d",&choice);	

		switch(choice)
		{
			case 0: exit(0);

			case 1:{
				printf("ENTER THE DESTINATION FILE NAME:\n");
				char dst[30];
				scanf("%s",dst);
				removeComments(argv[1],dst);
				break;
			}

			case 2: {

					forwardPointer=0;
				
					backPointer=0;

					LN=1;

					endReached=0;

					fp=(FILE*)fopen(argv[1],"r");
	
					if(fp==NULL)
					{
						printf("Some error while opening the file");
						exit(1);
					}

					fp=getStream(fp);
	
					forwardPointer=-1;

					Token* next=NULL;
			
					while(1)
					{	
						next=getNextToken();
						if(!next)
							break;
						if(next->index!=-1)
							if(next->index==ht_search(mapping_table,"RNUM")->data->t_item->index)
								printf("LINE NUMBER =  %d  LEXEME = %s  VALUE=  %f  TOKEN_NAME =  %s\n",next->LN,next->lexeme,next->val.f_val,tokensList[next->index]->key);
							else
								printf("LINE NUMBER =  %d  LEXEME = %s  VALUE=  %d  TOKEN_NAME =  %s\n",next->LN,next->lexeme,next->val.i_val,tokensList[next->index]->key);
						else
							printf("LEXICAL ERROR !!! LINE NUMBER =  %d  LEXEME = %s\n",next->LN,next->lexeme);
					}

					printf("Lexical Analysis Successful\n");
	
		
					break;
				}

			case 3:{
					forwardPointer=0;
				
					backPointer=0;

					LN=1;

					endReached=0;
					
					start_time = clock();

					fp=(FILE*)fopen(argv[1],"r");
	
					fseek(fp, 0, SEEK_SET);

					if(fp==NULL)
					{
						printf("Some error while opening the file");
						exit(1);
					}

					fp=getStream(fp);

					forwardPointer=-1;

					parseTree(argv[2]);

					end_time = clock();

					total_CPU_time  =  (double) (end_time - start_time);
					
					total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
			
					break;
				}


			case 4:{
					forwardPointer=0;
				
					backPointer=0;

					LN=1;

					endReached=0;
					
					start_time = clock();

					fp=(FILE*)fopen(argv[1],"r");
	
					fseek(fp, 0, SEEK_SET);

					if(fp==NULL)
					{
						printf("Some error while opening the file");
						exit(1);
					}

					fp=getStream(fp);

					forwardPointer=-1;

					parseTree(argv[2]);

					astnode *xp = createAST(root);

					FILE* fp2=fopen("tree2.txt","w");
					fprintf(fp2,"lexeme\t\tlineno\t\ttokenName\t\tvalueIfNumber\t\tparentNodeSymbol\tisLeafNode(yes/no)\tNodeSymbol\n");
					printTraversalAst(xp, fp2);
					
					fclose(fp2);

						printf("\n");
					
					checkSemRules(xp);

					end_time = clock();

					total_CPU_time  =  (double) (end_time - start_time);
					
					total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
			
					break;
				}

			case 5:{
				
					printf("TOTAL CPU TIME = %f\n",total_CPU_time);

					printf("TOTAL TIME IN SECONDS TAKEN BY THE PARSER AND LEXER = %f",total_CPU_time_in_seconds);

					break;
				}

			default: printf("INVALID ENTRY\n");
		}
	}

	exit(0);
}
