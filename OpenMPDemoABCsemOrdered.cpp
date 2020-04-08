#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <omp.h>
using std::string;
omp_lock_t writelock;
class SharedArray {
private:
   char* array;
   int size;
   bool usemutex;
   int index;
public:
   SharedArray(int n, bool use) : size(n), index(0), usemutex(use) {
      array = new char[size];
      std::fill(array, array+size, '-');
   }
   ~SharedArray() {
      delete[] array;
   }
   void addChar(char c, int index) {
      if (usemutex) omp_set_lock(&writelock);
      array[index] = c;
      spendSomeTime();
      if (usemutex) omp_unset_lock(&writelock);
   }
   void addChar2(char c) {
      array[index] = c;
      spendSomeTime();
      index++;
   }
   int countOccurrences(char c) {
      return std::count(array, array+size, c);
   }
   std::string toString() {
      return std::string(array, size);
   }
private:
   void spendSomeTime() {
      for (int i = 0; i < 10000; i++) {
         for (int j = 0; j < 100; j++) {
            // These loops shouldn't be removed by the compiler
         }
      }
   }
};


class ArrayFiller {
private:
   static const int nThreads = 3;
   static const int nTimes = 20;
   SharedArray* array;
public:
   ArrayFiller(bool use_lock){//, int method, int cluster) {
      array = new SharedArray(nThreads * nTimes, use_lock);
   }
   void fillArrayConcurrently(int mode) {
      ArrayFiller::run(mode);
   }

   void printStats() {
      std::cout << array->toString() << std::endl;
      for (int i = 0; i < nThreads; ++i)
         std::cout << (char) ('A'+i) << "=" 
                   << array->countOccurrences('A'+i) << " ";
      std::cout << std::endl;
   }
   ~ArrayFiller() {
      delete array;
   }
private:
   void run(int mode) {
      int i;
      int chunk = 5;
      omp_init_lock(&writelock);
      switch(mode){
         case 1: 
            #pragma omp parallel shared(array) private(i)
            {
               #pragma omp for schedule(static)
               for (i = 0; i < nTimes*nThreads; i++) {
                  array->addChar('A' + omp_get_thread_num(), i);
               }
            }
            break;
         case 2: 
            #pragma omp parallel shared(array) private(i)
            {
               #pragma omp for schedule(static, chunk)
               for (i = 0; i < nTimes*nThreads; i++) {
                  array->addChar('A' + omp_get_thread_num(), i);
               }
            }
            break;
         case 3: 
            #pragma omp parallel shared(array) private(i)
            {
               #pragma omp for schedule(dynamic)
               for (i = 0; i < nTimes*nThreads; i++) {
                  array->addChar('A' + omp_get_thread_num(), i);
               }
            }
            break;
         case 4: 
            #pragma omp parallel shared(array) private(i)
            {
               #pragma omp for schedule(dynamic, chunk)
               for (i = 0; i < nTimes*nThreads; i++) {
                  array->addChar('A' + omp_get_thread_num(), i);
               }
            }
            break;
         case 5: 
            #pragma omp parallel shared(array) private(i)
            {
               #pragma omp for schedule(guided)
               for (i = 0; i < nTimes*nThreads; i++) {
                  array->addChar('A' + omp_get_thread_num(), i);
               }
            }
            break;
         case 6: 
            #pragma omp parallel shared(array) private(i)
            {
               #pragma omp for schedule(guided, chunk)
               for (i = 0; i < nTimes*nThreads; i++) {
                  array->addChar('A' + omp_get_thread_num(), i);
               }
            }
            break;
         case 7: 
            #pragma omp parallel shared(array) private(i)
            {
               #pragma omp for schedule(runtime)
               for (i = 0; i < nTimes*nThreads; i++) {
                  array->addChar('A' + omp_get_thread_num(), i);
               }
            }
            break;
         case 8: 
            #pragma omp parallel shared(array) private(i)
            {
               #pragma omp for schedule(auto)
               for (i = 0; i < nTimes*nThreads; i++) {
                  array->addChar('A' + omp_get_thread_num(), i);
               }
            }
            break;
         case 9: 
            #pragma omp parallel shared(array) private(i)
            {
               for (i = 0; i < nTimes; i++) {
                  array->addChar2('A' + omp_get_thread_num());
               }
            }
            break;
      }
      
   omp_destroy_lock(&writelock);
}
};


int main() {
   std::cout << "Each thread should add its char to the array n times (n=20)" << std::endl;
   std::cout << "Correct results should total exactly nThreads*nTimes chars" << std::endl;

   std::cout << "Case 1: static no chunk" << std::endl;
   ArrayFiller m1(true);
   m1.fillArrayConcurrently(1);
   m1.printStats();

   std::cout << "Case 2: static chunk = 5" << std::endl;
   ArrayFiller m2(true);
   m2.fillArrayConcurrently(2);
   m2.printStats();

   std::cout << "Case 3: dynamic no chunk" << std::endl;
   ArrayFiller m3(true);
   m3.fillArrayConcurrently(3);
   m3.printStats();

   std::cout << "Case 4: dynamic chunk = 5" << std::endl;
   ArrayFiller m4(true);
   m4.fillArrayConcurrently(4);
   m4.printStats();

   std::cout << "Case 5: guided no chunk" << std::endl;
   ArrayFiller m5(true);
   m5.fillArrayConcurrently(5);
   m5.printStats();

   std::cout << "Case 6: guided chunk = 5" << std::endl;
   ArrayFiller m6(true);
   m6.fillArrayConcurrently(6);
   m6.printStats();

   std::cout << "Case 7: runtime" << std::endl;
   ArrayFiller m7(true);
   m7.fillArrayConcurrently(7);
   m7.printStats();

   std::cout << "Case 8: auto" << std::endl;
   ArrayFiller m8(true);
   m8.fillArrayConcurrently(8);
   m8.printStats();

   std::cout << "Case 9: static no lock" << std::endl;
   ArrayFiller m9(false);
   m9.fillArrayConcurrently(9);
   m9.printStats();
}