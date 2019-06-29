/*
 * File  : console_sink.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/27 20:48:43
 */

#include "console_sink.h"
#include "ijk/base/termcolor.h"

#include <ostream>

namespace ijk {

ConsoleSink::ConsoleSink(LogFormatter formatter)
    : formatter_(std::move(formatter)) {}

void ConsoleSink::log(const LogMessage &msg) {
    auto content = formatter_(msg);
    switch (msg.level) {
        case kDebug:
            std::cout << termcolor::green << content;
            break;
        case kInfo:
            std::cout << termcolor::white << content;
            break;
        case kWarning:
            std::cout << termcolor::yellow << content;
            break;
        case kError:
            std::cerr << termcolor::red << content;
            break;
        default:
            break;
    }

    std::cout << termcolor::reset;
}

}  // namespace ijk
