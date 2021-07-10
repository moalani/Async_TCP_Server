#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <mutex>
#include <vector>
#include "boost/asio.hpp"
#include "boost/thread/thread.hpp"
#include "Socket_server_delegate.h"


class Connection
{
public:
    Connection(boost::asio::io_context &io_context) : connection_socket(io_context)
    {

    }

    void start(std::shared_ptr<Test_delegate> delegate, int conn_id, unsigned int port)
    {
        std::cout << "New connection\n";
        boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string("127.0.0.1"), port);
        connection_socket.async_connect(endpoint, boost::bind(&Connection::connect_handler, this, boost::asio::placeholders::error));
        _delegate = delegate;
        connection_id = conn_id;
        read();
    }
    

    boost::asio::ip::tcp::socket &socket()
    {
        
        return connection_socket;
    }
    
    ~Connection()
    {
        std::cout << "Destroyed Connection\n";
        connection_socket.close();
    };
    static boost::shared_ptr<Connection> create(boost::asio::io_context &io_context)
    {
        return boost::shared_ptr<Connection>(new Connection(io_context));
    }
    int connection_id;

    //Socket_server server;
private:
    void connect_handler(const boost::system::error_code &error)
    {
        if (!error)
        {
            // Connect succeeded.
        }
    }
    void read()
    {
        // reading data at every new line entry.
        boost::asio::async_read_until(connection_socket, buffer, '\n',
                                      boost::bind(&Connection::handler, this,
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));
    }
    void handler(const boost::system::error_code &e, std::size_t size)
    {
        if (!e)
        {
            std::lock_guard<std::mutex> lock(_read_mutex);
            std::string line;
            std::istream is(&buffer);
            std::getline(is, line);
            std::vector<uint8_t> data_in(line.begin(), line.end());
            if (!line.empty()) {
                std::string id = "Connection ID  " + std::to_string(connection_id);
                _delegate->data_received(id, data_in);
            }
        }
        read();
    }
    boost::asio::ip::tcp::socket connection_socket;
    std::shared_ptr<Test_delegate> _delegate;
    boost::asio::streambuf buffer;
    mutable std::mutex _read_mutex;
};

/// \brief Handles a simple TCP server
class Socket_server
{
public:
    void handle_accept(boost::shared_ptr<Connection> connection, const boost::system::error_code &err);
    //void start_accept();
    void async_accept();
    /// \brief Creates an asynchonous TCP server
    ///
    /// The server operates asynchronously. This constructor returns immediately.
    ///
    /// \param port The port number on which to listen for connections
    ///
    /// \param delegate Interface called for certain events.
    ///
    /// \throw std::exception Error setting up the server
    
    Socket_server(unsigned int port, std::shared_ptr<Test_delegate> delegate);
    
    /// \brief The number of currently connected clients
    unsigned int connection_count();
    
    /// \brief Sends data to all connected clients
    ///
    /// \param data The data
    void broadcast(const std::vector<std::uint8_t> &data);
    void run_io_contex();
    void send(boost::asio::ip::tcp::socket &sock, const std::vector<std::uint8_t> &data);
    void handle_write(const boost::system::error_code & /*error*/,
                      size_t /*bytes_transferred*/);
    /// \brief Closes all connections and shuts down the server
    void do_close()
    {
        //_socket.close();
    }
    void close()
    {
        _io_context.post(boost::bind(&Socket_server::do_close, this));
    }
    ~Socket_server()
    {
        std::cout << "Server closing...\n";
        _io_context.stop();
        close();
        if (_thread.joinable())
            _thread.join();
    };
    Socket_server(const Socket_server &) = delete;
    void operator=(const Socket_server &) = delete;
    std::shared_ptr<Test_delegate> _delegate;
    
private:

    unsigned int _port;
    boost::asio::io_context _io_context;
    std::thread _thread;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::vector<boost::shared_ptr<Connection>> connections;
    mutable std::mutex _write_mutex;
    mutable std::mutex _connect_mutex;
    
};


