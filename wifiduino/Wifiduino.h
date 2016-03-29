#ifndef Wifiduino_h
#define Wifiduino_h

//#include "Arduino.h"

enum returnType {CHAR, STRING, INT, FLOAT, DOUBLE};

typedef struct Wifiduino Wifiduino;
typedef struct VarDict VarDict;
//typedef struct FunctDict FunctDict;
typedef struct VarNode VarNode;
//typedef struct FunctNode FunctNode;

//////////////////////////////
//Wifiduino itself
//////////////////////////////
Wifiduino* createWifiduino();
typedef struct Wifiduino {
    void (*function)(char*, void*);
    void (*variable)(char*, void*);
    VarDict* varDict;
    //FunctDict* functDict;
} Wifiduino;

//////////////////////////////
//for var access and storage
//////////////////////////////
VarDict* createVarDict();

typedef struct VarDict {
    void (*sendValue)(char*, int);
    void (*storeVar)(char*, int, int);
    VarNode* (*findVarNode)(VarDict*, char*);
    VarNode* (*appendVarNode)(VarDict*, char*);
    void (*deleteVarNode)(VarDict*, char*);
    VarNode* head;
} VarDict;

typedef struct VarNode {
    char* name;
    void* location;
    int varType;
    VarNode* next;
} VarNode;

//////////////////////////////
//for funct access and storage
//////////////////////////////
/*FunctDict* createFunctDict();
typedef struct FunctDict {
    void (*sendValue)(char*, int);
    void (*storeVar)(char*, int, int);
    FunctNode* head;
} FunctDict;

FunctNode* createFunctNode();
typedef struct FunctNode {
    char* name;
    void* location;
    FunctNode* next;
} FunctNode;*/

#endif
