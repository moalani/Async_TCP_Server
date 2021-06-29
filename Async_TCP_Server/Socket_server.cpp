#include <iostream>
#include <string>
#include "Socket_server.h"

namespace nbs
{

	namespace util
	{
		// vector to keep track of new connections
		static std::vector<boost::asio::ip::tcp::socket> clients{};
		// mutex will be used to lock the vector when adding new connections.
		std::mutex mu;
		Socket_server::Socket_server(unsigned int port, std::shared_ptr<Socket_server_delegate_handler>& delegate) :
			server_io_context(delegate->_io_context), _acceptor(delegate->_io_context, delegate->_endpoint)
		{
			mu.lock();
			clients.emplace_back(std::move(delegate->_socket));
			mu.unlock();
			std::cout << "Num of connected clients = " << connection_count() << std::endl;

		}
		// ths function is responsible for handling new connections
		void Socket_server::async_accept(std::shared_ptr<Socket_server_delegate_handler>& delegate)
		{

			socket.emplace(server_io_context);
			std::cout << "Accepting new connections ..." << std::endl;
			_acceptor.async_accept(*socket, [&](boost::system::error_code error) {
				std::make_shared<Socket_server_delegate_handler>(std::move(*socket), delegate->_io_context, delegate->_port)->start();
				async_accept(delegate);
				});
		}
		void Socket_server::send(boost::asio::ip::tcp::socket& socket, const std::vector<std::uint8_t>& data) {
			boost::asio::write(socket, boost::asio::buffer(data));
		}
		unsigned int Socket_server::connection_count() {
			return static_cast<unsigned int>(clients.size());
		}
		// send data to all clients connected
		// TODO: find a way to test this function in "real-time"
		void Socket_server::broadcast(const std::vector<std::uint8_t>& data) {
			for (int i = 0; i < connection_count(); i++) {
				send(clients[i], data);
			}

		}

	}
}