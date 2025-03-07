
#ifndef WELCOMEHPP
#define WELCOMEHPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "usefull.hpp"


class WelcomeScreen: public sf::Drawable, public sf::Transformable{


    enum class m_clickedOn {
        solo = 1,
        oneVone = 2,
        settings = 3,
        quit = 4,
        error = 0
    };
    // 3 onglets --> 1 V 1 // SOLO / SETTINGS 
    // Il faut un jolie titre
    // LA page doit être meilleur que les autres en terme esthétique
    // Surtout une page esthétique

public : 

    WelcomeScreen(sf::Vector2f windowSize,sf::Color backGroundColor = sf::Color::Black, sf::Color fontColor = sf::Color::Magenta,std::string backGroundFile = "image/tetris_back_clean.jpg",std::string titleFile = "image/tetris-game-icon.png",std::string fontFile = "font/MegamaxJonathanToo-YqOq2.ttf");


    m_clickedOn detectedClick(sf::Vector2f mousePosition);

private:

void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

sf::Color m_backGroundColor;
sf::Color m_fontColor;
sf::Vector2f m_windowSize;
sf::Font m_font;
sf::Texture m_backGroundTexture;
sf::Texture m_titleTexture;
sf::RectangleShape m_settingsRectangle;
sf::RectangleShape m_soloRectangle;
sf::RectangleShape m_oneVoneRectangle;
sf::RectangleShape m_quitRectangle;
std::vector<sf::Sprite> m_textureArray;
std::vector<sf::Text> m_textArray;
};

#endif