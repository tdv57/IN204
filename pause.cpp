#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "usefull.hpp"
#include "pause.hpp"



PauseScreen::PauseScreen(sf::Vector2f windowSize,sf::Color backGroundColor, sf::Color fontColor, std::string fontFile):m_windowSize(windowSize),m_backGroundColor(backGroundColor),m_fontColor(fontColor){
        


    m_backGroundRectangle.setSize(windowSize);
    m_backGroundRectangle.setFillColor(backGroundColor);
    


    m_keepPlayingRectangle.setSize({windowSize.x/3,0.12*windowSize.y});
    m_keepPlayingRectangle.setFillColor(backGroundColor);
    m_keepPlayingRectangle.setOutlineThickness(10.f);
    m_keepPlayingRectangle.setOutlineColor(fontColor);
    m_keepPlayingRectangle.setPosition({windowSize.x/3,(0.05*1+0.12*1+0.1)*windowSize.y});

    m_restartRectangle.setSize({windowSize.x/3,0.12*windowSize.y});
    m_restartRectangle.setFillColor(backGroundColor);
    m_restartRectangle.setOutlineThickness(10.f);
    m_restartRectangle.setOutlineColor(fontColor);
    m_restartRectangle.setPosition({windowSize.x/3,(0.05*2+0.12*2+0.1)*windowSize.y});
     
    m_settingsRectangle.setSize({windowSize.x/3,0.12*windowSize.y});
    m_settingsRectangle.setFillColor(backGroundColor);
    m_settingsRectangle.setOutlineThickness(10.f);
    m_settingsRectangle.setOutlineColor(fontColor);
    m_settingsRectangle.setPosition({windowSize.x/3,(0.05*3+0.12*3+0.1)*windowSize.y});

    m_quitRectangle.setSize({windowSize.x/3,0.12*windowSize.y});
    m_quitRectangle.setFillColor(backGroundColor);
    m_quitRectangle.setOutlineThickness(10.f);
    m_quitRectangle.setOutlineColor(fontColor);
    m_quitRectangle.setPosition({windowSize.x/3,(0.05*4+0.12*4+0.1)*windowSize.y});
     

    if(! m_font.openFromFile(fontFile)){
        std::cout << "Pausescreen : FontFile erroné" << std::endl;
    }


    sf::Text m_pauseText(m_font); 
    sf::Text m_quitText(m_font);
    sf::Text m_settingsText(m_font);    
    sf::Text m_restartText(m_font);   
    sf::Text m_keepPlayingText(m_font); 

    m_pauseText.setFillColor(fontColor);
    m_quitText.setFillColor(fontColor);
    m_settingsText.setFillColor(fontColor);
    m_restartText.setFillColor(fontColor);
    m_keepPlayingText.setFillColor(fontColor);

    m_pauseText.setCharacterSize(100);
    m_quitText.setCharacterSize(50);
    m_settingsText.setCharacterSize(50);
    m_restartText.setCharacterSize(50);
    m_keepPlayingText.setCharacterSize(50);

    m_pauseText.setPosition({(windowSize.x/3+windowSize.x/12),(0.1)*windowSize.y});
    m_keepPlayingText.setPosition({(windowSize.x/3+windowSize.x/10), (0.1+0.03+1*0.12+1*0.05)*windowSize.y});
    m_restartText.setPosition({windowSize.x/3+windowSize.x/9,(0.1+0.03+2*0.12+2*0.05)*windowSize.y});
    m_settingsText.setPosition({windowSize.x/3+windowSize.x/10, (0.1+0.03+3*0.12+3*0.05)*windowSize.y});
    m_quitText.setPosition({windowSize.x/3+windowSize.x/9, (0.1+0.03+4*0.12+4*0.05)*windowSize.y});

    m_pauseText.setString("PAUSE");
    m_keepPlayingText.setString("continue");
    m_restartText.setString("restart");
    m_settingsText.setString("settings");
    m_quitText.setString("quitter");


    m_text.emplace_back(m_pauseText);
    m_text.emplace_back(m_quitText);
    m_text.emplace_back(m_settingsText);
    m_text.emplace_back(m_restartText);
    m_text.emplace_back(m_keepPlayingText);
}


PauseScreen::m_clickedOn PauseScreen::detectedClick(sf::Vector2f mousePosition){
    sf::FloatRect boundingRestart = m_restartRectangle.getGlobalBounds();
    sf::FloatRect boundingQuit = m_quitRectangle.getGlobalBounds();
    sf::FloatRect boundingKeepPlaying = m_keepPlayingRectangle.getGlobalBounds();
    sf::FloatRect boundingSettings = m_settingsRectangle.getGlobalBounds();
    if(boundingKeepPlaying.contains(mousePosition)){
        std::cout << "continue" << std::endl;
        return m_clickedOn::keepPlaying;
    }
    if(boundingRestart.contains(mousePosition)){
        std::cout << "restart" << std::endl;
        return m_clickedOn::restart;
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


void PauseScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // draw the vertex array
    target.draw(m_backGroundRectangle, states);
    target.draw(m_restartRectangle, states);
    target.draw(m_keepPlayingRectangle, states);
    target.draw(m_quitRectangle, states);
    target.draw(m_settingsRectangle, states);

    for(int i=0 ; i<m_text.size();i++){
         target.draw(m_text[i],states);
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode({1200,800}),"Fenetre de Pause",sf::Style::Default, sf::State::Fullscreen );
    sf::Vector2f windowSize = sf::Vector2f(window.getSize().x, window.getSize().y);
    PauseScreen pauseScreen(windowSize);
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
                pauseScreen.detectedClick(position);
            }
        }

        window.clear();
        window.draw(pauseScreen);
        window.display();
    }
    return 0;
}