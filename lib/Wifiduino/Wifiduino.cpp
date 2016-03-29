#include <stdlib.h>
#include <string.h>
#include "Wifiduino.h"

//////////////////////////////
//Wifiduino itself
//////////////////////////////

VarDict* createVarDict();
VarNode* findVarNode(char*);
VarNode* appendVarNode(char*, void*, int);
void writeMessage(VarNode*, char*);
void readMessage();
void deleteVarNode(char*);
//FunctDict* createFunctDict();

Wifiduino Wifi;

void createWifiduino() {
    Wifi = *((Wifiduino *) malloc(sizeof(Wifiduino)));
    Wifi.varDict = createVarDict();
    Wifi.variable = Wifi.varDict->appendVarNode;
    Wifi.readMessage = readMessage;
    Wifi.writeMessage = writeMessage;
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
        case FLOAT: {
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
int msgSize = 20;
char* msgNum;
char* name;
char* type;
char* value;
char* temp;
void readMessage() {
    char ch;
    while(Serial.available()) {
        ch = Serial.read();
        /*Serial.print("Character: ");
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
        Serial.print(", readingPosition: ");
        Serial.println(readingPosition);*/
        switch(readingPosition) {
            case 0:
                if (ch == '[') {
                    readingPosition++;
                    msgNum = (char*) calloc(msgSize, sizeof(char));
                }
                break;
            case 1:
                if (ch == ',') {
                    readingPosition++;
                    strncat(msgNum, "\0", 1);
                    name = (char*) calloc(msgSize, sizeof(char));
                } else {
                    strncat(msgNum, &ch, 1);
                }
                break;
            case 2:
                if (ch == ',') {
                    readingPosition++;
                    strncat(name, "\0", 1);
                    type = (char*) calloc(msgSize, sizeof(char));
                } else {
                    strncat(name, &ch, 1);
                }
                break;
            case 3:
                if (ch == ',') {
                    readingPosition++;
                    strncat(type, "\0", 1);
                    value = (char*) calloc(msgSize, sizeof(char));
                } else if (ch == ']'){
                    readingPosition = 0;
                    VarNode* nodeToRead = Wifi.varDict->findVarNode(name);
                    if (nodeToRead) {
                        writeMessage(nodeToRead, msgNum);
                    } else {
                        Serial.println("false");
                        free(value);
                    }
                    free(msgNum);
                    free(name);
                    free(type);
                } else {
                    strncat(type, &ch, 1);
                }
                break;
            case 4:
                if (ch == ']') {
                    strncat(value, "\0", 1);
                    readingPosition = 0;
                    VarNode* nodeToChange = Wifi.varDict->findVarNode(name);
                    if (nodeToChange) {
                        switch (nodeToChange->varType) {
                            case CHAR: {
                                char* temp = (char*) nodeToChange->location;
                                nodeToChange->location = value;
                                free(temp);
                                }break;
                            case INT:
                                *(int*) nodeToChange->location = (int)atoi(value);
                                free(value);
                                break;
                            case FLOAT:
                                *(float*) nodeToChange->location = (float)atof(value);
                                free(value);
                                break;
                        }
                        writeMessage(nodeToChange, msgNum);
                    } else {
                        Serial.println("false");
                        free(value);
                    }
                    free(msgNum);
                    free(name);
                    free(type);
                } else {
                    strncat(value, &ch, 1);
                }
                break;
        }
    }
}

void writeMessage(VarNode* varNode, char* msgNum) {
  char* stringValue = charray(varNode->location, varNode->varType);
  Serial.print("[");
  Serial.print(msgNum);
  Serial.print(", \"");
  Serial.print(varNode->name);
  Serial.print("\", ");
  Serial.print(stringValue);
  Serial.print(", ");
  Serial.print(varNode->varType);
  Serial.println("]");
  free(stringValue);
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
