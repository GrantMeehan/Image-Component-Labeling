// Header file for array version of Stack Template class
// Source: www.cs.colorado.edu/~main/chapter7/stack1.h

#ifndef STACK_H
#define STACK_H
#include <cstdlib>
using namespace std;

template <class Item>
class stack
{
public:
  // TYPEDEFS AND MEMBER CONSTANT
  typedef size_t size_type;
  typedef Item value_type;
  static const size_type CAPACITY = 100;
  // CONSTRUCTOR
  stack() { used = 0; }
  // MODIFICATION MEMBER FUNCTIONS
  void push(const Item& entry);
  void pop();
  // CONSTANT MEMBER FUNCTIONS
  bool empty() const { return (used == 0);}
  size_type size() const { return used; }
  Item top() const;
private:
  Item data[CAPACITY];  // partially filled array
  size_type used;  // How much of array being used
};

#include "stack.template"

#endif //STACK_H
