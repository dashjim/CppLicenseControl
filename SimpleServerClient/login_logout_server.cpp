
#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>

using boost::asio::ip::tcp;
class param_t{
    
    std::string     m_value;
    std::string     m_name;
public:
    void            set_value(std::string value){m_value=value;}
    std::string     get_value(){return m_value;}
    void            set_name(std::string name){m_name=name;}
    std::string     get_name(){return m_name;}
};

typedef std::vector<param_t> param_vector;

class api_t{

public:
    std::string m_operation;
    param_vector params;

    void set_one_para(param_t param){
        params.push_back(param);
    }

    void print(){
        std::cout << "op: " <<m_operation << std::endl;
        std::cout << "params:" << std::endl;
        for (int i=0; i!=params.size(); i++) {
            std::cout << "\tname: " << params[i].get_name()<< std::endl;
            std::cout << "\tvalue: " << params[i].get_value()<< std::endl;
        }
    }
};

//----------------------------------------------------------------------

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void deliver(const std::string msg) = 0;
};

typedef boost::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_session : public chat_participant, public boost::enable_shared_from_this<chat_session>
{
public:
  chat_session(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  { 
    boost::asio::async_read_until(socket_, data_, "\r\n",
        boost::bind(
          &chat_session::handle_read_header, shared_from_this(),
          boost::asio::placeholders::error));
  }

  void deliver(const std::string msg)
  {
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg.c_str(),std::strlen(msg.c_str())),
        boost::bind(&chat_session::handle_write, shared_from_this(),
        boost::asio::placeholders::error));
  }

  void handle_read_header(const boost::system::error_code& error)
  {
    if (!error)
    {
        //print it out
        using namespace std;
        
        std::ostringstream ss;
        ss << &data_;
        std::string tmps = ss.str();
        cout << "received: " << ss.str() << endl;
        if(tmps.size()>2){
            boost::erase_all(tmps, " ");
            // remove the \r\n
            tmps.resize(tmps.size() -2); 
            cout << "received: " << tmps << endl;
            parse_json(tmps);
        }
        //response to sender
        deliver("Server: I got it.\r\n");

        /*TODO need to close the connection.*/
        start();//start to wait for another line of incoming data.
    }
    else
    {
     std::cout << "read error"<< std::endl;
    }
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
        std::string result = "write done.\r\n";
        std::cout << result;

    }
    else
    {
      //room_.leave(shared_from_this());
        std::cout << "write error";
    }
  }

  int parse_json(std::string str)  
    {  
      //{\"operation\": "login":, \"para\": [{ \"name\":\"ext\",\"value\": \"888888\"}, {\"name\":\"pwd\", \"value\":\"guessME!\"  } ]}\r\n
      using namespace boost::property_tree;  
      
      std::stringstream ss(str);  
      ptree pt;  
      try{      
        read_json(ss, pt);  
      }  
      catch(ptree_error & e) {  
          std::cout << "parse JSON error - "<<e.what()<<std::endl;
          return 1;
      }
      
      try{  
        std::string op = pt.get<std::string>("operation");
        std::cout << "operation:  "<< op.c_str() << std::endl;
        received_call.m_operation = op.c_str();
          
        BOOST_FOREACH(ptree::value_type &v, pt.get_child("para")){
            param_t result;
            result.set_value(v.second.get<std::string>("value"));
            result.set_name(v.second.get<std::string>("name"));
            received_call.set_one_para(result);
        }
        received_call.print();
 
      }  
      catch (ptree_error & e)  
      {  
          std::cout << "ptree error"<<std::endl;
        return 2;  
      }  
      return 0;  
    }  

private:
  tcp::socket socket_;
  boost::asio::streambuf data_;
  std::ostringstream oss_;
  api_t received_call;
};


typedef boost::shared_ptr<chat_session> chat_session_ptr;

//----------------------------------------------------------------------

class chat_server
{
public:
  chat_server(boost::asio::io_service& io_service,
      const tcp::endpoint& endpoint)
    : io_service_(io_service),
      acceptor_(io_service, endpoint)
  {
    start_accept();
  }

  void start_accept()
  {
    chat_session_ptr new_session(new chat_session(io_service_));
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&chat_server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

  void handle_accept(chat_session_ptr session,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      session->start();
    }else{
        std::cout << "error to start";
    }

    start_accept();
  }

private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};

typedef boost::shared_ptr<chat_server> chat_server_ptr;
typedef std::list<chat_server_ptr> chat_server_list;

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: chat_server <port> [<port> ...]\n";
      return 1;
    }

    boost::asio::io_service io_service;

    chat_server_list servers;
    for (int i = 1; i < argc; ++i)
    {
      using namespace std; // For atoi.
      tcp::endpoint endpoint(tcp::v4(), atoi(argv[i]));
      chat_server_ptr server(new chat_server(io_service, endpoint));
      servers.push_back(server);
    }

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}