/*
 * File  : asio.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/07 17:49:25
 */

#pragma once
#ifndef ASIO_H_FRPC460D
#define ASIO_H_FRPC460D

#ifdef ASIO_STANDALONE

#include <asio.hpp>

#else

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
namespace asio {
using namespace boost::asio;
namespace errc = boost::system::errc;
using boost::system::error_code;
}  // namespace asio

#endif

#endif /* end of include guard:  ASIO_H_FRPC460D */
