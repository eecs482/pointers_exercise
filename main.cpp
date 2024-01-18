#include <cassert>
#include <cstring>
#include <iostream>

struct slist_node{
    void *elem;
    slist_node *next;
};

/* In-place append
 */
void slist_append(
        slist_node *&head, // NOTE: this is by reference
        void *elem
        ){
    if(head){
        slist_append(head->next, elem);
    }
    else{
        head = new slist_node{elem, nullptr};
    }
}

/* In-place bubble sort
 */
bool slist_sort_helper( // forward declare
        slist_node *&head,
        bool (*le)(void *, void *)
        );
void slist_sort(
        slist_node *&head, // NOTE: this is by reference
        bool (*le)(void *, void *)
        ){
    while(slist_sort_helper(head, le)){
        /* 🙏 */
    }
}
/* Returns true iff a swap happened
 */
bool slist_sort_helper(
        slist_node *&head,
        bool (*le)(void *, void *)
        ){
    if(head && head->next){ // if size >= 2
        assert(((void)"le comparator should be a total ordering", le(head->elem, head->next->elem) ||
                le(head->next->elem, head->elem)));
        if(le(head->elem, head->next->elem)){
            return slist_sort_helper(head->next, le);
        }
        else{
            slist_node *next = head->next; // save pointer to next node
            // swap head and head->next
            head->next = next->next;
            next->next = head;
            head = next; // return to caller with updated head
            return true;
        }
    }
    else{
        return false;
    }
}

void slist_map(
        slist_node *head, // NOTE: this is by value
        void (*f)(void *)
        ){
    if(head){
        f(head->elem);
        slist_map(head->next, f);
    }
}

void slist_print(
        slist_node *head, // NOTE: this is by value
        void (*print_elem)(void *)
        ){
    std::cout << "[";
    slist_map(head, print_elem);
    std::cout << "]" << std::endl;
}


int main(){
    /* construct a singly-linked list
     */
    slist_node *foo = new slist_node{new int{5}, nullptr};
    slist_append(foo, new int{7});
    slist_append(foo, new int{3});
    slist_append(foo, new int{4});
    slist_append(foo, new int{9});
    slist_append(foo, new int{5});

    /* TODO A: Print the list!
     * Should output the list [5, 7, 3, 4, 9, 5]
     * (although the formatting is up to you)
     */
    std::cout << "A) ";
    slist_print(foo, nullptr /* TODO A */);

    /* TODO B: Sort the list in ascending order!
     */
    slist_sort(foo,
            nullptr /* TODO B */);
    std::cout << "B) ";
    slist_print(foo, nullptr /* TODO A */);

    /* TODO C: Sort the list in descending order!
     */
    slist_sort(foo,
            nullptr /* TODO C */);
    std::cout << "C) ";
    slist_print(foo, nullptr /* TODO A */);

    /* TODO D: Increment every element by 1!
     */
    slist_map(foo,
            nullptr /* TODO D */);
    std::cout << "D) ";
    slist_print(foo, nullptr /* TODO A */);

    /* TODO E (Harder): Map the list to a list of sums!
     * For example, map the list [1, 2, 3, 4, 5] to [1, 3, 6, 10, 15]
     *
     * (For a hint, see the bottom of this file)
     */
    slist_map(foo,
            nullptr /* TODO E */);
    std::cout << "E) ";
    slist_print(foo, nullptr /* TODO A */);

    /* TODO F (Really hard): Find how many characters are in the list, by mapping
     * each string to its length, and applying TODO E!
     *
     * (Hint: Why is there an extra layer of void * indirection?)
     * (Hint 2: Mapping the list of strings to a list of ints requires extreme care.)
     */
    slist_node *bar = new slist_node{new void *{strdup("It")}, nullptr};
    slist_append(bar, new void *{strdup("may")});
    slist_append(bar, new void *{strdup("be")});
    slist_append(bar, new void *{strdup("faster")});
    slist_append(bar, new void *{strdup("to")});
    slist_append(bar, new void *{strdup("just")});
    slist_append(bar, new void *{strdup("count")});
    slist_append(bar, new void *{strdup("manually")});
    slist_append(bar, new void *{strdup("...")});

    // First, print out the dang thing
    slist_print(bar,
            nullptr /* TODO F */);
    // Then map it to a list of lengths
    slist_map(bar,
            nullptr /* TODO F */);
    // Then take the list of sums, from TODO E
    slist_map(bar,
            nullptr /* TODO E */);
    // And print out what we've got
    std::cout << "F) ";
    slist_print(bar,
            nullptr /* TODO F */);
    return 0;
}





















































/* Hint: Consider using a global variable or even better, a static function
 * variable.
 *
 * If you don't want to use either of those, adapt slist_map to a list fold.
 * See the variant of this exercise linked in the README for a solution with fold.
 */
