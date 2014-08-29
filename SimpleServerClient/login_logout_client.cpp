//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <string>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>


using boost::asio::ip::tcp;

class chat_client
{
public:
  chat_client(boost::asio::io_service& io_service,
      tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service),
      socket_(io_service)
  {
    boost::asio::async_connect(socket_, endpoint_iterator,
        boost::bind(&chat_client::handle_connect, this,
          boost::asio::placeholders::error));
  }

  void write(const std::string& msg)
  {
      std::cout << "going to post. "<<std::endl;
    io_service_.post(boost::bind(&chat_client::do_write, this, msg));
  }

  void close()
  {
      std::cout <<"client closed" << std::cout;
    io_service_.post(boost::bind(&chat_client::do_close, this));
  }

private:

  void handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      //get response
        boost::asio::async_read_until(socket_, data_, "\r\n",
            boost::bind(
              &chat_client::handle_read_header, this,
              boost::asio::placeholders::error));
    }
  }

  void handle_read_header(const boost::system::error_code& error)
  {
    if (!error )
    {
        // print out
        std::ostringstream ss;
        ss << &data_;
        std::cout << "client read: " << ss.str() << "\n";
        boost::asio::async_read_until(socket_, data_, "\r\n",
            boost::bind(
              &chat_client::handle_read_header, this,
              boost::asio::placeholders::error));
    }
    else
    {
      do_close();
    }
  }

  void do_write(std::string msg)
  {
      std::cout << "Going to write."<<std::endl;
      boost::asio::async_write(socket_,
          boost::asio::buffer(msg.c_str(),std::strlen(msg.c_str())),
          boost::bind(&chat_client::handle_write, this,
            boost::asio::placeholders::error));
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
        std::cout << "write done." << std::endl;
    }
    else
    {
        std::cout << "write error.";
      do_close();
    }
  }

  void do_close()
  {
    std::cout << "socket close.";
    socket_.close();
  }

private:
  boost::asio::io_service& io_service_;
  tcp::socket socket_;
  boost::asio::streambuf data_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[1], argv[2]);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    chat_client c(io_service, iterator);

    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

    using namespace std;
    std::string input = "";
    while (std::getline(cin, input))
    {
      using namespace std; // For strlen and memcpy.
      cout << "echo: " << input << endl;

      c.write(input+"\r\n");
     // c.write(input);
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}