#include <stdlib.h>
#include <string.h>
#include "Propel.hpp"


PropelBase::PropelBase() {

}

PropelBase::PropelNode::PropelNode(std::string name, void* address, int type) {
  this->name = name;
  this->address = address;
  this->type = type;
  this->next = NULL;
}

PropelBase::PropelDict::PropelDict() {
  this->head = NULL;
}

void PropelBase::PropelDict::deleteNode(std::string name) {
    PropelNode* prev = this->head;
    PropelNode* ptr = NULL;
    if (ptr->name == name) {
        this->head = this->head->next;
        delete prev;
        return;
    }
    while (prev->next) {
        ptr = prev->next;
        if (ptr->name == name) { //if string literals are equal
            prev->next = ptr->next;
            delete ptr;
            return;
        }
        prev = prev->next;
    }
}

void PropelBase::function(std::string name, void* address) {

}

void PropelBase::variable(std::string name, void* address, int type) {

}


PropelBase Propel;
