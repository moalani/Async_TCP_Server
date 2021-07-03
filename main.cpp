#include <iostream>
#include "Socket_server.h"

using namespace nbs;
using namespace util;



int main() {
	try {
		Socket_server server(1080, std::make_shared<Test_delegate>());
		while (true) {
			std::string input;
			std::getline(std::cin, input);
			if (input.empty()) break;
			server.broadcast(std::vector<std::uint8_t>(input.begin(), input.end()));
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	catch (...) {
		std::cerr << "Unknown exception\n";
	}

	return 0;
}
