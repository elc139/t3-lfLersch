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
   void addChar(char c) {
      if(usemutex){
         #pragma omp ordered
         {
            array[index] = c;
            spendSomeTime();
            index++;
         }
      }else{
            array[index] = c;
            spendSomeTime();
            index++;
      }

      //if (usemutex) omp_unset_lock(&writelock);
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
   void fillArrayConcurrently() {
      #pragma omp parallel
      {
         #pragma omp for schedule(runtime) ordered
         for(int i = 0; i < nTimes * omp_get_num_threads(); i++){
            array->addChar('A' + omp_get_thread_num());
         }
      }
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
};


int main() {
   std::cout << "Each thread should add its char to the array n times (n=20)" << std::endl;
   std::cout << "Correct results should total exactly nThreads*nTimes chars" << std::endl;

   std::cout << "Case 1: static no chunk" << std::endl;
   ArrayFiller m1(true);
   omp_set_schedule(omp_sched_static, 0);
   m1.fillArrayConcurrently();
   m1.printStats();

   std::cout << "Case 2: static chunk = 5" << std::endl;
   ArrayFiller m2(true);
   omp_set_schedule(omp_sched_static, 5);
   m2.fillArrayConcurrently();
   m2.printStats();

   std::cout << "Case 3: dynamic no chunk" << std::endl;
   ArrayFiller m3(true);
   omp_set_schedule(omp_sched_dynamic, 0);
   m3.fillArrayConcurrently();
   m3.printStats();

   std::cout << "Case 4: dynamic chunk = 5" << std::endl;
   ArrayFiller m4(true);
   omp_set_schedule(omp_sched_dynamic, 5);
   m4.fillArrayConcurrently();
   m4.printStats();

   std::cout << "Case 5: guided no chunk" << std::endl;
   ArrayFiller m5(true);
   omp_set_schedule(omp_sched_guided, 0);
   m5.fillArrayConcurrently();
   m5.printStats();

   std::cout << "Case 6: guided chunk = 5" << std::endl;
   ArrayFiller m6(true);
   omp_set_schedule(omp_sched_guided, 5);
   m6.fillArrayConcurrently();
   m6.printStats();

   std::cout << "Case 7: auto" << std::endl;
   ArrayFiller m7(true);
   omp_set_schedule(omp_sched_auto, 5);
   m7.fillArrayConcurrently();
   m7.printStats();

   std::cout << "Case 8: static no lock chunk = 5" << std::endl;
   ArrayFiller m9(false);
   omp_set_schedule(omp_sched_static, 5);
   m9.fillArrayConcurrently();
   m9.printStats();
}