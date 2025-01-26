#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "game.hpp"

/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */
/********************************-------Programme principal faisant tourner le jeu---------------------*************************************/
/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */


int main(){
    std::srand(std::time(nullptr));
    sf::RenderWindow window(sf::VideoMode({1200,800}),"Fenetre de test");

    window.setPosition({0,0});
    sf::View fixedView(sf::FloatRect({0, 0}, {1200, 800}));
    window.setView(fixedView);
    sf::RectangleShape rectangle({1200.f, 800.f});
    rectangle.setFillColor(sf::Color::White);
    ScreenGame gameboard(200,50,32);
    TetrisFigure fig(50,sf::Color::Red,0,0,500,400,3);
    while(window.isOpen()){

        while(std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }
            if (event->is<sf::Event::Resized>()) {
                window.setView(fixedView);
            }
            if(!gameboard.gameEnded()){
                if(const auto* DownKey = event->getIf<sf::Event::KeyReleased>()){
                    if((DownKey->scancode == sf::Keyboard::Scancode::Left) || (DownKey->scancode == sf::Keyboard::Scancode::Right) ||(DownKey->scancode == sf::Keyboard::Scancode::H)||(DownKey->scancode == sf::Keyboard::Scancode::J)){
                        gameboard.rotateOrTranslate(false);

                    }
                }
                if(const auto* UpKey = event->getIf<sf::Event::KeyPressed>()){
                    if(UpKey->scancode == sf::Keyboard::Scancode::Down){
                        gameboard.goDown();
                        gameboard.rotateOrTranslate(false);
                    }
                    else if((UpKey->scancode == sf::Keyboard::Scancode::Left) || (UpKey->scancode == sf::Keyboard::Scancode::Right) ||(UpKey->scancode == sf::Keyboard::Scancode::H)||(UpKey->scancode == sf::Keyboard::Scancode::J)){
                        
                        if(UpKey->scancode == sf::Keyboard::Scancode::Left){
                            gameboard.goLeft();
                            gameboard.rotateOrTranslate(true);
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::Right){
                            gameboard.goRight();
                            gameboard.rotateOrTranslate(true);
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::H){
                            gameboard.rotateLeft();
                            gameboard.rotateOrTranslate(true);
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::J){
                            gameboard.rotateRight();
                            gameboard.rotateOrTranslate(true);
                        }     
                    }


                }               
            }

        }
        if(!gameboard.gameEnded()){
            gameboard.autoGoDown();
        }

        window.clear();
        window.draw(rectangle);
        gameboard.draw(window);     
        window.display();
    }


    return 0;
}

