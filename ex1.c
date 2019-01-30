#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#define TRUE 1

//Constructing Stack as a structure

struct Node {
    int value;
    struct Node* next;
};

struct stack {
    struct Node* header;
    int size;
};

//Creating methods for stack

struct stack* stack_create(){
    struct stack* new_stack = (struct stack*)malloc(sizeof(struct stack*));
    new_stack->header = NULL;
    new_stack->size = 0;
    return new_stack;
}

int pop(struct stack* Stack){
    if (Stack == NULL) {
        printf("Stack was not created\n");
        return 1;
    }
    struct Node* current = Stack->header;
    if (current != NULL) {
        struct Node* last;
        if (current->next != NULL) {
            last = current->next;
        }
        else last = current;
        while (last->next != NULL) {
            current = current->next;
            last = last->next;
        }
        printf("Value %d was popped from stack\n", last->value);
        free(last);
        Stack->size--;
        current->next = NULL;
        return 0;
    }
    else return 1;
}

int peek(struct stack* Stack){
    if (Stack == NULL) {
        printf("Stack was not created\n");
        return 0;
    }
    if (Stack->header != NULL)
        printf("%d ", Stack->header->value);
    else printf("Stack is empty\n");
    return 0;
}

int push(struct stack* Stack, int value){
    if (Stack == NULL) {
        printf("Stack was not created\n");
        return 0;
    }
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->value = value;
    new_node->next = NULL;
    struct Node* last = Stack->header;
    if (last == NULL) Stack->header = new_node;
    else {
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_node;
    }
    Stack->size++;
    printf("Value %d was successfully pushed\n", value);
    return 0;

}

int empty(struct stack* Stack){
    if (Stack == NULL) {
        printf("Stack was not created\n");
        return 1;
    }
    if (Stack->size == 0) printf("Stack is empty\n");
    else printf("Stack is not empty\n");
    return 0;
}

int display(struct stack* Stack){
    if (Stack == NULL) {
        printf("Stack was not created\n");
        return 1;
    }
    struct Node* current = Stack->header;
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
    return 0;
}

//Parsing the command into acceptable one to produce

int parse(char* input, char* command, char* value) {
    int i, j = 0;
    while (input[i] != '\n' || input[i] != ' ') {
        command[i] = input[i];
        i++;
    }
    if (i < strlen(input)) {
        j = i;
        while (i < strlen(input)) {
            if (isdigit(input[i])) {
                value[i - j] = input[i];
            }
            j++;
            i++;
        }
    }
}


int main() {
    int par_id = getpid();
    pid_t pid = fork();

    int fds[2];
    char buffer[1024];
    char* string;
    if (pipe(fds) == -1) {
        printf("Pipe failed\n");
        return 1;
    }


    if (pid == 0) {
//        child process
        printf("Hello from Child\n");
        struct stack* Stack;
        while(TRUE) {
            wait(1);
            printf("Hello after little pause\n");
            char* input = malloc(sizeof(char)*100), command = malloc(sizeof(char)*100), value = malloc(sizeof(char)*100);
            read(fds, input, 100);
            printf("Smth\n");
            printf("%s\n", input);
            parse(input, command, value);
            if(strcmp(command, "peek") == 0)
                peek(Stack);
            else if(strcmp(command, "push") == 0)
                push(Stack, atoi(value));
            else if(strcmp(command, "pop") == 0)
                pop(Stack);
            else if(strcmp(command, "empty") == 0)
                empty(Stack);
            else if(strcmp(command, "display") == 0)
                display(Stack);
            else if(strcmp(command, "create") == 0)
                Stack = stack_create();
            else if(strcmp(command, "stack_size") == 0)
                Stack->size;
            else if(strcmp(command, "exit") == 0)
                break;
            else printf("Undefined command, try another one!\n");
            free(input);
            free(command);
            free(value);
        }

    }
    else if (pid < 0) return -1;
    else {
//        parent process
        printf("Hello from Parent\n");
        char command[100]; //string to get command
        while(TRUE) {
            printf("Enter command: peek; push int; pop; empty; display; create; stack_size or exit (to exit the program)\n");
            scanf("%s", command);
            printf("One more hello\n");
            write(fds, command, 100);

            wait(1);
            printf("Hello after wait\n");
        }
    }

    return 0;
}

