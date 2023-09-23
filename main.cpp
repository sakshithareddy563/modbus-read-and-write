#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

int main()
{
    spdlog::info("hello");
    try
    {
        auto mylogger = spdlog::basic_logger_mt("mylogger", "logs.txt");
        mylogger ->info("helllo");
    }
    catch(const spdlog::spdlog_ex &ex)
    {
        std::cout << ex.what() << std::endl;

    }

}