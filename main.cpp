#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "game.hpp"

int main(){
    std::srand(std::time(nullptr));
    sf::RenderWindow window(sf::VideoMode({1200,800}),"Fenetre de test",sf::Style::Default, sf::State::Fullscreen );
    sf::Vector2f windowSize = {window.getSize().x,window.getSize().y};
    sf::RectangleShape rectangle(windowSize);

    rectangle.setFillColor(sf::Color::White);
    ScreenGame gameboard((windowSize.x/3),windowSize.y/5,32);
    while(window.isOpen()){

        while(std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }

            if(!gameboard.gameBoard.gameEnded()){
                if(const auto* DownKey = event->getIf<sf::Event::KeyReleased>()){
                    if((DownKey->scancode == sf::Keyboard::Scancode::Left) || (DownKey->scancode == sf::Keyboard::Scancode::Right) ||(DownKey->scancode == sf::Keyboard::Scancode::H)||(DownKey->scancode == sf::Keyboard::Scancode::J)){
                        gameboard.gameBoard.rotateOrTranslate(false);

                    }
                }
                if(const auto* UpKey = event->getIf<sf::Event::KeyPressed>()){
                    if(UpKey->scancode == sf::Keyboard::Scancode::Down){
                        gameboard.gameBoard.goDown(gameboard.nextScreen);
                        gameboard.gameBoard.rotateOrTranslate(false);
                    }
                    else if((UpKey->scancode == sf::Keyboard::Scancode::Left) || (UpKey->scancode == sf::Keyboard::Scancode::Right) ||(UpKey->scancode == sf::Keyboard::Scancode::H)||(UpKey->scancode == sf::Keyboard::Scancode::J)){
                        
                        if(UpKey->scancode == sf::Keyboard::Scancode::Left){
                            gameboard.gameBoard.goLeft();
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::Right){
                            gameboard.gameBoard.goRight();
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::H){
                            gameboard.gameBoard.rotateLeft();
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::J){
                            gameboard.gameBoard.rotateRight();
                        }     
                    }


                }               
            }

        }
        if(!gameboard.gameBoard.gameEnded()){
            gameboard.gameBoard.autoGoDown(gameboard.nextScreen);
        }

        window.clear();
        window.draw(rectangle);
        gameboard.draw(window);     
        window.display();
    }


    return 0;
}

