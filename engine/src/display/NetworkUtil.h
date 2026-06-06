#pragma once

#include <SFML/Network.hpp>

namespace TDEngine::Inner {

/** Binds TcpListener; on Linux uses SO_REUSEADDR so the port can be reused immediately after stopNetwork(). */
bool listenTcpReuse(sf::TcpListener &listener, unsigned short port);

} // namespace TDEngine::Inner
