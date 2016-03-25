//#include "Arduino.h"
#include "Wifiduino.h"
#include <stdlib.h>
#include <string.h>

//////////////////////////////
//Wifiduino itself
//////////////////////////////
VarDict* createVarDict();
FunctDict* createFunctDict();

Wifiduino* createWifiduino() {
    Wifiduino* wifiduino = malloc(sizeof(Wifiduino));
    wifiduino->varDict = createVarDict();
    wifiduino->functDict = createFunctDict();
    return wifiduino;
}

//////////////////////////////
//for var access and storage
//////////////////////////////
VarNode* findVarNode(VarDict*, const char[]);
VarNode* appendVarNode(VarDict*, const char[]);
void deleteVarNode(VarDict*, const char[]);

VarDict* createVarDict() {
    VarDict* varDict = malloc(sizeof(VarDict));
    varDict->findVarNode = findVarNode;
    varDict->appendVarNode = appendVarNode;
    varDict->deleteVarNode = deleteVarNode;
    varDict->head = NULL;
    return varDict;
}

VarNode* findVarNode(VarDict* varDict, const char name[]) {
    VarNode* ptr = varDict->head;
    while (ptr) {
        if (!strcmp(ptr->name, name)) { //if string literals are equal
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

VarNode* appendVarNode(VarDict* varDict, const char name[]) {
    VarNode* ptr = varDict->head;
    if (!ptr) {
        VarNode* varNode = malloc(sizeof(VarNode));
        varNode->name = name;
        varNode->next = NULL;
        varDict->head = varNode;
        return varNode;
    }
    while (ptr->next) {
        if (!strcmp(ptr->name, name)) { //if string literals are equal
            return ptr;
        }
    }
    VarNode* varNode = malloc(sizeof(VarNode));
    varNode->name = name;
    varNode->next = NULL;
    ptr->next = varNode;
    return varNode;
}

void deleteVarNode(VarDict* varDict, const char name[]) {
    VarNode* prev = varDict->head;
    VarNode* ptr = NULL;
    if (!strcmp(ptr->name, name)) {
        varDict->head = varDict->head->next;
        free(prev);
        return;
    }
    while (prev->next) {
        ptr = prev->next;
        if (!strcmp(ptr->name, name)) { //if string literals are equal
            prev->next = ptr->next;
            free(ptr);
            return;
        }
        prev = prev->next;
    }
}

//////////////////////////////
//for funct access and storage
//////////////////////////////
FunctDict* createFunctDict() {
    FunctDict* functDict = malloc(sizeof(FunctDict));
    functDict->head = NULL;
    return functDict;
}