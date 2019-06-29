///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2017 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef GSL_THREAD_H
#define GSL_THREAD_H

#include <thread>

#ifdef _MSC_VER
#pragma warning(push)

// turn off some warnings that are noisy about our Expects statements
#pragma warning(disable : 4127) // conditional expression is constant

// blanket turn off warnings from CppCoreCheck for now
// so people aren't annoyed by them when running the tool.
// more targeted suppressions will be added in a future update to the GSL
#pragma warning(disable : 26481 26482 26483 26485 26490 26491 26492 26493 26495)
#endif // _MSC_VER

namespace gsl
{

class joining_thread
{
    friend void swap(joining_thread& t1, joining_thread& t2) noexcept;

public:
    joining_thread() noexcept = default;

    joining_thread(joining_thread const&) = delete;
    joining_thread(joining_thread&& other) : t(std::move(other.t)) {}

    joining_thread(std::thread const&) = delete;
    joining_thread(std::thread&& other) noexcept : t(std::move(other)) {}

    joining_thread& operator=(joining_thread const&) = delete;
    joining_thread& operator=(joining_thread&& other) noexcept
    {
        t = std::move(other.t);
        return *this;
    }

    joining_thread& operator=(std::thread const&) = delete;
    joining_thread& operator=(std::thread&& other) noexcept
    {
        t = std::move(other);
        return *this;
    }

    template <typename Callable, typename... Args>
    explicit joining_thread(Callable&& f, Args&&... args)
        : t(std::forward<Callable>(f), std::forward<Args>(args)...)
    {
    }

    ~joining_thread() { if (t.joinable()) t.join(); }

    bool joinable() const { return t.joinable(); }

    std::thread::id get_id() const noexcept { return t.get_id(); }

    std::thread::native_handle_type native_handle() { return t.native_handle(); }

    void join() { t.join(); }

    void swap(joining_thread& other) noexcept
    {
        using std::swap;
        swap(t, other.t);
    }

private:
    std::thread t;
};

inline void swap(joining_thread& t1, joining_thread& t2) noexcept
{
    using std::swap;
    swap(t1.t, t2.t);
}

} // namespace gsl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // GSL_THREAD_H
