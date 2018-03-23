// Copyright 2018 Joren Brunekreef
#ifndef pool_hpp
#define pool_hpp

#include <array>
// T is the type of object stored
// N is the size of the pool
// Poolable objects need getter and setter methods for the key variable
// to store their position in the array
template <class T, int N>
class Pool {
 public:
     T* elements = new T[N+1];
     int first;  // the index of the first 'empty' slot
     int total;  // total number of 'active' slots

     Pool() {
         first = 1;
         total = 0;
         for (int i = 1; i < N+1; i++) {
             elements[i].setKey(-(i+1));
         }
     }

     T& create() {
         int tmp = first;
         first = -elements[tmp].getKey();  // the 'key' value of an empty slot points to the next empty slot
         elements[tmp].setKey(tmp);  // 'newly created' object now has key pointing to its position in 'elements' array
         total++;
         return elements[tmp];
     }

     void destroy(int i) {
         elements[i].setKey(-first);  // 'deactivate' object at position i
         first = i;  // reset index of first inactive object

         total--;
     }

     void destroy(const T& obj) {
         auto tmp = obj.getKey();
         obj.setKey(-first);  // 'deactivate' object
         first = tmp;  // reset index of first inactive object

         total--;
     }

     T& pick() {
         return elements[1 /* random */];
     }

     int size() {
         return total;
     }

     T& operator[] (int i) {
         return elements[i];
     }

     void log() {
         for (int i = 0; i < sizeof(elements); i++) {
             printf("%d: %d \n", i, elements[i].getKey());
         }
         printf("first: %d\n", first);
         printf("--\n");
     }
};
#endif
