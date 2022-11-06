//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t, int* count) {
    if (*count >= 5) { return; }
    std::cout << *count << std::endl;
    ++(*count);

    t->expires_at(t->expiry() + std::chrono::seconds(1));
    t->async_wait(std::bind(print, std::placeholders::_1, t, count));
}

int main() {
  boost::asio::io_context io;

  int count = 0;
  boost::asio::steady_timer t(io, std::chrono::seconds(1));
  t.async_wait(std::bind(print, std::placeholders::_1, &t, &count));

  io.run();

  std::cout << "Final count is " << count << std::endl;
}