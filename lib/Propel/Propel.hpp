#ifndef Propel_hpp
#define Propel_hpp

#include <Arduino.h>


class PropelBase { //all nested, because let's not interfere with devs' Arduino code more than we have to
  private:
    enum returnType {BOOL, INT, CHAR, FLOAT, DOUBLE, STRING}
    String readMessage();
    void writeMessage(VarNode*, char*);

    class PropelNode {
      public:
        char* name;
        void* location;
        int varType;
        PropelNode* next;
    };

    class PropelDict {
      public:
        void* getNode(char*, int);
        void setNode(char*, void*, int);
        void deleteNode(char*);
        PropelNode* head;
    } varDict;

  public:
    void function (char*, void*);
    void variable(char*, void*, int);
} Propel;


#endif
