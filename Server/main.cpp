#include"server.h"

int main (void)
 {
    try
    {
       auto server = Test2::Server::Create();

        server->Start();

        std::cin.get();

        server->Stop();
    }
    catch (const std::exception& ex)
    {
       std::cout << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unhandled exception..." << std::endl;
    }                                                         

 return 0;
}
