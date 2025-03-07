#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "usefull.hpp"
#include "settings.hpp"


SettingsScreen::m_clickedOn SettingsScreen::detectedClick(sf::Vector2f mousePosition){
    sf::FloatRect _returnRectangle = m_returnRectangle.getGlobalBounds();
    sf::FloatRect _bindAudio = m_audioCircle.getGlobalBounds();
    sf::FloatRect _bindLeft = m_bindGoLeft.getGlobalBounds();
    sf::FloatRect _bindLeftRotate = m_bindLeftRotate.getGlobalBounds();
    sf::FloatRect _bindRight = m_bindGoRight.getGlobalBounds();
    sf::FloatRect _bindRightRotate = m_bindRightRotate.getGlobalBounds();
    sf::FloatRect _bindDown = m_bindGoDown.getGlobalBounds();
    sf::FloatRect _bindLevel = m_bindLevel.getGlobalBounds();

    // enum m_clickedOn {
    //     _return = 1,
    //     bindLeft = 2,
    //     bindRight = 3,
    //     bindDown = 4,
    //     bindRotateLeft = 5,
    //     bindRotateRight = 6,
    //     bindLevel = 7,
    //     bindAudio = 8,
    //     error = 0,
    // };
    
    if(_returnRectangle.contains(mousePosition)){
        std::cout << "return" << std::endl;
        return m_clickedOn::_return;
    }
    else if(_bindAudio.contains(mousePosition)){
        std::cout << "audioCircle" << std::endl;
        if(!m_audioCircleMoving){
            m_audioCircleMoving = true;
            m_audioCircleAbs = mousePosition.x;
        }

        return m_clickedOn::bindAudio;
    }
    else if(_bindLeft.contains(mousePosition)){
        std::cout << "bind left" << std::endl;
        launchNewBinding(m_clickedOn::bindLeft);
        return m_clickedOn::bindLeft;
    }
    else if(_bindRight.contains(mousePosition)){
        std::cout << "bind right " << std::endl;
        launchNewBinding(m_clickedOn::bindRight);
        return m_clickedOn::bindRight;
    }
    else if(_bindDown.contains(mousePosition)){
        std::cout << "bind down" << std::endl;
        launchNewBinding(m_clickedOn::bindDown);
        return m_clickedOn::bindDown;
    }
    else if(_bindLeftRotate.contains(mousePosition)){
        std::cout << "bind left rotate" << std::endl;
        launchNewBinding(m_clickedOn::bindLeftRotate);
        return m_clickedOn::bindLeftRotate;
    }
    else if(_bindRightRotate.contains(mousePosition)){
        std::cout << "bind right rotate " << std::endl;
        launchNewBinding(m_clickedOn::bindRightRotate);
        return m_clickedOn::bindRightRotate;
    }
    else if(_bindLevel.contains(mousePosition)){
        std::cout << "bind level" << std::endl;
        launchNewBinding(m_clickedOn::bindLevel);
        return m_clickedOn::bindLevel;
    }
    else{
        std::cout <<"error" << std::endl;
        return m_clickedOn::error;
    }
}

float SettingsScreen::modifyingAudio(sf::Vector2f mousePosition){
    if(m_audioCircleMoving==true){
        float newAbs = mousePosition.x;
        float delta = newAbs-m_audioCircleAbs;
        float audioCircleAbs = m_audioCircle.getPosition().x;
        float audioCircleOrd = m_audioCircle.getPosition().y;
        newAbs = delta + audioCircleAbs;
        float minAbs = m_audioLine.getPosition().x;
        float maxAbs = minAbs + m_audioLine.getSize().x;
        if( newAbs > maxAbs){
            newAbs = maxAbs;
        }
        else if(newAbs < minAbs){
            newAbs = minAbs;
        }
        m_audioCircle.setPosition({newAbs,audioCircleOrd});
        m_audioCircleAbs = -1;
        m_audioCircleMoving = false;
        return (newAbs-minAbs)/maxAbs; 
    }
    return 0; // SI on fait un if(modifyingAudio) ça proquera que si le cercle a bougé
}

SettingsScreen::SettingsScreen(sf::Vector2f windowSize,sf::Color backGroundColor, sf::Color fontColor, std::string fontFile): m_windowSize(windowSize),m_backGroundColor(backGroundColor),m_fontColor(fontColor){

    if(! m_font.openFromFile(fontFile)){
        std::cout << "Settings : fontFile erroné" << std::endl;
    }

    sf::Text m_goLeft(m_font);
    sf::Text m_goRight(m_font);
    sf::Text m_goDown(m_font);
    sf::Text m_rotateRight(m_font);
    sf::Text m_rotateLeft(m_font);
    sf::Text m_level(m_font);


    m_goLeft.setCharacterSize(15);
    m_goRight.setCharacterSize(15);
    m_goDown.setCharacterSize(15);
    m_rotateRight.setCharacterSize(15);
    m_rotateLeft.setCharacterSize(15);
    m_level.setCharacterSize(15);

    m_goLeft.setFillColor(m_fontColor);
    m_goRight.setFillColor(m_fontColor);
    m_goDown.setFillColor(m_fontColor);
    m_rotateRight.setFillColor(m_fontColor);
    m_rotateLeft.setFillColor(m_fontColor);
    m_level.setFillColor(m_fontColor);


    m_goLeft.setString("Left arrow");
    m_goRight.setString("Right arrow");
    m_goDown.setString("Down arrow");
    m_rotateRight.setString("H");
    m_rotateLeft.setString("J");
    m_level.setString("0");

    m_goLeft.setPosition({m_windowSize.x/6*5,m_windowSize.y/64*(15 + 1*3)});
    m_goRight.setPosition({m_windowSize.x/6*5,m_windowSize.y/64*(15 + 3*3)});
    m_goDown.setPosition({m_windowSize.x/6*5,m_windowSize.y/64*(15 + 5*3)});
    m_rotateRight.setPosition({m_windowSize.x/6*5,m_windowSize.y/64*(15 + 7*3)});
    m_rotateLeft.setPosition({m_windowSize.x/6*5,m_windowSize.y/64*(15 + 9*3)});
    m_level.setPosition({m_windowSize.x/6*5,m_windowSize.y/64*(15 + 11*3)});

    // chaque vide = 3/64 et idem chaque case + 1/32 entre return et la derniere case et + 1/64 entre bind et la première case


    m_bind.emplace_back(m_goLeft);
    m_bind.emplace_back(m_goRight);
    m_bind.emplace_back(m_goDown);
    m_bind.emplace_back(m_rotateRight);
    m_bind.emplace_back(m_rotateLeft);
    m_bind.emplace_back(m_level);

    sf::Text m_audioText(m_font);
    sf::Text m_settingsText(m_font);
    sf::Text m_bindText(m_font);
    sf::Text m_returnText(m_font);
    sf::Text m_toGoLeft(m_font);
    sf::Text m_toGoRight(m_font);
    sf::Text m_toGoDown(m_font);
    sf::Text m_toRotateRight(m_font);
    sf::Text m_toRotateLeft(m_font);
    sf::Text m_toLevel(m_font);

    m_audioText.setCharacterSize(50);
    m_settingsText.setCharacterSize(100);
    m_bindText.setCharacterSize(50);
    m_returnText.setCharacterSize(75);
    m_toGoLeft.setCharacterSize(20);
    m_toGoRight.setCharacterSize(20);
    m_toGoDown.setCharacterSize(20);
    m_toRotateRight.setCharacterSize(20);
    m_toRotateLeft.setCharacterSize(20);
    m_toLevel.setCharacterSize(20);


    m_audioText.setFillColor(m_fontColor);
    m_settingsText.setFillColor(m_fontColor);
    m_bindText.setFillColor(m_fontColor);
    m_returnText.setFillColor(m_fontColor);
    m_toGoLeft.setFillColor(m_fontColor);
    m_toGoRight.setFillColor(m_fontColor);
    m_toGoDown.setFillColor(m_fontColor);
    m_toRotateLeft.setFillColor(m_fontColor);
    m_toRotateRight.setFillColor(m_fontColor);
    m_toLevel.setFillColor(m_fontColor);

    m_audioText.setString("AUDIO");
    m_settingsText.setString("SETTING");
    m_bindText.setString("BINDS");
    m_returnText.setString("RETURN");
    m_toGoLeft.setString("GO LEFT : ");
    m_toGoRight.setString("GO RIGHT : ");
    m_toGoDown.setString("GO DOWN : ");
    m_toRotateLeft.setString("ROTATE LEFT : ");
    m_toRotateRight.setString("ROTATE RIGHT : ");
    m_toLevel.setString("LEVEL : ");


    m_settingsText.setPosition({m_windowSize.x/96*35,m_windowSize.y/64});
    m_returnText.setPosition({windowSize.x/12*5, windowSize.y/64 * 57});
    m_audioText.setPosition({m_windowSize.x/24*5,m_windowSize.y/32*5});
    m_bindText.setPosition({m_windowSize.x/48*35,m_windowSize.y/32*5});
    m_toGoLeft.setPosition({m_windowSize.x/6*4,m_windowSize.y/64*(15 + 1*3)});
    m_toGoRight.setPosition({m_windowSize.x/6*4,m_windowSize.y/64*(15 + 3*3)});
    m_toGoDown.setPosition({m_windowSize.x/6*4,m_windowSize.y/64*(15 + 5*3)});
    m_toRotateRight.setPosition({m_windowSize.x/6*4,m_windowSize.y/64*(15 + 7*3)});
    m_toRotateLeft.setPosition({m_windowSize.x/6*4,m_windowSize.y/64*(15 + 9*3)});
    m_toLevel.setPosition({m_windowSize.x/6*4,m_windowSize.y/64*(15 + 11*3)});


    m_text.emplace_back(m_toGoLeft);
    m_text.emplace_back(m_toGoRight);
    m_text.emplace_back(m_toGoDown);
    m_text.emplace_back(m_toRotateRight);
    m_text.emplace_back(m_toRotateLeft);
    m_text.emplace_back(m_toLevel);
    m_text.emplace_back(m_settingsText);
    m_text.emplace_back(m_returnText);
    m_text.emplace_back(m_audioText);
    m_text.emplace_back(m_bindText);


    sf::Text _m_newBindText(m_font);
    sf::Text m_newText(m_font);
    
    _m_newBindText.setCharacterSize(70);
    m_newText.setCharacterSize(50);

    _m_newBindText.setFillColor(m_fontColor);
    m_newText.setFillColor(m_fontColor);

    _m_newBindText.setPosition({m_windowSize.x/6,m_windowSize.y/4});
    m_newText.setPosition({m_windowSize.x/12*5,m_windowSize.y/2});

    m_newBindText.emplace_back(_m_newBindText);
    m_newBindText.emplace_back(m_newText);

    m_backGroundRectangle.setSize(windowSize);
    m_backGroundRectangle.setFillColor(backGroundColor);

    m_returnRectangle.setSize({m_windowSize.x/3,m_windowSize.y/16});
    m_returnRectangle.setFillColor(m_backGroundColor);
    m_returnRectangle.setOutlineThickness(10.f);
    m_returnRectangle.setOutlineColor(m_fontColor);
    m_returnRectangle.setPosition({m_windowSize.x/3, m_windowSize.y/32 * 29});

    m_newBindRectangle.setSize({m_windowSize.x/4*3 , m_windowSize.y/4*3});
    m_newBindRectangle.setFillColor(m_backGroundColor);
    m_newBindRectangle.setOutlineThickness(10.f);
    m_newBindRectangle.setOutlineColor(m_fontColor);
    m_newBindRectangle.setPosition({m_windowSize.x/8,m_windowSize.y/8});

    m_audioCircle.setRadius(50.f);
    m_audioCircle.setOrigin({m_audioCircle.getRadius(), m_audioCircle.getRadius()});
    m_audioCircle.setFillColor(m_fontColor);
    m_audioCircle.setPosition({m_windowSize.x/4,m_windowSize.y/2});

    m_audioLine.setSize({m_windowSize.x/2 - m_windowSize.x/32,m_windowSize.y/100});
    m_audioLine.setFillColor(m_fontColor);
    m_audioLine.setPosition({m_windowSize.x/64 ,m_windowSize.y/200 * 99});

    m_verticalLine.setSize({m_windowSize.x/500,m_windowSize.y/4*3});
    m_verticalLine.setFillColor(m_fontColor);
    m_verticalLine.setPosition({m_windowSize.x*99/200,m_windowSize.y*110/800});

    m_bindGoLeft.setSize({m_windowSize.x/12,m_windowSize.y/32});
    m_bindGoLeft.setFillColor(m_backGroundColor);
    m_bindGoLeft.setOutlineThickness(1.f);
    m_bindGoLeft.setOutlineColor(m_fontColor);
    m_bindGoLeft.setPosition({m_windowSize.x/6*5 - m_windowSize.x/64,m_windowSize.y/64*(15 + 1*3) - m_windowSize.y/128});


    m_bindGoRight.setSize({m_windowSize.x/12,m_windowSize.y/32});
    m_bindGoRight.setFillColor(m_backGroundColor);
    m_bindGoRight.setOutlineThickness(1.f);
    m_bindGoRight.setOutlineColor(m_fontColor);
    m_bindGoRight.setPosition({m_windowSize.x/6*5 - m_windowSize.x/64,m_windowSize.y/64*(15 + 3*3) - m_windowSize.y/128});

    m_bindGoDown.setSize({m_windowSize.x/12,m_windowSize.y/32});
    m_bindGoDown.setFillColor(m_backGroundColor);
    m_bindGoDown.setOutlineThickness(1.f);
    m_bindGoDown.setOutlineColor(m_fontColor);
    m_bindGoDown.setPosition({m_windowSize.x/6*5 - m_windowSize.x/64,m_windowSize.y/64*(15 + 5*3) - m_windowSize.y/128});

    m_bindLeftRotate.setSize({m_windowSize.x/12,m_windowSize.y/32});
    m_bindLeftRotate.setFillColor(m_backGroundColor);
    m_bindLeftRotate.setOutlineThickness(1.f);
    m_bindLeftRotate.setOutlineColor(m_fontColor);
    m_bindLeftRotate.setPosition({m_windowSize.x/6*5 - m_windowSize.x/64,m_windowSize.y/64*(15 + 7*3) - m_windowSize.y/128});

    m_bindRightRotate.setSize({m_windowSize.x/12,m_windowSize.y/32});
    m_bindRightRotate.setFillColor(m_backGroundColor);
    m_bindRightRotate.setOutlineThickness(1.f);
    m_bindRightRotate.setOutlineColor(m_fontColor);
    m_bindRightRotate.setPosition({m_windowSize.x/6*5 - m_windowSize.x/64,m_windowSize.y/64*(15 + 9*3) - m_windowSize.y/128});
    
    
    m_bindLevel.setSize({m_windowSize.x/12,m_windowSize.y/32});
    m_bindLevel.setFillColor(m_backGroundColor);
    m_bindLevel.setOutlineThickness(1.f);
    m_bindLevel.setOutlineColor(m_fontColor);
    m_bindLevel.setPosition({m_windowSize.x/6*5 - m_windowSize.x/64,m_windowSize.y/64*(15 + 11*3) - m_windowSize.y/128});
    


    // y/16 * x/16 sera la taille des carrés du bind 
    // titre x/3 ; y/16 et la taille sera x/3 ; y/8 
    // Audio sera positionné à x/8 et y/4 taille sera x/4 et y/16
    // Binds sera positionné à x/8 + x/2 et y/4 taille sera x/4 et y /16 
    // barre sera x/16 à x/2-x/16 et y sera y/4 + y/8 largeur sera y/16 
    // circle on verra mais taille devrait être x/16 (rayon) et son centre sera borné sur la barre
    // En dessous on mettra le pourcentage;
    //return sera en y - y/16 - y/8 et taille y/8

    // Distribution de la page 1/8 pour le header 
    // 1/8 pour le bottom
    // 3/4 pour le reste
    // A gauche audio avec une barre et un cercle et en dessous le pourcentage lié au son

    // A droite Binds avec le nom des touches et les touches bindées
    // au milieu une barre droite fine

    // Fonction pour bind
    // Fonction pour déplacer le cercle sur la barre et mettre à jour l'audio
    
}


bool SettingsScreen::isBinding() const { 
    return m_newBind;
}


void SettingsScreen::newBindReceived(sf::Keyboard::Scancode newKey){
    if((int)m_currentBind == 5){
        if(isANumber(newKey)){
            m_ScanCode[(int)m_currentBind] = newKey;
            m_bind[(int)m_currentBind].setString(ScanToString[static_cast<size_t>(newKey)]);
            m_newBindText[1].setString(ScanToString[static_cast<size_t>(newKey)]);     
        }
        // truc special pour level (il faut vérifier que c'est entre 0 et 15)
    }
    else{
        m_ScanCode[(int)m_currentBind] = newKey;
        m_bind[(int)m_currentBind].setString(ScanToString[static_cast<size_t>(newKey)]);
        m_newBindText[1].setString(ScanToString[static_cast<size_t>(newKey)]);

    }
}


void SettingsScreen::endBinding(){
    m_newBind = false;
}


void SettingsScreen::launchNewBinding(SettingsScreen::m_clickedOn bind){
    m_newBind = true;
    m_currentBind = bind;
    sf::Text *currentBind = &m_bind[(int)bind];
    sf::Text *textBind = &m_text[(int)bind];
    m_newBindText[0].setString("new bind for : " + textBind->getString());
    m_newBindText[1].setString(currentBind->getString());
}



void SettingsScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform

    //std::cout << "SettingsScreen::draw " << isBinding() << std::endl;
    states.transform *= getTransform();

    // draw the vertex array
    if(isBinding()){
        target.draw(m_newBindRectangle,states);
        for(int i =0 ; i<m_newBindText.size();i++){
            target.draw(m_newBindText[i],states);
        }
    }
    else{
        target.draw(m_backGroundRectangle, states);
        target.draw(m_audioCircle, states);
        target.draw(m_audioLine, states);
        target.draw(m_verticalLine, states);
        target.draw(m_returnRectangle, states);
        target.draw(m_bindGoLeft,states);
        target.draw(m_bindGoRight,states);
        target.draw(m_bindGoDown,states);
        target.draw(m_bindLeftRotate,states);
        target.draw(m_bindRightRotate,states);
        target.draw(m_bindLevel,states);
    
        for(int i=0 ; i<m_text.size();i++){
             target.draw(m_text[i],states);
        }
    
        for(int i=0; i<m_bind.size();i++){
            target.draw(m_bind[i],states);
        }
    }

}

int main(){
    sf::RenderWindow window(sf::VideoMode({1200,800}),"Fenetre des settings",sf::Style::Default, sf::State::Fullscreen );
    sf::Vector2f windowSize = sf::Vector2f(window.getSize().x, window.getSize().y);
    SettingsScreen settings(windowSize);
    while(window.isOpen()){
        
        while(std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }   
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();
                else if(settings.isBinding()){
                    if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                        if(keyPressed->scancode != sf::Keyboard::Scancode::Enter){
                            settings.newBindReceived(keyPressed->scancode);
                        }
                        else if(keyPressed->scancode == sf::Keyboard::Scancode::Enter){
                            settings.endBinding();
                        }
                    }
                }
            }
            else if(!settings.isBinding()){
                if(const auto* MouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                    if(MouseButtonPressed->button == sf::Mouse::Button::Left){
                        sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition().x ,  sf::Mouse::getPosition().y);
                        settings.detectedClick(position);
                    }
                }

                else if(const auto* MouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()){
                    if (MouseButtonReleased->button == sf::Mouse::Button::Left){
                        sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition().x ,  sf::Mouse::getPosition().y);
                        settings.modifyingAudio(position);
                    }
                
                }
            }

        }

        window.clear();
        window.draw(settings);
        window.display();
    }
    return 0;
}