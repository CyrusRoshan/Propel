#ifndef Propel_h
#define Propel_h

#include <string>//#include <Arduino.h>


enum {BOOL, INT, CHAR, FLOAT, DOUBLE, STRING};

class PropelBase { //all nested, because let's not interfere with devs' Arduino code more than we have to
    private:
        class PropelNode {
            public:
                PropelNode(std::string, void*, int);
                std::string toString();
                std::string name;
                void* address;
                int type;
                PropelNode* next;
        };

        class PropelDict {
            public:
                PropelDict();
                PropelNode* getNodeVal(std::string, int);
                PropelNode* setNodeVal(std::string, void*, int);
                void deleteNode(std::string);
                PropelNode* head;
        };

        PropelDict varDict;
        void writeMessage(PropelNode*, char*);
        std::string readMessage();
    
    public:
        PropelBase();
        void function (std::string, void*);
        void variable(std::string, void*, int);
        PropelNode debugFunction(std::string);
        PropelNode debugVariable(std::string);
} extern Propel;


#endif
