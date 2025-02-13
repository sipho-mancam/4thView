#include "AppMain.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppMain w;
    w.show();
    std::thread t(
        [&]() {
            for (int i = 0; i < 60; i++)
            {
                // Test stdout output
                std::cout << "Hello, this is stdout!\n" << std::flush;
                std::cout << "This text will appear in the QDockWidget.\n" << std::flush;

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
    );
   int ret = a.exec();

   if (t.joinable())
       t.join();

    return  ret;
}
