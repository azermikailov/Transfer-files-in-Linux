#include"client.h"
#include"senderworker.h"

 enum : BYTE 
 {
    address = 1,
    path,
    sourse
 };

int main(int argv,char** args)
{
   try
   {
       if (argv < sourse) 
          throw std::invalid_argument("Invalid command line argument");

               auto client = Test2::CClient::Create(args[address]);
	
            if (args[path])
             {
	              	auto worker = Test2::IWorker::Create<Test2::CSenderWorker>(client,
		             	args[path]);

	              	worker->DoWork();
             }
             else
                std::cout << "path to file is empty" << std::endl;

	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unahdled exception..." << std::endl;
	}
   
  return 0;
}