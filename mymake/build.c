//
//  build.c
//  assg10
//
//  Created by Huang, Benlong - (benlongh) on 4/15/16.
//  Copyright © 2016 Huang, Benlong - (benlongh). All rights reserved.
//

#include "mymake.h"

void readedge(FILE *FP){
    char word[65] = "";
    int index = -1, iscmd = 0;
    struct node *target = NULL;
    
    edge *denp = NULL, *denp1 = NULL;
    
    // this is similar with the part I did above, but the errors have already been checked. so no error in this method. Just set the pointer on different place.
    while (fscanf(FP, "%s", word) != EOF){
        index++;
        if (iscmd == 0){
            if (index == 0)
                continue;
            
            if (index == 1){
                target = find(word);
               
                target->isTarget = 1;
            }
            
            if (index == 2)
                continue;
            
            if (strcmp(word, "@cmd") == 0){
                index = 0;
                iscmd = 1;
                continue;
            }
            
            if (target->toedge == NULL){
                denp = malloc(sizeof(edge));
                denp->to = find(word);
                denp->next = NULL;
                target->toedge = denp;
            }
            else{
                //denp = target->toedge;
                while (denp->next != NULL)
                    denp = denp->next;
                denp1 = malloc(sizeof(edge));
                denp1->to = find(word);
                denp1->next = NULL;
                denp->next = denp1;
                denp = denp1;
            }
        }
        // if the word scanned is @target then start next loop. let index and iscmd be 0 again.
        else{
            if (strcmp(word, "@target") == 0){
                index = 0;
                iscmd = 0;
                continue;
            }
        }
        
    }
}



// this method is make the word appear by the postorder form.
void postOrder(struct node *target){
    if (target == NULL)
        return;
    // if the flag = 1, means it's already done before.
    if (target->flag == 1)
        return;
    // or let the flag be 1.
    target->flag = 1;
    if (target->toedge == NULL){
        //printf("%s\n", target->val);
        return;
    }
    edge *temp = target->toedge;
    
    while (temp != NULL){
        postOrder(temp->to);
        temp = temp->next;
    }
    DoIt(target);
    //printf("%s\n", target->val);
    return;
}
