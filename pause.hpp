#ifndef PAUSEHPP
#define PAUSEHPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "usefull.hpp"

class PauseScreen: public sf::Drawable, public sf::Transformable {
    enum class m_clickedOn {
        keepPlaying = 1,
        restart = 2,
        settings = 3,
        quit = 4,
        error = 0
    };
public:

    
    
    PauseScreen(sf::Vector2f windowSize,sf::Color backGroundColor = sf::Color::Black, sf::Color fontColor = sf::Color::White, std::string fontFile = "font/MegamaxJonathanToo-YqOq2.ttf");
// Rectangle du pause
// Rectangle du reprendre
// Rectangle du Quitter
// Rectangle du Restart
// Rectangle des settings
// Grand REctangle de background

// On laisse un espace de 1/10 en haut et en bas
// puis sur les 80% restant on fait 4 sauts et 5 prints
// chaque saut fera 5% et 1 Print fera 12% de la page 

    m_clickedOn detectedClick(sf::Vector2f mousePosition);

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Font m_font;
    sf::Color m_backGroundColor;
    sf::Color m_fontColor;
    sf::Vector2f m_windowSize; 
    sf::RectangleShape m_backGroundRectangle;
    sf::RectangleShape m_restartRectangle;
    sf::RectangleShape m_keepPlayingRectangle;
    sf::RectangleShape m_quitRectangle;
    sf::RectangleShape m_settingsRectangle;
    std::vector<sf::Text> m_text;

    
};





#endif