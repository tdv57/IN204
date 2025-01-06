#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>

class figure : sf::Drawable , sf::Transformable {


public:
    figure(unsigned int nbcarre,sf::Color color,sf::VertexArray carre):nbCarre(nbcarre),Color(color),LeCarre(carre){
        // Verifier que tout va bien
        // Important on doit avoir 2*nbcarres
        // On ne peut pas avoir des carrés isolés
    }


    // Une figure c'est plusieurs carrés qu'on va générer avec des vertex et des triangles
    // C'est également une abscisse et une ordonnée pour chaque carré
protected:
    unsigned int nbCarre;
    sf::Color Color;
    sf::VertexArray LeCarre;
    // On va créer pour chaque classe fille une initialisation
    
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // apply the entity's transform -- combine it with the one that was passed by the caller
        states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

        // apply the texture

        // you may also override states.shader or states.blendMode if you want

        // draw the vertex array
        target.draw(LeCarre, states);
    }
};

class fig1 : public figure{
    //fig1() : figure(4,sf::Color::Red,)
};
class tableau{
    // 22 longueurs 10 largeurs // Il faut des cases en plus invisibles pour charger l'objet

private:
    // ON va lui mettre un std::vector de figure (des pointeurs) 

};

class generator{
 // Le générateur de figure 
};

int main(){
    sf::RenderWindow window(sf::VideoMode({1200,800}),"Fenetre de test");
    window.setVerticalSyncEnabled(true);
    window.setPosition({0,0});
    std::cout << window.getPosition().x <<  window.getPosition().y  << std::endl;
    sf::Texture texture;
    if(! texture.loadFromFile("image/OIP.jpg",false,sf::IntRect({0,0},{10000,10000}))){
        std::cout << "Impossible d'afficher l'image" << std::endl;
    }
    sf::Sprite sprite(texture);
    sprite.setTextureRect(sf::IntRect({0,0},{400,400}));
    while(window.isOpen()){
        while(std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}