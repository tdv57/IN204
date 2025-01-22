#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "game.hpp"

sf::Vector2i operator * (const float& side,const sf::Vector2i v);

sf::Vector2f operator * (const float& side,const sf::Vector2f v);

sf::Vector2i operator * (const sf::Vector2i v,const float& side);

sf::Vector2f operator * (const sf::Vector2f v,const float& side);