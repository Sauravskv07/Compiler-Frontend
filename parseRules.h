/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/
# define MAX_RULES 201

typedef struct rule_rhs{
    ht_item *node;
    struct rule_rhs *next;
} rule_rhs;

typedef struct {
    ht_item *lhs;
    rule_rhs* key;
} rules_table;

rules_table rules[MAX_RULES];
int rule_count;

int parse(char *fileName);
int addLine(char *line, int index);
void printRules();
void printRule(int i);
