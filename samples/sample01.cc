/*
 * File Name: sample01.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/28 15:45:08
 */

#include "ijk/network/asio.h"
#include "ijk/network/io_context_pool.h"

int main(int argc, char *argv[]) {
    /*
    asio::io_context ioc;
    asio::io_context::strand strand(ioc);

    asio::post(strand, [&](){
        bool val = strand.running_in_this_thread(); 
        if (val) {
            int a = 0;
            ++a;
        }
        int b = 0;
    });

    ioc.run();
    */

    ijk::io_context_pool pool;
    pool.run();


    return 0;
}
