#include <stdlib.h>
#include <string.h>
#include "Propel.hpp"

PropelBase::PropelDict::PropelNode::PropelNode(const char* name, void* address, int type) {
  this->name = name;
  this->address = address;
  this->type = type;
  this->next = NULL;
}

PropelBase::PropelDict::PropelDict() {
  this->head = NULL;
}

PropelBase::PropelBase() {

}

void PropelBase::function(const char* name, void* address) {

}

void PropelBase::variable(const char* name, void* address, int type) {

}


PropelBase Propel;
