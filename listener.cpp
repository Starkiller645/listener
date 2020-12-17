#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <errno.h>
#include <fmt/color.h>

using namespace fmt;

int main(int argc, char *argv[]) {
int sockfd, newsockfd, portno;
socklen_t clilen;
char buffer[500];
struct sockaddr_in serv_addr, cli_addr;
int n;
  while(true) {
    sleep(2);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
      bzero((char *) &serv_addr, sizeof(serv_addr));
      std::string portno_str = argv[1];
      print(fg(color::floral_white) | emphasis::bold, "Ready...                        ");
      std::cout << "\r" << std::flush;
      portno = std::stoi(portno_str);
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = INADDR_ANY;
      serv_addr.sin_port = htons(portno);
      if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "ERROR while binding socket\n";
        std::cout << std::to_string(errno) << std::endl;
        continue;
      };
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  int reuse = 1;
  if(setsockopt(newsockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) > 0) {
    std::cout << "ERROR while setting socket options. Program will now exit" << std::endl;
    return 1;
  }
  if(newsockfd < 0) {
    std::cout << "ERROR on connection accept\n";
  };
  bzero(buffer, 500);
  n = read(newsockfd, buffer, 499);
  if(n < 0) {
    std::cout << "ERROR while read from socket\n";
  };
  if(n < 0) {
    std::cout << "ERROR while write to socket\n";
  }
  std::string headers = buffer;
  headers = headers.substr(0, headers.find("\r\n\r\n"));
  print(fg(color::floral_white) | emphasis::bold, "---------------------------------\n");
  print(fg(color::steel_blue) | emphasis::bold, "HTTP Headers Received:\n\n");
  std::vector<std::string> head_arr = {};
  std::vector<std::string> head_types = {};
  std::vector<std::string> head_vals = {};
  std::string token;
  size_t pos = 0;
  std::string request = headers.substr(0, headers.find("\r\n"));
  if(request.find("GET") != std::string::npos) {
    print(fg(color::light_sea_green) | emphasis::bold, request + "\n");
  } else if(request.find("POST") != std::string::npos) {
    print(fg(color::light_cyan) | emphasis::bold, request + "\n");
  };
  while((pos = headers.find("\r\n")) != std::string::npos) {
    token = headers.substr(0, pos);
    head_arr.push_back(token);
    headers.erase(0, pos + 2);
  }
  int size_vect = static_cast<int>(head_arr.size());
  for(int i = 1; i < size_vect; i++) {
    int dlpos = head_arr[i].find(": ");
    head_types.push_back(head_arr[i].substr(0, dlpos));
    head_vals.push_back(head_arr[i].substr(dlpos));
    print(fg(color::light_sky_blue) | emphasis::bold, head_types[i - 1] + " ");
    print(fg(color::floral_white), head_vals[i - 1] + "\n");
  }
  std::string body = buffer;
  body = body.substr(body.find("\r\n\r\n"));
  print(fg(color::steel_blue) | emphasis::bold, "\nHTTP Body:");
  print(fg(color::floral_white), body + "\n\n");
  print(fg(color::steel_blue) | emphasis::bold, "Sent HTTP Response:\n\n");
  if(request.find("GET") != std::string::npos) {
    char responsebuffer[50] = "HTTP/1.1 418 I'm a teapot\r\nConnection: close\r\n";
    write(newsockfd, responsebuffer, 50);
    print(fg(color::salmon) | emphasis::bold, "HTTP/1.1 418 I'm a teapot\n");
    print(fg(color::floral_white), "Connection: close\n\n\n");
  } else if(request.find("POST") != std::string::npos) {
    char responsebuffer[50] = "HTTP/1.1 200 OK\r\nConnection: close\r\n";
    write(newsockfd, responsebuffer, 50);
    print(fg(color::light_green) | emphasis::bold, "HTTP/1.1 200 OK\n");
    print(fg(color::floral_white), "Connection: close\n\n\n");
  } else {
    char responsebuffer[50] = "HTTP/1.1 200 OK\r\nConnection: close\r\n";
    write(newsockfd, responsebuffer, 50);
  }
  print(fg(color::floral_white) | emphasis::bold, "---------------------------------\n");
  close(newsockfd);
  close(sockfd);
  print(fg(color::salmon), "Waiting for socket read-ready...");
  std::cout << "\r" << std::flush;
}
  return 0;
}
