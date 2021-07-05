#include <iostream>
#include <string>
#include "Socket_server.h"

namespace nbs
{

	namespace util
	{
		Socket_server::Socket_server(unsigned int port, std::shared_ptr<Test_delegate> delegate) :
			_io_context(),
			_acceptor(_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {

		
			std::cout << "Server is online..." << std::endl;

			boost::asio::ip::tcp::resolver resolver(_io_context);
			boost::asio::ip::tcp::resolver::query query("127.0.0.1", std::to_string(port));
			boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
			async_accept();
			run_io_contex();
		}
		void Socket_server::run_io_contex() {
			try {
				_thread = std::thread(boost::bind(&boost::asio::io_context::run, &_io_context));
			}
			catch (std::exception &e) {
			
				std::cerr << e.what() << '\n';
			}
		}

		void Socket_server::async_accept()
		{
			boost::shared_ptr<Connection> connection = Connection::create(_io_context);
			_acceptor.async_accept(connection->socket(), [this, connection](boost::system::error_code ec)
				{
					if (!ec) {
					
						std::lock_guard<std::mutex> lock(_read_mutex);
						connections.push_back(connection);
						std::cout << "Connection Successful !" << std::endl;
						std::cout << "Current number of connections = " << connection_count() << std::endl;
						connection->start();
					}
					async_accept();
				});
		}
			

		void Socket_server::handle_accept(boost::shared_ptr<Connection>connection, boost::system::error_code const &error)
		{
			if (!error)
			{
				std::cout << "Connection Successful !" << std::endl;
				std::cout << "Current number of connections = " << connection_count() << std::endl;
				connection->start();
				async_accept();
			}
		}
		void Socket_server::send(boost::asio::ip::tcp::socket& sock, const std::vector<std::uint8_t>& data) {

			boost::asio::async_write(sock, boost::asio::buffer(data),
									 boost::bind(&Socket_server::handle_write, this,
												 boost::asio::placeholders::error,
												 boost::asio::placeholders::bytes_transferred));
		}
		void Socket_server::handle_write(const boost::system::error_code & /*error*/,
										 size_t /*bytes_transferred*/)
		{
			std::lock_guard<std::mutex> lock(_write_mutex);
		}

		unsigned int Socket_server::connection_count()
		{
			return static_cast<unsigned int>(connections.size());
		}

		void Socket_server::broadcast(const std::vector<std::uint8_t> &data)
		{
			for (int i = 0; i < connection_count(); i++)
			{
				send(connections[i]->socket(), data);
			}
		}
	}
}