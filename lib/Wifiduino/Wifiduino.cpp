#include <stdlib.h>
#include <string.h>
#include "Wifiduino.h"

//////////////////////////////
//Wifiduino itself
//////////////////////////////

VarDict* createVarDict();
VarNode* findVarNode(char*);
VarNode* appendVarNode(char*, void*, int);
void readMessage(char*, char*, char*, char*);
void deleteVarNode(char*);
//FunctDict* createFunctDict();

Wifiduino Wifi;

void createWifiduino() {
    Wifi = *((Wifiduino *) malloc(sizeof(Wifiduino)));
    Wifi.varDict = createVarDict();
    Wifi.variable = Wifi.varDict->appendVarNode;
    Wifi.readMessage = readMessage;
    //Wifiduino->functDict = createFunctDict();
}

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

char* charray(void* value, int type) {
    char* returnArray = NULL;
    int size;
    switch(type) {
        case CHAR:
            returnArray = (char*) value;
            break;
        case INT:
            size = 8;
            returnArray = (char*) malloc(size);
            snprintf(returnArray, size, "%d", *(int*)value);
            break;
        case FLOAT:
        case DOUBLE: {
            size = 16;
            int d1 = *(float*)value;
            float f2 = *(float*)value - d1;
            int d2 = trunc(f2 * 10000);
            float f3 = f2 * 10000 - d2;
            int d3 = trunc(f3 * 10000);
            returnArray = (char*) malloc(size);
            snprintf (returnArray, size, "%d.%04d%04d", d1, d2, d3);
            }break;
    }
    return returnArray;
}

//<M:N:T:V>
int readingPosition = 0;
void readMessage(char* msgNum, char* name, char* type, char* value) {
    char ch;
    while(Serial.available()) {
        ch = Serial.read();
        switch(readingPosition) {
            case 0:
                readingPosition = (ch == '<' ? readingPosition + 1 : 0);
                break;
            case 1:
                if (ch == ':') { //read message num
                    readingPosition++;
                } else {
                    msgNum = (char *) realloc(msgNum, sizeof(msgNum) + 1);
                    strncat(msgNum, &ch, 1);
                }
                break;
            case 2:
                if (ch == ':') { //read message num
                    readingPosition++;
                } else {
                    name = (char *) realloc(name, sizeof(name) + 1);
                    strncat(name, &ch, 1);
                }
                break;
            case 3:
                if (ch == ':') { //read message num
                    readingPosition++;
                } else {
                    type = (char *) realloc(type, sizeof(type) + 1);
                    strncat(type, &ch, 1);
                }
                break;
            case 4:
                if (ch == '>') { //read message num
                    readingPosition = 0;
                    Serial.println("VVVV");
                    Serial.println(msgNum);
                    Serial.println(name);
                    Serial.println(type);
                    Serial.println(value);
                    Serial.println("^^^^");
                    free(msgNum);
                    free(name);
                    free(type);
                    free(value);
                } else {
                    value = (char *) realloc(value, sizeof(value) + 1);
                    strncat(value, &ch, 1);
                }
                break;
        }
    }
}

void writeMessage(char* msgNum, char* value) {
    char* msg = (char*) malloc(((int)strlen(msgNum) + 1 + (int)strlen(value) + 9) * sizeof(char)); //whatever the size of msgNum is (max 6) + 1 for msgtype + whatever the length of value is + 9 for "<m:></m>"
    strcat(msg, "<m:");
    strcat(msg, msgNum);
    strcat(msg, ">");
    strcat(msg, value);
    strcat(msg, "</m>");
    //Serial.write(msg);
}

VarNode* findVarNode(char* name) {
    VarDict* varDict = Wifi.varDict;
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
    VarDict* varDict = Wifi.varDict;
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
    VarDict* varDict = Wifi.varDict;
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
