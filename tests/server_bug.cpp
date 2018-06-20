//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include "asio.hpp"

using asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    do_read();
  }

private:
  void do_read()
  {
    auto self(shared_from_this());
    socket_.async_read_some(asio::buffer(data_),
          [this, self](const std::error_code& ec, const std::size_t length)
          {
            std::cout << "Length: " << length << std::endl;
            if (!ec)
            {
              do_write(length);
            }
            else
            {
              std::cerr << ec << std::endl;
            }
          });
  }

  void do_write(std::size_t length)
  {
    auto self(shared_from_this());
    std::cout << "Data: " << std::endl << std::string(std::begin(data_), std::end(data_)) << std::endl;
    for (int i = 0; i < 100000; i++)
    {
      std::string a = "aaaa";
    }
    for (int i = 0; i < 100000; i++)
    {
      std::string a = "aaaa";
    }
    for (int i = 0; i < 100000; i++)
    {
      std::string a = "aaaa";
    }
    for (int i = 0; i < 100000; i++)
    {
      std::string a = "aaaa";
    }
    asio::async_write(socket_, asio::buffer(data_, length),
          [this, self](const std::error_code& ec, const std::size_t /*length*/)
          {
            if (!ec)
            {
              do_read();
            }
          });
  }

  // The socket used to communicate with the client.
  tcp::socket socket_;

  // Buffer used to store data received from the client.
  std::array<char, 1024> data_;

  // The allocator to use for handler-based custom memory allocation.
};

class server
{
public:
  server(asio::io_service& io_service, short port) :
    // : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      socket_(io_service)
  {
    acceptor_ = new asio::ip::tcp::acceptor(
      io_service, tcp::endpoint(tcp::v4(), port)
      );
    do_accept();
  }

private:
  void do_accept()
  {
    std::cout << "Accepting new conn" << std::endl;
    acceptor_->async_accept(socket_,
        [this](const std::error_code& ec)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket_))->start();
          }
          else
          {
            std::cerr << ec << std::endl;
          }

          do_accept();
        });
  }

  tcp::acceptor* acceptor_;
  tcp::socket socket_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: server <port>\n";
      return 1;
    }

    asio::io_service io_service;
    server s(io_service, std::atoi(argv[1]));
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

