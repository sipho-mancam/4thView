#include <iostream>
#include <conio.h>
#include "kevents.hpp"
#include "processes_manager.hpp"

int main()
{
    std::cout << "Hello World!\n";
    json globalConfig = KEvents::__load_config__();

    ProcessesManager procMan(globalConfig);

    procMan.init();

    _getch();
    return 0;
}
