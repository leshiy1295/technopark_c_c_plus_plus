#include <boost/interprocess/anonymous_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

int main ()
{
   using namespace boost::interprocess;
   try {
      //Create an anonymous shared memory segment with size 1000
      mapped_region region(anonymous_shared_memory(1000));

      //Write all the memory to 1
      std::memset(region.get_address(), 1, region.get_size());

      //The segment is unmapped when "region" goes out of scope
   } catch(interprocess_exception &ex) {
      std::cout << ex.what() << std::endl;
      return 1;
   }
}