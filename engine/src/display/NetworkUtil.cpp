#include "NetworkUtil.h"

#include <chrono>
#include <thread>

#if !defined(_WIN32)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace TDEngine::Inner {

namespace {

#if !defined(_WIN32)
bool probePortReusable(unsigned short port) {
	const int fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		return false;
	}

	int yes = 1;
	::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	const bool ok = ::bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == 0;
	::close(fd);
	return ok;
}
#endif

} // namespace

bool listenTcpReuse(sf::TcpListener &listener, unsigned short port) {
	listener.close();

	if (listener.listen(port) == sf::Socket::Done) {
		return true;
	}

	listener.close();
	std::this_thread::sleep_for(std::chrono::milliseconds(150));

	if (listener.listen(port) == sf::Socket::Done) {
		return true;
	}

#if !defined(_WIN32)
	// Warm up: if the kernel still holds the port, a reuse bind probe tells us to wait once more.
	if (!probePortReusable(port)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	listener.close();
	return listener.listen(port) == sf::Socket::Done;
#else
	listener.close();
	return false;
#endif
}

} // namespace TDEngine::Inner
