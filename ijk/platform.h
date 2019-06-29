/*
 * File Name: platform.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/4/21 10:08:36
 */

#ifndef _PLATFORM_H
#define _PLATFORM_H

#if !defined(__unix__) && (defined(__APPLE__) && defined(__MACH__))
#define __unix__ 1
#endif

#ifdef __unix__
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif /* __unix__ */

#if defined(_WIN32) || defined(_WIN64)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif /* windows */

#include <fcntl.h>

#if defined(_MSC_VER)
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

#ifndef _WIN32

#if EAGAIN == EWOULDBLOCK
#define IJK_ERR_IS_EAGAIN(e) ((e) == EAGAIN)
#else
#define IJK_ERR_IS_EAGAIN(e) ((e) == EAGAIN || (e) == EWOULDBLOCK)
#endif

/* True iff e is an error that means a read/write operation can be retried. */
#define IJK_ERR_RW_RETRIABLE(e) ((e) == EINTR || IJK_ERR_IS_EAGAIN(e))
/* True iff e is an error that means an connect can be retried. */
#define IJK_ERR_CONNECT_RETRIABLE(e) ((e) == EINTR || (e) == EINPROGRESS)
/* True iff e is an error that means a accept can be retried. */
#define IJK_ERR_ACCEPT_RETRIABLE(e) \
    ((e) == EINTR || IJK_ERR_IS_EAGAIN(e) || (e) == ECONNABORTED)

/* True iff e is an error that means the connection was refused */
#define IJK_ERR_CONNECT_REFUSED(e) ((e) == ECONNREFUSED)

#else
/* Win32 */

#define IJK_ERR_IS_EAGAIN(e) ((e) == WSAEWOULDBLOCK || (e) == EAGAIN)

#define IJK_ERR_RW_RETRIABLE(e) ((e) == WSAEWOULDBLOCK || (e) == WSAEINTR)

#define IJK_ERR_CONNECT_RETRIABLE(e)                                      \
    ((e) == WSAEWOULDBLOCK || (e) == WSAEINTR || (e) == WSAEINPROGRESS || \
     (e) == WSAEINVAL)

#define IJK_ERR_ACCEPT_RETRIABLE(e) IJK_ERR_RW_RETRIABLE(e)

#define IJK_ERR_CONNECT_REFUSED(e) ((e) == WSAECONNREFUSED)

#endif

#endif
