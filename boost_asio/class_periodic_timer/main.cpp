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

struct printer {
  printer(boost::asio::io_context& io) : timer_(io, std::chrono::seconds(1)) {
    timer_.async_wait(std::bind(&printer::print, this));
  }

  ~printer() {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print() {
    if (count_ >= 5) { return; }
    std::cout << count_ << std::endl;
    ++count_;

    timer_.expires_at(timer_.expiry() + std::chrono::seconds(1));
    timer_.async_wait(std::bind(&printer::print, this));
  }

private:
  boost::asio::steady_timer timer_;
  int count_ = 0;
};

int main() {
  boost::asio::io_context io;
  printer p(io);
  io.run();
}