//
// Created by Mikle on 14.12.2025.
//

#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace TDEngine {
	namespace Inner {
		class TextureCache {
		private:
			std::unordered_map<std::string, sf::Texture> cache;

		public:
			sf::Texture& getTexture(const std::string& path) {
				if (cache.find(path) == cache.end()) {
					sf::Texture tex;
					if (!tex.loadFromFile(path)) {
						std::cerr << "Error loading texture: " << path << std::endl;
					}
					cache[path] = tex;
				}
				return cache[path];
			}
		};
	}
}



#endif //TEXTURECACHE_H
