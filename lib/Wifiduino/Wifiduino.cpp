#include <stdlib.h>
#include <string.h>
#include "Wifiduino.h"

//////////////////////////////
//Wifiduino itself
//////////////////////////////

VarDict* createVarDict();
VarNode* findVarNode(char*);
VarNode* appendVarNode(char*, void*, int);
void deleteVarNode(char*);
//FunctDict* createFunctDict();

Wifiduino wifi;

void createWifiduino() {
    wifi = *((Wifiduino *) malloc(sizeof(Wifiduino)));
    wifi.varDict = createVarDict();
    wifi.variable = wifi.varDict->appendVarNode;
    //wifiduino->functDict = createFunctDict();
}
//createWifiduino();

//////////////////////////////
//for var access and storage
//////////////////////////////
VarDict* createVarDict() {
    VarDict* varDict = (VarDict *) malloc(sizeof(VarDict));
    varDict->findVarNode = findVarNode;
    varDict->appendVarNode = appendVarNode;
    varDict->deleteVarNode = deleteVarNode;
    varDict->head = NULL;
    return varDict;
}

//<m:></m>
void readMessage(char* msgNum, char* varName) {
    int position = 0;
    char ch;
    while(0){//Serial.available()) {
        //ch = Serial.read();
        switch(ch) {
            case 0:
                position = (ch == '<' ? position + 1 : 0);
                break;
            case 1:
                position = (ch == 'm' ? position + 1 : 0);
                break;
            case 2:
                position = (ch == ':' ? position + 1 : 0);
                break;
            case 3:
                if (ch == ':') { //read message num
                    position++;
                    msgNum = (char *) realloc(msgNum, sizeof(msgNum) + 1);
                    strncat(msgNum, &ch, 1);
                }
                break;
            case 4:
                if (ch == '>') {
                    position++;
                } else {
                    free(msgNum);
                    msgNum = NULL;
                    position = 0;
                }
                break;
            case 5:
                if (ch == ':') { //read message num
                    position++;
                    msgNum = (char *) realloc(msgNum, sizeof(msgNum) + 1);
                    strncat(msgNum, &ch, 1);
                }
            case 6:
                position = (ch == '/' ? position + 1 : 0);
                break;
        }
    }
    //if (msgNum && )
}

void writeMessage(char* msgNum, char* msgType, char* value) {
    char* msg = (char*) malloc(((int)strlen(msgNum) + 1 + (int)strlen(value) + 9) * sizeof(char)); //whatever the size of msgNum is (max 6) + 1 for msgtype + whatever the length of value is + 9 for "<m:></m>"
    strcat(msg, "<m:");
    strcat(msg, msgNum);
    strcat(msg, ">");
    strcat(msg, value);
    strcat(msg, "</m>");
    //Serial.write(msg);
}

VarNode* findVarNode(char* name) {
    VarDict* varDict = wifi.varDict;
    VarNode* ptr = varDict->head;
    while (ptr) {
        if (!strcmp(ptr->name, name)) { //if string literals are equal
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

VarNode* appendVarNode(char* name, void* location, int varType) {
    VarDict* varDict = wifi.varDict;
    VarNode* ptr = varDict->head;

    VarNode* varNode = (VarNode *) malloc(sizeof(VarNode));
    varNode->name = name;
    varNode->location = location;
    varNode->varType = varType;
    varNode->next = NULL;

    if (!ptr) {
        varDict->head = varNode;
        return varNode;
    }
    while (ptr->next) {
        if (!strcmp(ptr->name, name)) { //if string literals are equal
            return ptr;
        }
    }
    ptr->next = varNode;
    return varNode;
}

void deleteVarNode(char* name) {
    VarDict* varDict = wifi.varDict;
    VarNode* prev = varDict->head;
    VarNode* ptr = NULL;
    if (!strcmp(ptr->name, name)) {
        varDict->head = varDict->head->next;
        free(prev->name);
        free(prev);
        return;
    }
    while (prev->next) {
        ptr = prev->next;
        if (!strcmp(ptr->name, name)) { //if string literals are equal
            prev->next = ptr->next;
            free(ptr->name);
            free(ptr);
            return;
        }
        prev = prev->next;
    }
}


/*
//////////////////////////////
//for funct access and storage
//////////////////////////////
FunctDict* createFunctDict() {
    FunctDict* functDict = malloc(sizeof(FunctDict));
    functDict->head = NULL;
    return functDict;
}*/
