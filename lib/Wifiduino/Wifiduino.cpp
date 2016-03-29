#include <stdlib.h>
#include <string.h>
#include "Wifiduino.h"

//////////////////////////////
//Wifiduino itself
//////////////////////////////

VarDict* createVarDict();
VarNode* findVarNode(char*);
VarNode* appendVarNode(char*, void*, int);
void readMessage();
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

//[M,N,T,V]
int readingPosition = 0;
char* msgNum;
char* name;
char* type;
char* value;
void readMessage() {
    char ch;
    while(Serial.available()) {
        ch = Serial.read();
        Serial.print("Character: ");
        Serial.println(ch);
        Serial.print("Message: [");
        Serial.print(msgNum);
        Serial.print(", ");
        Serial.print(name);
        Serial.print(", ");
        Serial.print(type);
        Serial.print(", ");
        Serial.print(value);
        Serial.print("]");
        Serial.print("readingPosition: ");
        Serial.println(readingPosition);
        switch(readingPosition) {
            case 0:
                if (ch == '[') {
                    readingPosition++;
                    msgNum = (char*) calloc(1, sizeof(char));
                }
                break;
            case 1:
                msgNum = (char *) realloc(msgNum, sizeof(char) * 2 + sizeof(msgNum));
                if (ch == ',') {
                    readingPosition++;
                    strncat(msgNum, "\0", 1);
                    name = (char*) calloc(1, sizeof(char));
                } else {
                    strncat(msgNum, &ch, 1);
                }
                break;
            case 2:
                name = (char *) realloc(name, sizeof(char) * 2 + sizeof(name));
                if (ch == ',') {
                    readingPosition++;
                    strncat(name, "\0", 1);
                    type = (char*) calloc(1, sizeof(char));
                } else {
                    strncat(name, &ch, 1);
                }
                break;
            case 3:
                type = (char *) realloc(type, sizeof(char) * 2 + sizeof(type));
                if (ch == ',') {
                    readingPosition++;
                    strncat(type, "\0", 1);
                    value = (char*) calloc(1, sizeof(char));
                } else {
                    strncat(type, &ch, 1);
                }
                break;
            case 4:
                value = (char *) realloc(value, sizeof(char) + sizeof(value));
                if (ch == ']') {
                    readingPosition++;
                    strncat(value, "\0", 1);
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
