#include <iostream>
#include "Socket_server.h"

using namespace nbs;
using namespace util;

int main() {

	boost::asio::io_context context;
	boost::asio::ip::tcp::socket socket_1(context);
	std::shared_ptr<Socket_server_delegate_handler> delegate = std::make_shared<Socket_server_delegate_handler>(std::move(socket_1), context, 15001);
	delegate->new_connection("127.0.0.1");
	Socket_server server(15001, delegate);
	server.async_accept(delegate);
	context.run();

	//std::cout << "Socket " << std::endl;
	return 0;
}
