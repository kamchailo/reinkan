#include "pch.h"
#include "Reinkan.h"

int main() 
{
    Reinkan::ReinkanApp app(800,600);

    try 
    {
        app.Run();
    }
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

    return 0;
}