#include <cassert>
#include <cstddef>
#include <string>
#include <iostream>

/* Linux slists:
 *
 * A traditional linked list (of <int, float> pair) might look like:
 * ------------------------------------------------------------
 *  struct myStruct{     +--> struct myStruct{
 *    int data1;         |      int data1;
 *    float data2;       |      float data2;
 *    myStruct *next; --+       myStruct *next;
 *  };                        };
 * ------------------------------------------------------------
 *
 * linux/list.h defines linked lists similar to the following
 * (although here we define *singly* linked lists):
 *
 * ------------------------------------------------------------
 *  node 1                    node 2                    node 3
 *
 *  struct myStruct{          struct myStruct{          struct myStruct{
 *    int data1;                int data1;                int data1;
 *    float data2;              float data2;              float data2;
 *    slist_node node{      +-> slist_node node{      +-> slist_node node{
 *      slist_node *next; --+     slist_node *next; --+     slist_node *next;
 *    }                         }                         }
 *  }                         }                         }
 * ------------------------------------------------------------
 * 
 * To make t*he list generic in what data it carries (ie to decouple the list
 * structure from the data), instead of pointing to the next myStruct, we point
 * to the next slist_node.
 *
 * A library can provide the linked list structure (the definition of slist_node)
 * and functions for working with lists (eg append, reverse, map, fold)
 * in a way that is completley agnostic to what data is attached to each node.
 *
 * How do you know what data is attached to a given slist node?
 * Becuase the library is *generic* in the list data (in the sense of
 * parametricity), actually, the library doesn't know!
 * Only the user knows, since their data is some user-defined myStruct,
 * and they know what offset into myStruct the slist_node is at, which can be calculated with stddef.h's `offsetof`.
 *
 * So starting from a given slist_node *, if we know what myStruct its embeded
 * in, we can subtract the offset of slist_node in the myStruct from the
 * slist_node *, to get back the myStruct *.
 *
 * See the below for this idea worked out!
 */

struct slist_node{
    slist_node *next;
};

/* In-place append
 */
void slist_append(
        slist_node *&head, // NOTE: this is by reference
        slist_node *node
        ){
    if(head){
        slist_append(head->next, node);
    }
    else{
        head = node;
    }
}
/* map with additional argument threaded through
 */
void* slist_fold(
        slist_node *head, // NOTE: this is by value
        void (*f)(slist_node *, void *),
        void *user
        ){
    if(head){
        f(head, user);
        return slist_fold(head->next, f, user);
    }
    else{
        return user;
    }
}
void slist_print(
        slist_node *head, // NOTE: this is by value
        void (*print_elem)(slist_node *)
        ){
    std::cout << "[";
    slist_fold(head,
            [](slist_node *node, void *user){
                std::cout << " ";
                ((void (*)(slist_node *))user)(node);
            },
            (void *)print_elem
            );
    std::cout << "]" << std::endl;;
}

/* Returns true iff a swap happened
 */
bool slist_sort_helper(
        slist_node *&head,
        bool (*le)(slist_node *, slist_node *)
        ){
    if(head && head->next){ // if size >= 2
        if(le(head, head->next)){
            return slist_sort_helper(head->next, le);
        }
        else{
            slist_node *next = head->next; // save pointer to next node
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
/* In-place bubble sort
 */
void slist_sort(
        slist_node *&head, // NOTE: this is by reference
        bool (*le)(slist_node *, slist_node *)
        ){
    while(slist_sort_helper(head, le)){
        /* 🙏 */
    }
}

struct int_list{
    int elem;
    slist_node node;
};
int &int_list_elem(slist_node *node){
    int &elem =
        ((int_list *)((char *)node - offsetof(int_list, node)))->elem;
    return elem;
}
bool le_int(slist_node *left, slist_node *right){
    int &left_elem = int_list_elem(left);
    int &right_elem = int_list_elem(right);
    return left_elem <= right_elem;
}
bool g_int(slist_node *left, slist_node *right){
    return !le_int(left, right);
}
void int_list_print(slist_node *current){
    int &elem = int_list_elem(current);
    std::cout << elem;
}

int main(int argc, char **argv){
    /* The cast to void `(void)` silences a spurious unused variable warning
     *
     * This use of C's comma operator is one way to include a message in
     * assertions
     */
    assert(((void) "Program expects exactly 1 argument", argc == 2));

    slist_node *bar = &(new int_list{5, {nullptr}})->node;
    slist_append(bar, &((new int_list{7, {nullptr}})->node));
    slist_append(bar, &((new int_list{3, {nullptr}})->node));
    slist_append(bar, &((new int_list{4, {nullptr}})->node));
    slist_append(bar, &((new int_list{9, {nullptr}})->node));

    slist_print(bar, int_list_print);
    if(argv[1] == std::string("ascending")){
        slist_sort(bar, le_int);
    }
    else if(argv[1] == std::string("descending")){
        slist_sort(bar, g_int);
    }
    else{
        assert(((void) "Argument invalid", false));
    }
    slist_print(bar, int_list_print);
    slist_fold(
            bar,
            [](slist_node *node, void *user){
                int *accumulator = (int *)user;
                int &elem = int_list_elem(node);
                *accumulator += elem;
                elem = *accumulator;
            },
#if BUG
            0);
#else
            new int{0});
#endif
    slist_print(bar, int_list_print);
    return 0;
}
