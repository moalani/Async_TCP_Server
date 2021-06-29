#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <mutex>
#include <vector>
#include "boost/asio.hpp"
#include "Socket_server_delegate.h"


namespace nbs
{

	namespace util
	{

		/// \brief Handles a simple TCP server
		class Socket_server
		{
		public:
			void async_accept(std::shared_ptr<Socket_server_delegate_handler>& delegate);
			/// \brief Creates an asynchonous TCP server
			///
			/// The server operates asynchronously. This constructor returns immediately.
			///
			/// \param port The port number on which to listen for connections
			///
			/// \param delegate Interface called for certain events.
			///
			/// \throw std::exception Error setting up the server

			Socket_server(unsigned int port, std::shared_ptr<Socket_server_delegate_handler>& delegate);

			/// \brief The number of currently connected clients
			unsigned int connection_count();

			/// \brief Sends data to all connected clients
			///
			/// \param data The data
			void broadcast(const std::vector<std::uint8_t>& data);
			void send(boost::asio::ip::tcp::socket& socket, const std::vector<std::uint8_t>& data);
			/// \brief Closes all connections and shuts down the server
			~Socket_server() {};
			Socket_server(const Socket_server&) = delete;
			void operator=(const Socket_server&) = delete;
			boost::asio::ip::tcp::acceptor _acceptor;
			// optional needs c++17 
			// refer to https://dens.website/tutorials/cpp-asio/async-tcp-server
			std::optional<boost::asio::ip::tcp::socket> socket;
			boost::asio::io_context& server_io_context;


		};


	} // namespace util

} // namespace nbs