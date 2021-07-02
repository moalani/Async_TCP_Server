#include <iostream>
#include <string>
#include "Socket_server.h"

namespace nbs
{

	namespace util
	{
		Socket_server::Socket_server(unsigned int port, std::shared_ptr<Test_delegate> delegate) :
			_io_context(),
			_thread(boost::bind(&boost::asio::io_context::run, &_io_context)),
			_socket(_io_context),
			_acceptor(_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {

		
			std::cout << "Server is online..." << std::endl;

			boost::asio::ip::tcp::resolver resolver(_io_context);
			boost::asio::ip::tcp::resolver::query query("127.0.0.1", std::to_string(port));
			boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
			async_accept();
		}

		void Socket_server::async_accept()
		{
			Connection* connection = new Connection(_io_context);
			_acceptor.async_accept(connection->socket(), [this, connection](boost::system::error_code ec)
				{
					if (!ec) {
					
						std::lock_guard<std::mutex> guard(mu);
						//connections.emplace_back(new boost::asio::ip::tcp::socket(_io_context)); => this works but message is not sent.
						connections.push_back(std::move(connection->socket()));	
						std::cout << "Connection Successful!" << std::endl;
						std::cout << "Current number of connections = " << connection_count() << std::endl;
						connection->start();
					}
					else {
					
						delete connection;
					}

					async_accept();
				});
		}

		void Socket_server::send(boost::asio::ip::tcp::socket& sock, const std::vector<std::uint8_t>& data) {
			boost::asio::write(sock, boost::asio::buffer(data));
		}

		unsigned int Socket_server::connection_count() {
			return static_cast<unsigned int>(connections.size());
		}

		void Socket_server::broadcast(const std::vector<std::uint8_t>& data) {
			for (int i = 0; i < connection_count(); i++) {
				send(connections[i], data);
			}

		}
	}
}