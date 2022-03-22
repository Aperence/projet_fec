#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef struct stack{
    struct node *next;
    char *name;
    int size;
}stack_t;

/**
* Remove the top element of the stack and return its content.
*
* @head : pointer to the top of the stack
* @result : pointer to store the popped element
*
* @return 0 if no error, 1 otherwise
*
* pre : @result contains a null-terminated correct string
* post : @result contains the string @name from the element at the top of the stack
*/

int pop(stack_t **head, char *result){
    if (head==NULL || *head==NULL) return 1;
    stack_t *n = *head;
    if (result == NULL){
        return 1;
    }
    strcpy(result, n->name);
    n->size--;
    *head = n->next;
    free(n->name);
    free(n);
    return 0;
}

/**
* Add @name at the "top" of the stack.
*
* @head : pointer to the top of the stack
* @name : the string to be placed in the element at the top of the stack
*
* @return 0 if no error, 1 otherwise
*/

int push(stack_t **head, const char *value){
    if (head==NULL) return 1;
    stack_t *n = malloc(sizeof(stack_t));
    if (n==NULL) return 1;
    char *cp = malloc((strlen(value)+1)*sizeof(char));
    if (cp==NULL) return 1;
    strcpy(cp, value);
    n->name = cp;
    if (*head != NULL){
        n->size++;
        n->next = *head;
    }
    *head = n;
    return 0;
}












