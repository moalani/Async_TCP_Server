#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include "Socket_server_delegate.h"

namespace nbs {

	namespace util {

		void Socket_server_delegate_handler::new_connection(const std::string& connection_id) {
			_endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(connection_id), _port);
			std::cout << "Connected! " << std::endl;

		}
		// TODO: NOT IMPLEMENTED YET
		void Socket_server_delegate_handler::connection_closed(const std::string& connection_id) {
			_socket.close();
		}
		// TODO: NOT IMPLEMENTED YET
		void Socket_server_delegate_handler::data_received(const std::string& connection_id, const std::vector<std::uint8_t>& data) {
			std::cout << " data_received" << std::endl;
		}
		// TODO: NOT IMPLEMENTED YET
		void Socket_server_delegate_handler::error(const std::string& message, bool fatal) {
			std::cout << " error" << std::endl;
		}
		void Socket_server_delegate_handler::start()
		{

			boost::asio::async_read_until(_socket, _streambuf, '\n', [self = shared_from_this()](boost::system::error_code error, std::size_t bytes_transferred)
			{
				std::cout << std::istream(&self->_streambuf).rdbuf();
			});

			_socket.async_write_some(
				boost::asio::buffer(_streambuf.data()),
				boost::bind(&Socket_server_delegate_handler::handle_write,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}

		// handle_read & handle_write implementation reference https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
		void Socket_server_delegate_handler::handle_read(const boost::system::error_code& err, size_t bytes_transferred)
		{
			if (!err) {
				std::cout << data << std::endl;
			}
			else {
				std::cerr << "error: " << err.message() << std::endl;
				_socket.close();
			}
		}

		void Socket_server_delegate_handler::handle_write(const boost::system::error_code& err, size_t bytes_transferred)
		{
			if (!err) {
				std::cout << "Server acknowledges message" << std::endl;
			}
			else {
				std::cerr << "error: " << err.message() << std::endl;
				_socket.close();
			}
		}

		Socket_server_delegate_handler::Socket_server_delegate_handler(boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& _io_context, unsigned int _port)
			:_io_context(_io_context), _socket(std::move(socket)), _port(_port)
		{
		}


	}// util
}// nbs
