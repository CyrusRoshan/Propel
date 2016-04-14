#ifndef Propel_h
#define Propel_h

#include <string>//#include <Arduino.h>


enum {BOOL, INT, CHAR, FLOAT, DOUBLE, STRING};

class PropelBase { //all nested, because let's not interfere with devs' Arduino code more than we have to
    private:
        class PropelNode {
            public:
                PropelNode(std::string, void*);
                PropelNode(std::string, void*, int);
                std::string varToString();
                std::string name;
                void* address;
                int type;
                PropelNode* next;
        };

        class PropelDict {
            public:
                PropelDict();
                PropelNode* getNodeVal(std::string);
                PropelNode* setNodeVal(std::string, void*, int);
                void deleteNode(std::string);
                PropelNode* head;
        };

        PropelDict varDict;
        PropelDict functDict;
    
    public:
        PropelBase();
        void function (std::string, void*);
        void variable(std::string, void*, int);
    
        void writeMessage(std:: string);
        void readMessage();
    
        char serialRead();
    
        std::string getFunction(std::string);
        std::string getVariable(std::string);
} extern Propel;


#endif
