#ifndef Wifiduino_h
#define Wifiduino_h

//#include "Arduino.h"

enum returnType {CHAR, STRING, INT, FLOAT, DOUBLE};

typedef struct Wifiduino Wifiduino;
typedef struct VarDict VarDict;
typedef struct VarNode VarNode;
typedef struct FunctNode FunctNode;
typedef struct FunctDict FunctDict;

//////////////////////////////
//Wifiduino itself
//////////////////////////////
Wifiduino* createWifiduino();
typedef struct Wifiduino {
    void (*function)(char*, void*);
    void (*variable)(char*, void*);
    VarDict* varDict;
    FunctDict* functDict;
} Wifiduino;

//////////////////////////////
//for var access and storage
//////////////////////////////
VarDict* createVarDict();

typedef struct VarDict {
    void (*sendValue)(char*, int);
    void (*storeVar)(char*, int, int);
    VarNode* (*findVarNode)(VarDict*, const char[]);
    VarNode* (*appendVarNode)(VarDict*, const char[]);
    void (*deleteVarNode)(VarDict*, const char[]);
    VarNode* head;
} VarDict;

typedef struct VarNode {
    const char* name;
    void* location;
    int varType;
    VarNode* next;
} VarNode;

//////////////////////////////
//for funct access and storage
//////////////////////////////
FunctDict* createFunctDict();
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
} FunctNode;

#endif
