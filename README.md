# Async_TCP_Server


### Install  Dependencies
1. Install g++ using `sudo apt-get install g++`
2. Install boost asio using `sudo apt-get install libboost-all-dev`
3. Install cmake using `sudo apt-get -y install cmake`

### Build
Build from cmake file using `cmake .`

### Run
Run the executable using `./Async_TCP_Server`

### Test
If everything runs ok, go ahead and start testing the application by connecting to the server locally using telnet.
1. Install telnet using `sudo apt-get install telnet`
2. Open a new terminal window and connect `telnet localhost 1080`
3. Open a second terminal window and connect `telnet localhost 1080`
4. From the server, send a message. If the broadcast function was used, the message should be seen on both clients like this:
![Screenshot from 2021-07-04 17-22-05](https://user-images.githubusercontent.com/49099141/124403669-6633de80-dcec-11eb-834f-4a9105f2eddd.png)
