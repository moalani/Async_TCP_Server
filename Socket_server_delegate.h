#ifndef Socket_server_delegate_h
#define Socket_server_delegate_h
#include <iostream>
#include <vector>
// -Implement Socket_server class as described in comments using Boost asio library.
// -Follow the same naming conventions of this code.
// -Provide command line program that unit tests your implementation.
// -After initial research, please provide a rough time estimate for completion.
// I am aware of the difficulty in making accurate development time estimates.
// -If you encounter a problem that appears to prevent making an implementation
// as described, please contact me. Also contact me any time for assistance if
// required.


namespace nbs
{

	namespace util
	{

		/// \brief Handler for Socket_server events
		///
		/// \note All methods of this interface must be thread safe.
		class Socket_server_delegate
		{
			//private:


		public:
			/// \brief Called when a new connection is made to the server
			///
			/// \param connection_id A string that uniquely identifies this connection in other calls
			virtual void new_connection(const std::string& connection_id) = 0;

			/// \brief Called when a connection is lost
			///
			/// \param connection_id A unique identifier provided to new_connection when the connection being
			/// closed was connected
			virtual void connection_closed(const std::string& connection_id) = 0;

			/// \brief Called when data is received from a client
			///
			/// This method is called as soon as new data is available. The amount of data provided in each call is not
			/// defined, so implementation should make no assumptions about it.
			///
			/// \param connection_id The unique identifier for the connection which sent the data
			///
			/// \param data The data received.
			virtual void data_received(const std::string& connection_id, const std::vector<std::uint8_t>& data) = 0;

			/// \brief Called when there is some error
			///
			/// \param message Error message describing the error
			///
			/// \param fatal If true, the server is no longer in a functional state and may not continue
			/// to handle current or future connections.
			virtual void error(const std::string& message, bool fatal) = 0;
			virtual ~Socket_server_delegate() {}
		};

		class Test_delegate : public Socket_server_delegate
		{
		public:
			virtual void new_connection(const std::string& connection_id) {
				std::cout << "New connection: " << connection_id << '\n';
			}

			virtual void connection_closed(const std::string& connection_id) {
				std::cout << "Connection closed: " << connection_id << '\n';
			}

			virtual void data_received(const std::string& connection_id, const std::vector<std::uint8_t>& data) {
				std::cout << "Data received for " << connection_id << ": " << std::string(data.begin(), data.end()) << '\n';
			}

			virtual void error(const std::string& message, bool fatal) {
				std::cout << (fatal ? "Fatal error: " : "Error: ") << message << '\n';
			}
		};

	} // namespace util

} // namespace nbs


#endif /* Socket_server_delegate_h */
