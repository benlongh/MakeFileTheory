/*
 * File: mymake.c
 * Author: Benlong Huang
 * Purpose: This problem involves writing a C program that implements part the core functionality of the make utility. This assignment involves reading in a file specifying dependencies and rules as in make (though with a different and more explicit syntax); constructing a dependency tree; and then traversing the tree starting with some specified target. The extension to actually check timestamps and execute the associated commands where necessary will be part of a future assignment. */

#include "mymake.h"
struct node *list = NULL;
char *name = NULL;

int freememory(){
    struct node *st0, *st1;
    st0 = list;
    while(st0 != NULL){
        edge *edg0, *edg1;
        st1 = st0->next;
        edg0 = st0->toedge;
        while(edg0 != NULL){
            edg1 = edg0->next;
            free(edg0);
            edg0 = edg1;
        }
        free(st0->val);
        free(st0);
        st0 = st1;
    }
    return 0;
}

void error(FILE *fp, char *name){
    if(fp == NULL){
        perror(name);
        exit(1);
    }
}

// the same as check below, check whether the word is already in the list.
struct node *find(char *word){
    struct node *temp = list;
    while(temp!= NULL){
        if(strcmp(temp->val,word) == 0 ){
            break;
        }
        temp = temp->next;
    }
    return temp;
}

// use to check whether the word is already in the list.
int check(char *word){
    struct node *temp = list;
    while(temp!= NULL){
        if(strcmp(temp->val,word) == 0 ){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void readnode(char word[]){
    struct node *tempnode;
    tempnode = malloc(sizeof(struct node));
    if(check(word) == 0){
        tempnode->val = strdup(word);
        tempnode->flag = 0;
        tempnode->toedge = NULL;
        tempnode->next = list;
        list = tempnode;
    }
    
}
// this method put the all the cmd into relevant target.
void putcmd(char *targetname, char *cmdname){
    struct node *temp = list;
    while(strcmp(targetname, temp->val) != 0){
        temp = temp->next;
    }
    temp->command = strdup(cmdname);
}
//this part just print the content in list
/*void printList(){
    struct node *temp = NULL;
    
    temp = list;
    while (temp != NULL){
        printf("%s\n", temp->val);
        temp = temp->next;
    }
}*/

void checkFile(FILE *FP){
    char word[65];  char *temp = strdup(""); size_t size = 1; char *targetname = NULL;
    int error2 = 0; int index = -1; int iscmd = 0;
    // scan the first word in the file which should be @target.
    while (fscanf(FP, "%s", word) != EOF){
        index++;
        // if it's target line.
    if(iscmd == 0){
        if(index == 0){
            
            if(strcmp(word, "@target") == 0){
                
                continue;
            }
            else{
                fprintf(stderr, "Miss @target\n");
                freememory();
                exit(1);
            }
        }
        // if it's target
        if(index == 1){
            // copy the target name into targetname.
            targetname = strdup(word);
            readnode(word);
            // if the target start with @.
            if(word[0] == '@'){
                fprintf(stderr, "Bad Word\n");
                freememory();
                exit(1);
            }
        }
        // if it's ":"
        if(index == 2){
            if(strcmp(word,":") == 0){
                
                continue;
            }
            else{
                fprintf(stderr, "missing :\n");
                freememory();
                exit(1);
            }
        }
        // if it's dependency
        if(index > 2){
            // if the dependency start with @ and is not @cmd.
            if(word[0] == '@' && strcmp(word,"@cmd") != 0){
                fprintf(stderr, "Illegal Word\n");
                freememory();
                exit(1);
            }
            if(strcmp(word,"@cmd") == 0){
                index = 0;
                
                iscmd = 1;
                continue;
            }
            readnode(word);
        }
    }
        // if it's command line
        if(iscmd == 1){
            // if the first argument after @cmd is @target
            if(index == 1 && strcmp(word,"@target") == 0){
                fprintf(stderr, "missing command\n");
                freememory();
                exit(1);
            }
            // if it's @target
            if(strcmp(word,"@target") == 0){
                index = 0;
                iscmd = 0;
                putcmd(targetname, temp);
                temp = NULL;
                continue;
            }
            // if it's normal command.
            else{
                // if it's bad word like there is a @ infront of the word.
                if(word[0] == '@' && strcmp(word,"@target") != 0){
                    fprintf(stderr, "Bad Word\n");
                    freememory();
                    exit(1);
                }
                // if it's the common command. gcc -Wall mymake
                else{
                    // connect word
                    size = size + sizeof(word);
                    temp = realloc(temp, size);
                    temp = strcat(temp, word);
                    // connect with space
                    size = size + sizeof(" ");
                    temp = realloc(temp, size);
                    temp = strcat(temp, " ");
                    
                    error2 = 1;
                    
                    continue;
                }
            }
        }
    }
    
    // if iscmd is not end with 1. then missing @cmd.
    if(iscmd == 0){
        fprintf(stderr, "missing @cmd\n");
        freememory();
        exit(1);
    }
    // if there has nothing after @cmd.
    if(error2 == 0){
        fprintf(stderr, "missing command\n");
        freememory();
        exit(1);
    }
    // do one more time to print out the last command, because it will jump out if there don't have @target anymore. 
    putcmd(targetname, temp);
}


// this method is to execute the command and judge whether it's already exist or not.
int checkExe(struct node *target){
    struct stat *tar = malloc(sizeof(struct stat));
    int status = stat(target->val, tar);
    if(status != 0){
        return 1;
    }
    long targetTime = tar->st_mtime;
    struct edge *temp = target->toedge;
    while(temp != NULL){
        // check each dependency file is newer than the target or not
        struct stat *dep = malloc(sizeof(struct stat));
        // compare the dependency name
        int statusD = stat(temp->to->val, dep);
        // if the return value is -1 which is not =0, then it means missing dependency.
        if(statusD != 0){
            fprintf(stderr, "Missing dependency: %s\n", temp->to->val);
            exit(1);
        }
        // or compare the denpendency time, if denpendency time is newer than target time, then do execute the command, set the return value be 1.
        long DependencyTime = dep->st_mtime;
        if(DependencyTime > targetTime){
            return 1;
        }
        
        temp = temp->next;
    }

    return 0;
}


void DoIt(struct node *target){
    if(target->isTarget == 1 &&  checkExe(target) == 1){
        printf("%s\n", target->command);
        system(target->command);
    }
    else
        return;
     
}


// This is the main method.
int main(int argc, char *argv[]) {
    FILE *FP;
    // if the argument is not equal to 3, then error. it has to be the form of mymake <input file> <target>.
    if (argc != 3) {
        fprintf(stderr, "Usage: ./mymake <input file> <target>\n");
        return 1;
    }
    
    name = argv[1];
    FP =fopen(name, "r");
    error(FP, name);
    checkFile(FP);
    rewind(FP);
    readedge(FP);
    postOrder(find(argv[2]));
    
#ifdef DEBUG
    struct node *temp = list;
    while(temp != NULL){
        if(temp->isTarget == 1 ){
            printf("%d ",temp->isTarget);
            printf("@@@%s\n",temp->val);
            printf("%s\n",temp->command);
        }
        temp = temp->next;
    }
#endif
    
    fclose(FP);
    freememory();
    return 0;
}

