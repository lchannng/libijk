/*
 * File Name: delay.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2019/08/30 19:34:40
 */

#include "ijk/base/ignore_unused.hpp"
#include "ijk/base/logging.hpp"
#include "ijk/network/io.hpp"

using namespace ijk;

int main(int argc, char *argv[]) {
    ignore_unused(argc, argv);
    IJK_INITIALIZE_LOGGING();

    io_t io;

    LOG_INFO("before delay");
    ijk::delay(io, std::chrono::seconds(1)).finally([](auto) {
        LOG_INFO("delay 1 second.");
    });

    asio::steady_timer timer(io.context());
    delay(timer, std::chrono::seconds(1)).finally([](auto){
        LOG_INFO("delay 1 second.");
    });

    io.run();
    return 0;
}
