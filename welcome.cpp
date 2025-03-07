#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "usefull.hpp"
#include "welcome.hpp"


WelcomeScreen::WelcomeScreen(sf::Vector2f windowSize,sf::Color backGroundColor, sf::Color fontColor,std::string backGroundFile,std::string titleFile,std::string fontFile):m_windowSize(windowSize),m_font(fontFile),m_fontColor(fontColor),m_backGroundColor(backGroundColor){
    
    
    if(m_backGroundTexture.loadFromFile(backGroundFile,false,sf::IntRect({0,0},{m_windowSize.x,m_windowSize.y}))){

    }
    m_backGroundTexture.setRepeated(true);
    sf::Sprite m_backGround1(m_backGroundTexture);
    sf::Sprite m_backGround2(m_backGroundTexture);
    m_backGround2.move({1000,0});
    m_textureArray.emplace_back(m_backGround1);
    m_textureArray.emplace_back(m_backGround2);


    if(m_titleTexture.loadFromFile(titleFile,false,sf::IntRect({0,0},{m_windowSize.x,m_windowSize.y}))){

    }
    sf::Sprite m_title(m_titleTexture);
    m_title.move({m_windowSize.x/2 - 275,0});
    m_textureArray.emplace_back(m_title);

    m_soloRectangle.setSize({m_windowSize.x/3,m_windowSize.y/16});
    m_soloRectangle.setFillColor(backGroundColor);
    m_soloRectangle.setPosition({m_windowSize.x/3,m_windowSize.y/2});

    m_oneVoneRectangle.setSize({m_windowSize.x/3,m_windowSize.y/16});
    m_oneVoneRectangle.setFillColor(backGroundColor);
    m_oneVoneRectangle.setPosition({m_windowSize.x/3,m_windowSize.y/2+m_windowSize.y/8});

    m_settingsRectangle.setSize({m_windowSize.x/3,m_windowSize.y/16});
    m_settingsRectangle.setFillColor(backGroundColor);
    m_settingsRectangle.setPosition({m_windowSize.x/3,m_windowSize.y/2+m_windowSize.y/4});

    m_quitRectangle.setSize({m_windowSize.x/3,m_windowSize.y/16});
    m_quitRectangle.setFillColor(backGroundColor);
    m_quitRectangle.setPosition({m_windowSize.x/3,m_windowSize.y/2+m_windowSize.y*6/16});


    if(! m_font.openFromFile(fontFile)){
        std::cout << "Pausescreen : FontFile erroné" << std::endl;
    }

    sf::Text m_solo(m_font);
    sf::Text m_oneVone(m_font);
    sf::Text m_settings(m_font);
    sf::Text m_quit(m_font);

    m_solo.setFillColor(m_fontColor);
    m_oneVone.setFillColor(fontColor);
    m_settings.setFillColor(m_fontColor);
    m_quit.setFillColor(m_fontColor);

    m_solo.setCharacterSize(50);
    m_oneVone.setCharacterSize(50);
    m_settings.setCharacterSize(50);
    m_quit.setCharacterSize(50);

    m_solo.setPosition({m_windowSize.x/3+m_windowSize.x/8,m_windowSize.y/2});
    m_oneVone.setPosition({m_windowSize.x/3+m_windowSize.x/12,m_windowSize.y/2+m_windowSize.y/8});
    m_settings.setPosition({m_windowSize.x/3+m_windowSize.x/11,m_windowSize.y/2+m_windowSize.y/4});
    m_quit.setPosition({m_windowSize.x/3+m_windowSize.x/8,m_windowSize.y/2+m_windowSize.y*6/16});

    m_solo.setString("SOLO");
    m_oneVone.setString("ONE V ONE");
    m_settings.setString("SETTINGS");
    m_quit.setString("QUIT");

    m_textArray.emplace_back(m_solo);
    m_textArray.emplace_back(m_oneVone);
    m_textArray.emplace_back(m_settings);
    m_textArray.emplace_back(m_quit);

}


WelcomeScreen::m_clickedOn WelcomeScreen::detectedClick(sf::Vector2f mousePosition){
    sf::FloatRect boundingSolo = m_soloRectangle.getGlobalBounds();
    sf::FloatRect boundingOneVone = m_oneVoneRectangle.getGlobalBounds();
    sf::FloatRect boundingSettings = m_settingsRectangle.getGlobalBounds();
    sf::FloatRect boundingQuit = m_quitRectangle.getGlobalBounds();
    if(boundingSolo.contains(mousePosition)){
        std::cout << "continue" << std::endl;
        return m_clickedOn::solo;
    }
    if(boundingOneVone.contains(mousePosition)){
        std::cout << "restart" << std::endl;
        return m_clickedOn::oneVone;
    }
    if(boundingSettings.contains(mousePosition)){
        std::cout << "settings" << std::endl;
        return m_clickedOn::settings;
    }
    if(boundingQuit.contains(mousePosition)){
        std::cout << "quit" << std::endl;
        return m_clickedOn::quit;
    }
    else{
        std::cout << "error" << std::endl;
        return m_clickedOn::error;
    }
} 

void WelcomeScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= getTransform();

    target.draw(m_textureArray[0],states);
    target.draw(m_textureArray[1],states);
    target.draw(m_textureArray[2],states);
    target.draw(m_soloRectangle,states);
    target.draw(m_oneVoneRectangle,states);
    target.draw(m_settingsRectangle,states);
    target.draw(m_quitRectangle,states);
    for(int i=0 ; i<m_textArray.size();i++){
        target.draw(m_textArray[i],states);
    }

}


int main(){
    sf::RenderWindow window(sf::VideoMode({1200,800}),"Fenetre de Pause",sf::Style::Default, sf::State::Fullscreen );
    sf::Vector2f windowSize = sf::Vector2f(window.getSize().x, window.getSize().y);
    WelcomeScreen welcomeScreen(windowSize);
    while(window.isOpen()){
        
        while(std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }   
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition().x ,  sf::Mouse::getPosition().y);
                welcomeScreen.detectedClick(position);
            }
    
        }

        window.clear();
        window.draw(welcomeScreen);
        window.display();
    }

}