#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/process.hpp>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <thread>

int main ()
{
   using namespace boost::interprocess;
   std::cout << "Hello from " << boost::this_process::get_id() << "\n";
   BOOST_TRY{
    //   struct file_remove
    //   {
    //      file_remove() { std::remove("file_name"); }
    //      ~file_remove(){ std::remove("file_name"); }
    //   } file_remover;
      struct mutex_remove
      {
         mutex_remove() { named_mutex::remove("fstream_named_mutex"); }
         ~mutex_remove(){ named_mutex::remove("fstream_named_mutex"); }
      } remover;

      //Open or create the named mutex
      named_mutex mutex(open_or_create, "fstream_named_mutex");

      std::ofstream file("file_name", std::ios::app);

      for(int i = 0; i < 10; ++i){

         //Do some operations...

         //Write to file atomically
         scoped_lock<named_mutex> lock(mutex);
         file << "Process name -> " << boost::this_process::get_id() << ", ";
         file << "This is iteration #" << i;
         file << std::endl;
         std::this_thread::sleep_for(std::chrono::seconds(1));
      }
   }
   BOOST_CATCH(interprocess_exception &ex){
      std::cout << ex.what() << std::endl;
      return 1;
   } BOOST_CATCH_END
   return 0;
}