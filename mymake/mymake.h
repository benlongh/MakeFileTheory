//
//  mymake.h
//  assg9
//
//  Created by Benlong Huang on 4/5/16.
//  Copyright (c) 2016 Benlong Huang. All rights reserved.
//

#ifndef __assg9__mymake__
#define __assg9__mymake__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

// all the struct is in here


struct node;

typedef struct edge{
    struct node *to;
    struct edge *next;
} edge;


struct node {
    int isTarget;
    int flag;
    char *val;
    char *command;
    struct edge *toedge;
    struct node *next;
};


struct node *find(char *word);
void readedge(FILE *FP);
void DoIt(struct node *target);
void postOrder(struct node *target);

#endif /* defined(__assg9__mymake__) */
