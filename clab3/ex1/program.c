#define _GNU_SOURCE
/**
 * \author Bert Lagaisse
 *
 * main method that executes some test functions (without check framework)
 */

#include <stdio.h>
#include "dplist.h"

void ck_assert_msg(bool result, char * msg){
    if(!result) printf("%s\n", msg);
}
int main(void)
{
    dplist_t *numbers = NULL;
    numbers = dpl_create();

    ck_assert_msg(numbers != NULL, "numbers = NULL, List not created");
    ck_assert_msg(dpl_size(numbers) == 0, "Numbers may not contain elements.");

    dpl_insert_at_index(numbers, 'a', 0);
    ck_assert_msg(dpl_size(numbers) == 1, "Numbers must contain 1 element.");

    dpl_insert_at_index(numbers, '\e', -1);
    ck_assert_msg(dpl_size(numbers) == 2, "Numbers must contain 2 elements.");

    dpl_insert_at_index(numbers, 'b', 100);
    ck_assert_msg(dpl_size(numbers) == 3, "Numbers must contain 3 elements.");

    ck_assert_msg(dpl_get_index_of_element(numbers, '\e') == 0, "Wrong index");
    ck_assert_msg(dpl_get_index_of_element(numbers, 'a') == 1, "Wrong index");
    ck_assert_msg(dpl_get_index_of_element(numbers, 'b') == 2, "Wrong index");

    dpl_insert_at_index(numbers, 'k', 100);
    dpl_remove_at_index(numbers, 2);
    ck_assert_msg(dpl_get_element_at_index(numbers, 2) == 'k', "Wrong index");

    dpl_remove_at_index(numbers, 2);
    ck_assert_msg(dpl_size(numbers) == 2, "Numbers must contain 2 elements.");
    //dpl_remove_at_index(numbers, 0);
    ck_assert_msg(dpl_size(numbers) == 1, "Numbers must contain 1 element.");


    dpl_free(&numbers);

    return 0;
}