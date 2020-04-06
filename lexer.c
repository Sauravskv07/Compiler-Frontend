/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/
#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "hash_table.h"

int endReached=0;
int j_pointer=0;
int state=1;
int LN=1;
int backPointer=0;
int forwardPointer=0;

FILE * getStream(FILE* fp)
{
	int i=0;
	for(;i<(forwardPointer-backPointer);i++)
	{
		buffer[i]=buffer[backPointer+i];
		//printf("%d  %c \n",i,buffer[i]);
	}

	backPointer=0;
	forwardPointer=i;

	int ch;
	for(;i<MAX_BUFF_SIZE;i++)
	{
		ch=getc(fp);
		if(ch==EOF)
		{
			printf("REACHED END OF PROGRAM\n");
			buffer[i]=0;
			fclose(fp);
			break;
		}
		else
		{
			buffer[i]=(char)ch;
		}
	}
	
	return fp;
}

Token* createToken(int index,int bp,int fwp, int ln)
{
	Token* temp=(Token*) malloc(sizeof(Token));

	int i=bp;	

	for(;i<=fwp;i++)
		temp->lexeme[i-bp]=buffer[i];

	temp->lexeme[i-bp]='\0';

	temp->index=index;
	temp->LN=ln;
	temp->val.f_val=0.0;

	if(index==ht_search(mapping_table,"ID")->data->t_item->index)
	{
		if(ht_search(keyword_table,temp->lexeme))
		{	
			j_pointer=0;
			while(temp->lexeme[j_pointer])
			{
				token[j_pointer]=toupper(temp->lexeme[j_pointer++]);
			}
			token[j_pointer]=0;			

			temp->index=ht_search(mapping_table,token)->data->t_item->index;	
		}
		else
		{
			if(strlen(temp->lexeme)>20)
				temp->index=-1;
		}
	}

	if(index==ht_search(mapping_table,"NUM")->data->t_item->index)
	{	
		temp->val.i_val = atoi(temp->lexeme);
	}

	else if(index==ht_search(mapping_table,"RNUM")->data->t_item->index)
	{
		temp->val.f_val = atof(temp->lexeme);
	}

	return temp;
}

Token* getNextToken()
{
	state=1;

	if(endReached==1)
		return NULL;
		
	while(1)
	{
		forwardPointer++;
	
		if(forwardPointer==MAX_BUFF_SIZE)
		{	
			fp=getStream(fp);
		}
		//printf("forward pointer = %d\n",forwardPointer);
		character_read=buffer[forwardPointer];

		//printf("State = %d  Character Read = %c\n",state,character_read);
				
		switch(state)
		{
			case 1:
			{
				backPointer=forwardPointer;
				
				if(character_read==0)
				{
					endReached=1;
					return createToken(ht_search(mapping_table,"$")->data->t_item->index,0,0,LN);
				}
				else if(character_read=='(')
				{
					return createToken(ht_search(mapping_table,"BO")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read==')')
				{
					return createToken(ht_search(mapping_table,"BC")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read==',')
				{
					return createToken(ht_search(mapping_table,"COMMA")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read=='+')
				{
					return createToken(ht_search(mapping_table,"PLUS")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read=='-')
				{
					return createToken(ht_search(mapping_table,"MINUS")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read=='[')
				{
					return createToken(ht_search(mapping_table,"SQBO")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read==']')
				{
					return createToken(ht_search(mapping_table,"SQBC")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read=='\n')
				{
					state=1;
					LN++;
				}
				else if(character_read==':')
				{
					state=35;
				}
				else if(character_read=='/')
				{
					return createToken(ht_search(mapping_table,"DIV")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read==';')
				{
					return createToken(ht_search(mapping_table,"SEMICOL")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if(character_read=='<')
				{
					state=38;
				}
				else if(character_read=='>')
				{
					state=42;
				}
				else if(character_read=='.')
				{
					state=47;
				}
				else if(character_read==' ' || character_read=='\t')
				{
					state=31;
				}
				else if(character_read=='=')
				{
					state=21;
				}				
				else if(character_read=='!')
				{
					state=18;
				}

				else if(('a'<=character_read && character_read<='z') || ('A'<=character_read && character_read<='Z'))
				{
					state=15;
				}
				else if(character_read=='*')
				{
					state=24;
				}
				else if('0'<=character_read && character_read<='9')
				{
					state=2;
				}
				else{
					return createToken(-1,backPointer,forwardPointer,LN);
				}
				break;
					
			}
			case 35:
			{
				if(character_read=='=')
				{
					return createToken(ht_search(mapping_table,"ASSIGNOP")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"COLON")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 38:
			{
				if(character_read=='<')
				{
					state=43;
				}
				else if(character_read=='=')
				{
					return createToken(ht_search(mapping_table,"LE")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else
				{	
					forwardPointer--;
					return createToken(ht_search(mapping_table,"LT")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 43:
			{

				if(character_read=='<')
				{
					return createToken(ht_search(mapping_table,"DRIVERDEF")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"DEF")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 42:
			{
				if(character_read=='>')
				{
					state=46;
				}
				else if(character_read=='=')
				{
					return createToken(ht_search(mapping_table,"GE")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"GT")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 46:
			{

				if(character_read=='>')
				{
					return createToken(ht_search(mapping_table,"DRIVERENDDEF")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"ENDDEF")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 47:
			{
				if(character_read=='.')
				{
					return createToken(ht_search(mapping_table,"RANGEOP")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else
				{
					forwardPointer--;
					return createToken(-1,backPointer,forwardPointer,LN);
				}
				break;
			}

			case 31:
			{
				if(character_read==' ' ||  character_read=='\t')
				{
					state=31;
				}
				else
				{
					state=1;
					forwardPointer--;
				}
				break;
			}

			case 21:
			{
				if(character_read=='=')
				{
					return createToken(ht_search(mapping_table,"EQ")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else
				{
					forwardPointer--;
					return createToken(-1,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 18:
			{
				if(character_read=='=')
				{
					return createToken(ht_search(mapping_table,"NE")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else
				{
					forwardPointer--;
					return createToken(-1,backPointer,forwardPointer,LN);
				}
				break;
			}

			case 15:
			{
				//printf("%c\n",character_read);
				if(character_read=='_' ||('a'<=character_read && character_read<='z') || ('A'<=character_read && character_read<='Z')|| ('0'<= character_read && character_read<='9'))
				{
					state=15;
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"ID")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 24:
			{
				if(character_read=='*')
				{
					state=26;
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"MUL")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}	
			case 26:
			{
				if(character_read=='*')
				{
					state=27;
				}
				else
				{
					if(character_read=='\n')
						LN++;
					state=26;
				}
				break;
			}
			case 27:
			{
				if(character_read=='*')
				{
					state=1;
				}
				else
				{
					if(character_read=='\n')
						LN++;
					state=26;
				}

				break;
			}

			case 2:
			{
				if('0'<= character_read && character_read<='9')
				{
					state=2;
				}
				else if(character_read=='.')
				{
					state=4;
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"NUM")->data->t_item->index,backPointer,forwardPointer,LN);		
				}
				break;
			}
			case 4:
			{
				if(character_read=='.')
				{
					forwardPointer-=2;
					return createToken(ht_search(mapping_table,"NUM")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				else if('0'<= character_read && character_read<='9')
				{
					state=6;
				}		
				else
				{
					forwardPointer-=2;
					return createToken(ht_search(mapping_table,"NUM")->data->t_item->index,backPointer,forwardPointer,LN);
				}		
				break;
			}
			case 6:
			{
				if('0'<= character_read && character_read<='9')
				{
					state=6;
				}
				else if(character_read=='e' || character_read=='E')
				{
					state=8;
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"RNUM")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 8:
			{
				if(character_read=='+' || character_read=='-')
				{
					state=10;
				}
				else if('0'<= character_read && character_read<='9')
				{
					state=11;
				}
				else
				{
					forwardPointer-=2;
					return createToken(ht_search(mapping_table,"RNUM")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;
			}
			case 10:
			{
				if('0'<= character_read && character_read<='9')
				{
					state=11;
				}
				else
				{
					forwardPointer-=3;
					return createToken(ht_search(mapping_table,"RNUM")->data->t_item->index,backPointer,forwardPointer,LN);
				}
				break;			
			}

			case 11:
			{
				if('0'<= character_read && character_read<='9')
				{
					state=11;
				}
				else
				{
					forwardPointer--;
					return createToken(ht_search(mapping_table,"RNUM")->data->t_item->index,backPointer,forwardPointer,LN);
				}			
				break;
			}
		}
	}	
}


void removeComments(char* testcaseFile, char* cleanFile)
{
    FILE *sc = fopen(testcaseFile, "r");//char *src,char *dst
    FILE *cc = fopen(cleanFile, "w");
    char c,c2;
    int flag=0;
	while (!feof(sc)&&(c= fgetc(sc)))
	{
		if(c == EOF)
		{
			break;
		}
		if (c =='*')
		{
			if(feof(sc)){break;}
			c2= fgetc(sc);
			if (c2 =='*')//a comment starts
				flag=1;
			else
				{fputc(c,cc);fputc(c2,cc);}

			 while (flag==1)
			{
				if(feof(sc))
				{
					break;
				}
				c= fgetc(sc);
				if(c =='\n')
				{
					fputc(c,cc);
				}
				else if (c =='*')
				{
					if(feof(sc))
					{
						break;
					}
					c= fgetc(sc);
					if (c =='*')
						flag=0;
				}
			}	
		}
		else
			fputc(c,cc);
	}
	fclose(sc);
	fclose(cc);
}




