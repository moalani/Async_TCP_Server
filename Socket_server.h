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

namespace nbs
{

	namespace util
	{
		class Connection
		{
		public:
			Connection(boost::asio::io_context& io_context) : connection_socket(io_context) {
				
			}

			void start() {
			
				read();
			}

			boost::asio::ip::tcp::socket& socket() {
			
				return connection_socket;
			}

			~Connection() {
				connection_socket.close();
				std::cout << "Destroyed Connection\n";
			};
			static boost::shared_ptr<Connection> create(boost::asio::io_context& io_context) {
				std::cout << "Created new connection\n";
				return boost::shared_ptr<Connection>(new Connection(io_context));
			}
		
		//Socket_server server;
		private:
			void read() {
			
				boost::asio::async_read(connection_socket, boost::asio::buffer(message),
					[this](boost::system::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							std::cout << message << std::endl;
						}
					});
			}		
			boost::asio::ip::tcp::socket connection_socket;
			enum { max_length = 1024 };
			char message[max_length];
			
		};

		/// \brief Handles a simple TCP server
		class Socket_server : public boost::enable_shared_from_this<Socket_server>
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
			void broadcast(const std::vector<std::uint8_t>& data);
			void run_io_contex();
			void send(boost::asio::ip::tcp::socket& sock, const std::vector<std::uint8_t>& data);
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
			~Socket_server() {
				std::cout << "Server closing...\n";
				_io_context.stop();
				close();
				if (_thread.joinable())
					_thread.join();
			};
			Socket_server(const Socket_server&) = delete;
			void operator=(const Socket_server&) = delete;
			
		private:
			boost::asio::io_context _io_context;
			std::thread _thread;
			boost::asio::ip::tcp::acceptor _acceptor;
			std::vector<boost::shared_ptr<Connection> > connections;
			mutable std::mutex _write_mutex;
			mutable std::mutex _read_mutex;
		};



	} // namespace util

} // namespace nbs