#include <iostream>
#include <string>
#include <ostream>
#include <ctime>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <sfml/Network.hpp>
#include <sfml/Network/TcpSocket.hpp>
#include <sfml/Network/IpAddress.hpp>

using namespace std;

#define IP              "192.168.0.19"
#define PORT            5000

bool is_host_up(const string& address, int port) {
  sf::TcpSocket socket;
  bool open = (socket.connect(sf::IpAddress(address), port) == sf::Socket::Done);
  socket.disconnect();
  return open;
}

// We use sfml
void post_data(const time_t seconds, float total_price, string food) {
  if(is_host_up(IP, PORT)) {
    std::stringstream ss;
    ss << seconds;
    string content = "time=";
    content += ss.str();
    content +="&total_price=";
    content += to_string(total_price);
    content += "&food=";
    content += food;

    // prepare the request
    sf::Http::Request request("/firmware-api/v1/place_order", sf::Http::Request::Post);

    // encode the parameters in the request body
    std::ostringstream stream;
    stream << content;
    request.setBody(stream.str());

    // send the request
    sf::Http server(IP, 5000);
    sf::Http::Response response = server.sendRequest(request);

    if (response.getStatus() == sf::Http::Response::NoContent) {
        // check the contents of the response
        std::cout << "Order sent to server." << std::endl << std::endl;
    }
    else {
        std::cout << "Request failed." <<  std::endl << std::endl;
    }
  } else {
    std::cout << "Can't connect to API endpoint." << std::endl << std::endl;
  }
}

// we aren't using boost yet
void boost_post_data(const time_t seconds, float total_price, string food) {
  boost::asio::ip::tcp::iostream stream;

  string content = "data=";

  stream.connect(IP, to_string(PORT));
  stream << "POST /firmware-api/v1/place_order?" << content << " HTTP/1.1\r\n";
  stream << "Host: " << IP << ":" << to_string(PORT) << "\r\n";
  stream << "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
  stream << "Connection: close\r\n\r\n";
  stream << "Content-Length: 13\r\n";

  stream.flush();
  std::cout << stream.rdbuf();
}