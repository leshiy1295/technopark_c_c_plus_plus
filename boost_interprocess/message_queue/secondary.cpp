#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;

int main ()
{
   BOOST_TRY{
      //Open a message queue.
      message_queue mq
         (open_only        //only open
         ,"message_queue"  //name
         );

      unsigned int priority;
      message_queue::size_type recvd_size;

      //Receive 100 numbers
      for(int i = 0; i < 100; ++i){
         int number;
         mq.receive(&number, sizeof(number), recvd_size, priority);
         if(number != i || recvd_size != sizeof(number))
            return 1;
      }
   }
   BOOST_CATCH(interprocess_exception &ex){
      message_queue::remove("message_queue");
      std::cout << ex.what() << std::endl;
      return 1;
   } BOOST_CATCH_END
   message_queue::remove("message_queue");
   return 0;
}