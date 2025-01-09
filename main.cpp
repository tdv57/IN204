#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>


sf::Vector2i operator * (const size_t& side,const sf::Vector2i v){
    return sf::Vector2i(side*v.x,side*v.y);
}

sf::Vector2f operator * (const size_t& side,const sf::Vector2f v){
    return sf::Vector2f(side*v.x,side*v.y);
}

sf::Vector2i operator * (const sf::Vector2i v,const size_t& side){
    return sf::Vector2i(side*v.x,side*v.y);
}

sf::Vector2f operator * (const sf::Vector2f v,const size_t& side){
    return sf::Vector2f(side*v.x,side*v.y);
}


class Square: public sf::Drawable, public sf::Transformable { 
public:

    Square():m_side(0){}


    Square(size_t newSide, sf::Color newcolor = sf::Color::Black, size_t newinitX = 0,size_t newinitY=0): m_side(newSide),m_color(newcolor),m_initX(newinitX),m_initY(newinitY)
    {
        m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        m_vertices.resize(4);

        m_vertices[0].position = sf::Vector2f(m_initX+m_side,m_initY);
        m_vertices[1].position = sf::Vector2f(m_initX,m_initY);
        m_vertices[2].position = sf::Vector2f(m_initX+m_side,m_initY +m_side);
        m_vertices[3].position = sf::Vector2f(m_initX,m_initY+m_side);

        for(int i=0; i<4;i++){
            m_vertices[i].color = m_color;
        }
        setOrigin(sf::Vector2f(m_initX + m_side / 2.0f, m_initY + m_side / 2.0f));

        // Définit la position initiale de l'objet
        setPosition({m_initX, m_initY});
    }



    size_t getSide()const{
        return m_side;
    }

    template<typename T>
    requires std::integral<T>
    void setSide(T newside){
        m_side = newside;
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // apply the transform
        states.transform *= getTransform();

        // draw the vertex array
        target.draw(m_vertices, states);
    }


    sf::VertexArray m_vertices;
    size_t m_side;
    size_t m_initX;
    size_t m_initY;
    sf::Color m_color;
};




class Figure {


public:


    // sf::Color m_color;
    // size_t m_side;
    // int m_bottomAbsIndex;
    // int m_bottomOrdIndex;
    // int m_bottomAbs;
    // int m_bottomOrd;
    Figure(size_t side,sf::Color color,int bottomAbsIndex, int bottomOrdIndex,int bottomAbs,int bottomOrd):m_side(side),m_color(color),m_bottomAbsIndex(bottomAbsIndex),m_bottomOrdIndex(bottomOrdIndex),m_bottomAbs(bottomAbs),m_bottomOrd(bottomOrd){}
    

    void goLeft(){
        int size = m_squaresIndex.size();
        for(int i=0 ; i<size;i++){
            m_squaresIndex[i].x -= 1;
            m_squaresVector[i].move(m_side * sf::Vector2f(-1,0));
        }
        m_bottomAbsIndex -= 1;
    }

    void goRight(){
        int size = m_squaresIndex.size();
        for(int i=0; i<size;i++){
            m_squaresIndex[i].x +=1;
            m_squaresVector[i].move(m_side * sf::Vector2f(1,0));
        }
        m_bottomAbsIndex +=1;
    }

    void goDown(){
        int size = m_squaresIndex.size();
        for(int i=0; i<size;i++){
            m_squaresIndex[i].y +=1;
            m_squaresVector[i].move(m_side * sf::Vector2f(0,1));
        }
        m_bottomOrdIndex +=1;
    }


    auto getSquaresIndex() const{
        return m_squaresIndex;
    }

    void draw(sf::RenderWindow& window){
        int size = m_squaresVector.size();
        for (int i=0;i<size;i++){
            window.draw(m_squaresVector[i]);
        } 
    } // En gros grâce à m_origin m_originIndex m_side et m_squaresIndex on peut tout tracer;


    void setSquares(){
        for(int i=0 ; i<m_squaresIndex.size();i++){
            m_squaresVector.emplace_back(Square(m_side,m_color,m_bottomAbs + m_side * (m_squaresIndex[i].x-m_bottomAbsIndex) , m_bottomOrd + m_side * (m_squaresIndex[i].y-m_bottomOrdIndex) ));
        }
    }

    virtual void RotateLeft() = 0; // Chaque classe fille va implémenter sa rotation
    virtual void RotateRight() = 0; // Chaque classe fille va implémenter sa rotation

    // Une figure c'est plusieurs carrés qu'on va générer avec des vertex et des triangles
    // C'est également une abscisse et une ordonnée pour chaque carré
protected:



    sf::Color m_color;
    size_t m_side;
    std::vector<Square> m_squaresVector; // Je crois qu'on en a pas besoin
    std::vector<sf::Vector2i> m_squaresIndex;
    int m_bottomAbsIndex;
    int m_bottomOrdIndex;
    int m_bottomAbs;
    int m_bottomOrd;
    // On va créer pour chaque classe fille une initialisation
    
};

class Fig1 : public Figure{

public:

// Figure(size_t side,sf::Color color,int bottomAbsIndex, int bottomOrdIndex,int bottomAbs,int bottomOrd)
    Fig1(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0)
    {
        // Je rajoute squaresIndex
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex+1,bottomOrdIndex));
        // Je trace  avec setSquares()
        setSquares();
    }
    void RotateLeft() override{
        int size = m_squaresVector.size();
        for(int i=0; i<size;i++){
            m_squaresIndex[i] = m_squaresIndex[i] + m_changeArray[m_changeIndex][i];
            m_squaresVector[i].move(m_side *  static_cast<sf::Vector2f> (m_changeArray[m_changeIndex][i]));
        }
        m_changeIndex++;
        m_changeIndex = m_changeIndex % m_nbPosition;
    }

    void RotateRight() override{
        int size = m_squaresVector.size();
        for(int i=0; i<size;i++){
            m_squaresIndex[i] = m_squaresIndex[i] - m_changeArray[m_changeIndex][i];
            m_squaresVector[i].move(m_side * -1.0f * static_cast<sf::Vector2f> (m_changeArray[m_changeIndex][i]));
        }
        m_changeIndex --;
        if(m_changeIndex<0){
            m_changeIndex = m_nbPosition - 1;
        }
    }




private:
    


    static constexpr std::array<std::array< sf::Vector2i,4>,2> m_changeArray = {{   {sf::Vector2i(2, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(0, -2)},{sf::Vector2i(-2, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 2)}   }};
    int m_changeIndex;
    static const int m_nbPosition =2;

};





class tableau{
    // vector<std::array<Square,11>
    // 22 longueurs 10 largeurs // Il faut des cases en plus invisibles pour charger l'objet


    
private:
    std::vector<std::array<Square,10>> tableau_m;
    size_t SquareXsize;
    size_t SquareYsize; 
    // Ou alors std::array<std::array<Square,10>,23> tableau_m (j'ai mis 23 car on aura deux case à cacher); 
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
    Square FirstCarre(40,sf::Color::Red,100,100);

    Fig1 FirstFig1(50,sf::Color::Red,32,32,150,150);

    while(window.isOpen()){
        while(std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }

            if(const auto* UpKey = event->getIf<sf::Event::KeyPressed>()){
                if(UpKey->scancode == sf::Keyboard::Scancode::Down){
                    FirstFig1.goDown();
                }
                if(UpKey->scancode == sf::Keyboard::Scancode::Left){
                    FirstFig1.goLeft();
                }
                if(UpKey->scancode == sf::Keyboard::Scancode::Right){
                    FirstFig1.goRight();
                }
            }
        }
        
        window.clear();
        FirstCarre.rotate(sf::degrees(10));
        window.draw(FirstCarre);
        FirstFig1.draw(window);
        window.display();
    }
    return 0;
}
