#ifndef SETTINGSHPP
#define SETTINGSHPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "usefull.hpp"


class SettingsScreen: public sf::Drawable, public sf::Transformable{

public:
    enum class m_clickedOn {
        
        bindLeft = 0,
        bindRight = 1,
        bindDown = 2,
        bindRightRotate = 3,
        bindLeftRotate = 4,
        bindLevel = 5,
        bindAudio = 6,
        _return = 7,
        error = 8,
    };
// Objectif c'est d'avoir une barre audio c'est ici qu'on va gérer l'audio du jeu;
// il faut un partie pour bind les touches
// il faut un rectangle en bas pour revenir aux settings
// Quand on bind on veut faire apparaitre un rectangle demandant la nouvelle touche assignée
// On va pouvoir également modifier le niveau actuel
public:

m_clickedOn detectedClick(sf::Vector2f mousePosition);

float modifyingAudio(sf::Vector2f mousePosition);

SettingsScreen(sf::Vector2f windowSize,sf::Color backGroundColor = sf::Color::Black, sf::Color fontColor = sf::Color::White, std::string fontFile = "font/MegamaxJonathanToo-YqOq2.ttf");

bool isBinding() const;

void newBindReceived(sf::Keyboard::Scancode newKey);

void endBinding();


private:

void launchNewBinding(m_clickedOn bind);

void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

sf::RectangleShape m_return;
sf::RectangleShape m_audioLine;
sf::CircleShape m_audioCircle;
sf::RectangleShape m_verticalLine;
sf::RectangleShape m_backGroundRectangle;
sf::RectangleShape m_returnRectangle;

sf::Color m_backGroundColor;
sf::Color m_fontColor;
sf::Font m_font;
sf::Vector2f m_windowSize;

sf::RectangleShape m_bindLeftRotate;
sf::RectangleShape m_bindRightRotate;
sf::RectangleShape m_bindGoLeft;
sf::RectangleShape m_bindGoRight;
sf::RectangleShape m_bindGoDown;
sf::RectangleShape m_bindLevel;

sf::RectangleShape m_newBindRectangle;

std::vector<sf::Text> m_text;  // Stock les titres
std::vector<sf::Text> m_bind;  // Stock les binds actuels (peuvent changer)
std::vector<sf::Text> m_newBindText; // (Pour l'écran de la nouvelle touche à bind + la touche rentrée);
std::vector<sf::Keyboard::Scancode> m_ScanCode = {sf::Keyboard::Scancode::Left , sf::Keyboard::Scancode::Right , sf::Keyboard::Scancode::Down , sf::Keyboard::Scancode::J , sf::Keyboard::Scancode::H , sf::Keyboard::Scancode::Unknown };

bool m_audioCircleMoving = false;
float m_audioCircleAbs = -1;
bool m_newBind = false;
enum m_clickedOn m_currentBind;


};





#endif