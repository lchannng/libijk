/*
 * File Name: future.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2019/08/30 13:56:08
 */

 #include "ijk/base/logging.hpp"
 #include "ijk/future/future.h"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();

    promise<int> pm;

    auto fut = pm.get_future();
    fut.then([](int n) {
        LOG_INFO("n: {}", n);
    }).finally([](auto n) {
        LOG_INFO("finally");
    });

    // pm.set_value(111);
    pm.set_exception(std::make_exception_ptr(std::logic_error("error")));

    return 0;
}
