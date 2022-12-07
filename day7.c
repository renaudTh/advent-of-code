#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_SPACE 70000000
#define NEEDED_SPACE 30000000
#define MAX_SIZE 10000

typedef unsigned int uint16_t;

typedef enum CommandCode_t {
    GO_FOLDER,
    GO_PARENT,
    MKDIR,
    IS_FILE,
    ERROR,

} CommandCode;

typedef struct Label_t{

    char * name;
    uint16_t size;

} Label;

Label * label_new(const char * name, uint16_t size){
    Label * l = malloc(sizeof(Label));
    l->name = malloc(sizeof(char)*size);
    memset(l->name,0,size);
    l->size = size;
    for(int i = 0; i < size; i++){
        l->name[i] = name[i];
    }
    return l;
}
int label_equals(const Label * l1,  const Label * l2){
    if(l1->size != l2->size) return 0; 
    return (strncmp(l1->name, l2->name, l1->size) == 0);
}

void label_destroy(Label *label){
    if(label->name) free(label->name);
    label->name = NULL;
    free(label);
}

typedef struct Node_t {

    uint16_t data;
    Label * label;
    struct Node_t * firstChild;
    struct Node_t * nextSibling;
    struct Node_t * parent;

} Node;

Node * node_new(uint16_t data, Label * label){

    Node *n = malloc(sizeof(Node));
    memset(n, 0, sizeof(Node));
    n->data = data;
    n->label = label_new(label->name, label->size);
    n->firstChild = NULL;
    n->nextSibling = NULL;
    return n;
}

void node_append_child(Node * node, Node *child){
    if(node == NULL) return;
    Node * current = node;
    Node * next = NULL;

    child->parent = node;
    if(current->firstChild == NULL){
        current->firstChild = child;
        return;
    }
    current = node->firstChild;
    if(current->nextSibling == NULL){
        current->nextSibling = child;
        return;
    }
    next = current->nextSibling;
    while(next != NULL){
        current = next;
        next = next->nextSibling;
    }
    current->nextSibling = child;
}

Node * node_find_child_label(const Node *start, const Label * label){
    if(!start) return NULL;
    if(!start->firstChild) return NULL;
    Node * current = start->firstChild;
    while(current != NULL && !label_equals(current->label, label)){
        current = current->nextSibling;
    }
    return current;
}

void node_add_data(Node * node, uint16_t data){
    if(!node) return;
    node->data+=data;
}

void node_free(Node *node){
    if(node->label) label_destroy(node->label);
    node->label = NULL;
    free(node);
    node = NULL;
}

void node_free_recursive(Node * root){
    
    if (root->firstChild != NULL) {
        node_free_recursive(root->firstChild);
    }
    if (root->nextSibling != NULL) {
        node_free_recursive(root->nextSibling);
    }
    node_free(root);
}
void get_all_data(Node * root, uint16_t * data, uint16_t *size){

    if(root != NULL){

        get_all_data(root->firstChild, data, size);
        data[*size] = root->data;
        *size = *size + 1;
        get_all_data(root->nextSibling, data, size);
    }
}

int is_digit(const char c){
    return (c >= '0' && c <= '9'); 
}

CommandCode parseCommand(char * line, size_t lineLenght, Label ** label, uint16_t * size){
    if(line == NULL) return ERROR;
    *size = 0;
    if(strncmp(&line[0], "$ cd", 4) == 0){

        if(strncmp(&line[5], "..",2) == 0){
            return GO_PARENT;
        }
        else{
            (*label) = label_new(&line[5], lineLenght - 6);
            return GO_FOLDER; 
        }  
    }
    else if(line[0] == 'd'){
        (*label) = label_new(&line[4], lineLenght - 5);
        return MKDIR;
    }
    else if(is_digit(line[0])){
        int i = 0;
        while(i < lineLenght && is_digit(line[i])){
            *size = *size*10;
            *size += line[i] - '0';
            i++;
        }
        return IS_FILE;
    }
    return ERROR;
}

void go_folder(Node ** current, const Label * label){
#ifdef LOG
    printf("$ cd %s\n", label->name);
#endif

    if(strncmp(label->name, "/", 1) != 0)               
        *current = node_find_child_label(*current, label); 

}

void go_parent(Node ** current){
#ifdef LOG
    printf("$ cd ..\n");
#endif
    if((*current)->parent){
        *current = (*current)->parent;
    }
}

void mkdir(Node ** current, Label * label){
#ifdef LOG
    printf("dir %s\n", label->name);
#endif
    Node * child = node_new(0, label);
    node_append_child(*current, child);
}

void add_file(Node ** current, uint16_t size){
#ifdef LOG
    printf("%u file\n", size);
#endif
    if(*current == NULL) return;
    Node *prev = NULL;
    node_add_data(*current, size);
    prev = (*current)->parent;
    while(prev != NULL){
        node_add_data(prev, size);
        prev = prev->parent;
    }
}

int main(){

    char * line = NULL;
    size_t lineLength = 0;
    size_t read = 0;
    FILE *file = fopen("tests.txt", "r");
        if (!file)
            return EXIT_FAILURE;

    Label * rootLabel = label_new("/", 1);
    Node * root = node_new(0, rootLabel);
    Node * current = root;
    Label * label = NULL;
    uint16_t size = 0;

    while ((read = getline(&line, &lineLength, file)) != -1) {
        
        CommandCode code = parseCommand(line, read, &label, &size);
        
        switch(code){
            case GO_FOLDER: go_folder(&current, label);
            break;
            case GO_PARENT: go_parent(&current);
            break;
            case MKDIR: mkdir(&current, label);
            break;
            case IS_FILE: add_file(&current, size);
            break;
            default:
            break;  
        }
        if(label != NULL){
            label_destroy(label);
            label = NULL;
        }
        size = 0;
    }
    if (line)
        free(line);
    fclose(file);

    uint16_t data_size = 0;
    uint16_t * data = malloc(sizeof(uint16_t) * MAX_SIZE);
    get_all_data(root, data, &data_size);
    uint16_t used = data[data_size - 1];
    uint16_t amout_to_delete = 0;
    uint16_t under_100000 = 0;

    for(int i = 0; i < data_size; i++){
        if(data[i] <= 100000){
            under_100000+=data[i];
        }
        if(TOTAL_SPACE - used + data[i] >= NEEDED_SPACE){
            amout_to_delete = (amout_to_delete == 0) ? data[i] : amout_to_delete;
        }
    }
    printf("The first two stars answer is %u\n", under_100000);
    printf("The last two stars answer is %u\n", amout_to_delete);

    node_free_recursive(root);
    free(data);
    label_destroy(rootLabel);
    
    return 0;
}