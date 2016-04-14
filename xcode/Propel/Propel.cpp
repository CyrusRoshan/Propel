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

std::string PropelBase::PropelNode::toString() {
    switch (this->type) {
        case BOOL:
            return *(bool*)this->address ? "true" : "false";
            break;
            
        case INT:
            return std::to_string(*(int*)this->address);
            break;
            
        case CHAR:
            return std::to_string(*(char*)this->address);
            break;
            
        case FLOAT:
            return std::to_string(*(float*)this->address);
            break;
            
        case DOUBLE:
            return std::to_string(*(double*)this->address);
            break;
            
        case STRING:
            return *(std::string*)this->address;
            break;
        
        default:
            return "Error: invalid or nonexistent type entered";
    }
}

PropelBase::PropelDict::PropelDict() {
  this->head = NULL;
}

PropelBase::PropelNode* PropelBase::PropelDict::getNodeVal(std::string name, int type) {
    if (this->head) {
        PropelNode* ptr = this->head;
        while (ptr->next) {
            if (ptr->name == name) {
                return ptr;
            }
            ptr = ptr->next;
        }
    }
    return this->head;
}

PropelBase::PropelNode* PropelBase::PropelDict::setNodeVal(std::string name, void* address, int type) {
    if (this->head) {
        PropelNode* ptr = this->head;
        while (ptr->next) {
            if (ptr->name == name) {
                ptr->address = address;
                ptr->type = type;
                return ptr;
            }
            ptr = ptr->next;
        }
    }
    this->head = new PropelNode(name, address, type);
    return this->head;
}

void PropelBase::PropelDict::deleteNode(std::string name) {
    PropelNode* prev = this->head;
    PropelNode* ptr = NULL;
    while (prev->next) {
        ptr = prev->next;
        if (ptr->name == name) {
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
