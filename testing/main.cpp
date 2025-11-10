#include "serializer/core/serializer.h"
#include "engine/utility/uid.h"
#include <thread>
#include <chrono>

int main() {
    std::cout << utl::uid::get_uid() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    std::cout << utl::uid::get_uid() << std::endl;


    return 0;
}