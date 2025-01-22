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

    sf::RectangleShape rectangle({1200.f, 800.f});
    rectangle.setFillColor(sf::Color::White);
    ScreenGame gameboard(200,50,32);
    TetrisFigure fig(50,sf::Color::Red,0,0,500,400,3);
    //window.setFramerateLimit(60);   
    while(window.isOpen()){

        while(std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }
            if(!gameboard.gameEnded()){
                if(const auto* UpKey = event->getIf<sf::Event::KeyPressed>()){
                    if(UpKey->scancode == sf::Keyboard::Scancode::Down){
                        gameboard.goDown();
                        //fig.goDown();
                    }
                    if(UpKey->scancode == sf::Keyboard::Scancode::Left){
                        gameboard.goLeft();
                        //fig.goLeft();
                    }
                    if(UpKey->scancode == sf::Keyboard::Scancode::Right){
                        gameboard.goRight();
                        //fig.goRight();
                    }
                    if(UpKey->scancode == sf::Keyboard::Scancode::H){
                        gameboard.rotateLeft();
                        // fig.rotateLeft();
                    }
                    if(UpKey->scancode == sf::Keyboard::Scancode::J){
                        gameboard.rotateRight();
                        // fig.rotateRight();
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
        //fig.draw(window);
        window.display();
    }


    return 0;
}

