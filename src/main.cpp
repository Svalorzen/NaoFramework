#include <NaoFramework/Core/Brain.hpp>
#include <NaoFramework/Console/Console.hpp>
#include <NaoFramework/Log/Frontend.hpp>

int main(int argc, const char * argv[]) {
    // Must call this!
    NaoFramework::Log::init();

    namespace pl = std::placeholders;
    using namespace NaoFramework::Console;
    using namespace NaoFramework::Core;

    Console c("NaoFramework> ");

    Brain brain;
    c.registerCommand("add",    std::bind(&Brain::addDynamicModule,     &brain, pl::_1));
    c.registerCommand("create", std::bind(&Brain::createWave,           &brain, pl::_1));
    c.registerCommand("test",   std::bind(&Brain::execute,              &brain, pl::_1));

    std::cout << "\nWelcome to the NaoFramework command line interface!\n";
    // Default running script
    if ( argc > 1 ) {
        std::cout << "It looks like you have a script to run. Let's get to it.\n\n";
        if ( c.executeFile(argv[1]) == Console::ReturnCode::Quit ) return 0;
    }

    c.executeCommand("help");
    while ( c.readLine() != Console::ReturnCode::Quit ) {}

    return 0;
}
