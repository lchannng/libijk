/*
 * File Name: delay.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2019/08/30 19:34:40
 */

 #include "ijk/base/logging.hpp"
 #include "ijk/network/delay.hpp"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();
    io_t io;

    LOG_INFO("before delay");
    ijk::delay(io, std::chrono::seconds(1)).finally([](auto) {
        LOG_INFO("delay 1 second.");
    });

    async(io, []() {
        LOG_INFO("async");
    });

    io.run();
    return 0;
}
