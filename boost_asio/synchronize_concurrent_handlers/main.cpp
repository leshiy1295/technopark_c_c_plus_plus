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
#include <boost/bind/bind.hpp>

struct printer {
  printer(boost::asio::io_context& io) :
      strand_(boost::asio::make_strand(io)),
      timer1_(io, std::chrono::seconds(1)),
      timer2_(io, std::chrono::seconds(1)) {
    timer1_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer::print1, this)));
    timer2_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer::print2, this)));
  }

  ~printer() { std::cout << "Final count is " << count_ << std::endl; }

  void print1() {
    if (count_ >= 10) { return; }
    std::cout << "Timer 1: " << count_ << std::endl;
    ++count_;

    timer1_.expires_at(timer1_.expiry() + std::chrono::seconds(1));
    timer1_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer::print1, this)));
  }

  void print2() {
    if (count_ >= 10) { return; }
    std::cout << "Timer 2: " << count_ << std::endl;
    ++count_;

    timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));
    timer2_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer::print2, this)));
  }

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::asio::steady_timer timer1_, timer2_;
  int count_ = 0;
};

int main() {
  boost::asio::io_context io;
  printer p(io);
  std::thread t(boost::bind(&boost::asio::io_context::run, &io));
  io.run();
  t.join();
}