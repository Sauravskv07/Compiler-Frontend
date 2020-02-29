/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/
typedef struct ht_items_list{
    ht_item *node;
    struct ht_items_list *next;
} ht_items_list;

ht_items_list * computeFirst(ht_item *term);
ht_items_list * computeFollow(ht_item *term);
ht_items_list * computeFirstofRule(rule_rhs* rhs);
void printmyList(ht_items_list * lt);
