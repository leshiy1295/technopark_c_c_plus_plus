#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/move/utility_core.hpp> //boost::move
#include <cassert>//assert
#include <cstring>//std::memset
#include <new>    //std::nothrow
#include <vector> //std::vector

int main()
{
   using namespace boost::interprocess;
   typedef managed_shared_memory::multiallocation_chain multiallocation_chain;

   //Remove shared memory on construction and destruction
   struct shm_remove
   {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
   } remover;

   managed_shared_memory managed_shm(create_only,"MySharedMemory", 65536);

   //Allocate 16 elements of 100 bytes in a single call. Non-throwing version.
   multiallocation_chain chain;
   managed_shm.allocate_many(std::nothrow, 100, 16, chain);

   //Check if the memory allocation was successful
   if(chain.empty()) return 1;

   //Allocated buffers
   std::vector<void*> allocated_buffers;

   //Initialize our data
   while(!chain.empty()){
      void *buf = chain.pop_front();
      allocated_buffers.push_back(buf);
      //The iterator must be incremented before overwriting memory
      //because otherwise, the iterator is invalidated.
      std::memset(buf, 0, 100);
   }

   //Now deallocate
   while(!allocated_buffers.empty()){
      managed_shm.deallocate(allocated_buffers.back());
      allocated_buffers.pop_back();
   }

   //Allocate 10 buffers of different sizes in a single call. Throwing version
   managed_shared_memory::size_type sizes[10];
   for(std::size_t i = 0; i < 10; ++i)
      sizes[i] = i*3;

   managed_shm.allocate_many(sizes, 10, 1, chain);
   managed_shm.deallocate_many(chain);
   return 0;
}