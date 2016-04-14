#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include "Propel.hpp"

PropelBase::PropelBase() {

}

PropelBase::PropelNode::PropelNode(std::string name, void* address, int type) {
  this->name = name;
  this->address = address;
  this->type = type;
  this->next = NULL;
}

std::string PropelBase::PropelNode::varToString() {
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

PropelBase::PropelNode* PropelBase::PropelDict::getNodeVal(std::string name) {
    PropelNode* ptr = this->head;
    while (ptr != NULL) {
        if (ptr->name == name) {
            return ptr;
        }
        ptr = ptr->next;
    }
    throw std::invalid_argument("Incorrect name entered");
}

PropelBase::PropelNode* PropelBase::PropelDict::setNodeVal(std::string name, void* address, int type) {
    if (this->head != NULL) {
        PropelNode* ptr = this->head;
        while (ptr->next != NULL) {
            if (ptr->name == name) {
                ptr->address = address;
                ptr->type = type;
                return ptr;
            }
            ptr = ptr->next;
        }
        ptr->next = new PropelNode(name, address, type);
        return ptr->next;
    }
    this->head = new PropelNode(name, address, type);
    return this->head;
}

void PropelBase::PropelDict::deleteNode(std::string name) {
    PropelNode* prev = this->head;
    PropelNode* ptr = NULL;
    while (prev->next != NULL) {
        ptr = prev->next;
        if (ptr->name == name) {
            prev->next = ptr->next;
            delete ptr;
            return;
        }
        prev = prev->next;
    }
}

void PropelBase::readMessage() {
    
}

void PropelBase::writeMessage(std::string message) {
    
}

void PropelBase::function(std::string name, void* address) {
    this->functDict.setNodeVal(name, address, -1);
}

void PropelBase::variable(std::string name, void* address, int type) {
    this->varDict.setNodeVal(name, address, type);
}

std::string PropelBase::getFunction(std::string name) {
    return "";
}

std::string PropelBase::getVariable(std::string name) {
    return this->varDict.getNodeVal(name)->varToString();
}

PropelBase Propel;
