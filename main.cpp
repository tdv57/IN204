#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>


/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */
/********************************-------Déclaration des différentes formes de figures---------------------*************************************/
/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */

// Figure -> 23
//            01
class Fig1;

// Figure -> 3
//           2
//           1
//           0 
class Fig2;

// Figure ->  3
//            2
//           01
class Fig3;

// Figure -> 123
//            0
class Fig4;


// Figure -> 3
//           2
//           10
class Fig5; 


// Figure ->  32
//           10
class Fig6;


// Figure -> 32
//           10
class Fig7;


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

    Figure(size_t side,sf::Color color,int bottomAbsIndex, int bottomOrdIndex,int bottomAbs,int bottomOrd):m_side(side),m_color(color),m_bottomAbsIndex(bottomAbsIndex),m_bottomOrdIndex(bottomOrdIndex),m_bottomAbs(bottomAbs),m_bottomOrd(bottomOrd){}
    
    void goLeft(){
        int size = m_squaresIndex.size();
        for(int i=0 ; i<size;i++){
            m_squaresIndex[i].x -= 1;
            m_squaresVector[i].move(m_side * sf::Vector2f(-1,0));
        }
        m_bottomAbsIndex -= 1;
        m_leftAbsIndex -=1;
        m_rightAbsIndex -=1;
    }

    void goRight(){
        int size = m_squaresIndex.size();
        for(int i=0; i<size;i++){
            m_squaresIndex[i].x +=1;
            m_squaresVector[i].move(m_side * sf::Vector2f(1,0));
        }
        m_bottomAbsIndex +=1;
        m_leftAbsIndex +=1;
        m_rightAbsIndex +=1;
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

    void printSquaresIndex() const{
        for(int i=0 ; i<m_squaresIndex.size(); i++){
            std::cout << m_squaresIndex[i].x << " " << m_squaresIndex[i].y << std::endl;
        }
    }

    void setSquares(){
        for(int i=0 ; i<m_squaresIndex.size();i++){
            m_squaresVector.emplace_back(Square(m_side,m_color,m_bottomAbs + m_side * (m_squaresIndex[i].x-m_bottomAbsIndex) , m_bottomOrd + m_side * (m_squaresIndex[i].y-m_bottomOrdIndex) ));
        }
    }

    int getNbSquares() const{
        return m_squaresVector.size();
    }


    void draw(sf::RenderWindow& window){
        int size = m_squaresVector.size();
        for (int i=0;i<size;i++){
            window.draw(m_squaresVector[i]);
        } 
    } // En gros grâce à m_origin m_originIndex m_side et m_squaresIndex on peut tout tracer;


    int getBottomOrdIndex() const{
        return m_bottomOrdIndex;
    }

    int getLeftAbsIndex() const{
        return m_leftAbsIndex;
    }

    int getRightAbsIndex() const{
        return m_rightAbsIndex;
    }


    // virtual void RotateLeft() = 0; // Chaque classe fille va implémenter sa rotation
    // virtual void RotateRight() = 0; // Chaque classe fille va implémenter sa rotation

    // Une figure c'est plusieurs carrés qu'on va générer avec des vertex et des triangles
    // C'est également une abscisse et une ordonnée pour chaque carré
protected:



    sf::Color m_color;
    size_t m_side;
    std::vector<Square> m_squaresVector; // Je crois qu'on en a pas besoin
    std::vector<sf::Vector2i> m_squaresIndex;
    int m_bottomAbsIndex;
    int m_bottomOrdIndex;
    int m_leftAbsIndex;
    int m_rightAbsIndex;
    int m_bottomAbs;
    int m_bottomOrd;
    // On va créer pour chaque classe fille une initialisation
    
};






template<typename Figx>
concept isFig = std::same_as<Figx,Fig1> || std::same_as<Figx,Fig2> || std::same_as<Figx,Fig3> || std::same_as<Figx,Fig4>  || std::same_as<Figx,Fig5>  || std::same_as<Figx,Fig6>  || std::same_as<Figx,Fig7>;

template<typename Figx>
int getChangeIndex(const Figx &fig) requires isFig<Figx>{
    return fig.m_changeIndex;
}

template<typename Figx>
void decreaseChangeIndex(Figx &fig) requires isFig<Figx>{
    fig.m_changeIndex --;
}

template<typename Figx>
void increaseChangeIndex(Figx &fig) requires isFig<Figx>{
    fig.m_changeIndex ++;
}

template<typename Figx>
void setChangeIndex(Figx &fig, int n){
    if (n>=0 && n<fig.m_nbPosition) fig.m_changeIndex = n;
}

template<typename Figx>
int getNbPosition(const Figx &fig){
    return fig.m_nbPosition;
}

template<typename Figx>
auto getChangeArray(const Figx& fig){
    return fig.m_changeArray;
}

template<typename Figx>
auto getChangeAbsArray(const Figx& fig){
    return fig.m_changeAbsArray;
}


template<typename Figx>
void rotateLeft(Figx &fig) requires isFig<Figx>{

    int size = fig.getNbSquares();
    int changeIndex = getChangeIndex(fig);
    auto changeArray = getChangeArray(fig);
    for(int i=0; i<size;i++){
        fig.m_squaresIndex[i] += changeArray[changeIndex][i];
        fig.m_squaresVector[i].move(fig.m_side *static_cast<sf::Vector2f> (changeArray[changeIndex][i]));
    }

    auto changeAbsArray = getChangeAbsArray(fig);
    fig.m_leftAbsIndex +=  changeAbsArray[changeIndex].x;
    fig.m_rightAbsIndex += changeAbsArray[changeIndex].y;

    fig.m_changeIndex ++;
    fig.m_changeIndex = fig.m_changeIndex % getNbPosition(fig);

}


template<typename Figx>
void rotateRight(Figx &fig) requires isFig<Figx>{
    fig.m_changeIndex --;
    if(fig.m_changeIndex<0){
        setChangeIndex(fig,getNbPosition(fig)-1);
    }
    int size = fig.getNbSquares();
    int changeIndex = getChangeIndex(fig);
    auto changeArray = getChangeArray(fig);
    for(int i=0; i<size;i++){
        fig.m_squaresIndex[i] -=  changeArray[changeIndex][i];
        fig.m_squaresVector[i].move(fig.m_side * -1.0f * static_cast<sf::Vector2f> (changeArray[changeIndex][i]));
    }
    auto changeAbsArray = getChangeAbsArray(fig);
    fig.m_leftAbsIndex -=  changeAbsArray[changeIndex].x;
    fig.m_rightAbsIndex -= changeAbsArray[changeIndex].y;
}



// Figure -> 23
//            01
class Fig1 : public Figure{
public:
    Fig1(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0)
    {
        // Je rajoute squaresIndex
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex+1,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
        // Je trace  avec setSquares()
        setSquares();
                
        m_leftAbsIndex = bottomAbsIndex-1;
        m_rightAbsIndex = bottomAbsIndex+1;
    }


    friend int getChangeIndex<Fig1>(const Fig1 &fig);
    friend void decreaseChangeIndex<Fig1>(Fig1 &fig); 
    friend void increaseChangeIndex<Fig1> (Fig1 &fig);
    friend void setChangeIndex<Fig1> (Fig1 &fig, int n);
    friend int getNbPosition<Fig1> (const Fig1 &fig);
    friend auto getChangeArray<Fig1>(const Fig1& fig);
    friend auto getChangeAbsArray<Fig1>(const Fig1& fig);
    friend void rotateLeft<Fig1>(Fig1 &fig);
    friend void rotateRight<Fig1>(Fig1 &fig);

private:

    static constexpr std::array<std::array< sf::Vector2i,4>,2> m_changeArray = {{   {sf::Vector2i(0, 0), sf::Vector2i(0, -2), sf::Vector2i(2, 0), sf::Vector2i(0, 0)},{sf::Vector2i(0, 0), sf::Vector2i(0, 2), sf::Vector2i(-2, 0), sf::Vector2i(0, 0)}   }};
    static constexpr std::array<sf::Vector2i,2> m_changeAbsArray = { sf::Vector2i(1,0) , sf::Vector2i(-1,0)};
    int m_changeIndex;
    static const int m_nbPosition =2;
};



// Figure -> 3
//           2
//           1
//           0
class Fig2 : public Figure{
public:
    Fig2(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0)
    {
        // Je rajoute squaresIndex
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-2));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-3));
        // Je trace  avec setSquares()
        setSquares();
                
        m_leftAbsIndex = bottomAbsIndex;
        m_rightAbsIndex = bottomAbsIndex;
    }


    friend int getChangeIndex<Fig2>(const Fig2 &fig);
    friend void decreaseChangeIndex<Fig2>(Fig2 &fig); 
    friend void increaseChangeIndex<Fig2> (Fig2 &fig);
    friend void setChangeIndex<Fig2> (Fig2 &fig, int n);
    friend int getNbPosition<Fig2> (const Fig2 &fig);
    friend auto getChangeArray<Fig2>(const Fig2& fig);
    friend auto getChangeAbsArray<Fig2>(const Fig2& fig);
    friend void rotateLeft<Fig2>(Fig2 &fig);
    friend void rotateRight<Fig2>(Fig2 &fig);

private:

    static constexpr std::array<std::array< sf::Vector2i,4>,2> m_changeArray = {{   {sf::Vector2i(0, 0), sf::Vector2i(1, 1), sf::Vector2i(-1, 2), sf::Vector2i(-2, 3)},{sf::Vector2i(0, 0), sf::Vector2i(-1, -1), sf::Vector2i(1, -2), sf::Vector2i(2, -3)}   }};
    static constexpr std::array<sf::Vector2i,2> m_changeAbsArray = { sf::Vector2i(-2,1) , sf::Vector2i(2,-1)};
    int m_changeIndex;
    static const int m_nbPosition =2;
};


// Figure ->  2
//            1
//           30
class Fig3 : public Figure{
public:
    Fig3(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0)
    {
        // Je rajoute squaresIndex
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex+1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex+2));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex));
        // Je trace  avec setSquares()
        setSquares();
                
        m_leftAbsIndex = bottomAbsIndex;
        m_rightAbsIndex = bottomAbsIndex+1;
    }


    friend int getChangeIndex<Fig3>(const Fig3 &fig);
    friend void decreaseChangeIndex<Fig3>(Fig3 &fig); 
    friend void increaseChangeIndex<Fig3> (Fig3 &fig);
    friend void setChangeIndex<Fig3> (Fig3 &fig, int n);
    friend int getNbPosition<Fig3> (const Fig3 &fig);
    friend auto getChangeArray<Fig3>(const Fig3& fig);
    friend auto getChangeAbsArray<Fig3>(const Fig3& fig);
    friend void rotateLeft<Fig3>(Fig3 &fig);
    friend void rotateRight<Fig3>(Fig3 &fig);

private:

static constexpr std::array<std::array<sf::Vector2i, 4>, 4> m_changeArray = {{      {sf::Vector2i(0, 0), sf::Vector2i(-2, 0), sf::Vector2i(-2, 2), sf::Vector2i(0, 0)},{sf::Vector2i(0, -2), sf::Vector2i(1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, -2)},{sf::Vector2i(-2, 1), sf::Vector2i(1, 0), sf::Vector2i(1, 0),sf::Vector2i(0, 1) },{sf::Vector2i(2, 1), sf::Vector2i(0, 0), sf::Vector2i(0, -2), sf::Vector2i(0, 1)}       }};
    static constexpr std::array<sf::Vector2i,4> m_changeAbsArray = { sf::Vector2i(1,0) , sf::Vector2i(-1,-1), sf::Vector2i(1,1), sf::Vector2i(-1,0)};
    int m_changeIndex;
    static const int m_nbPosition =4;
};


// Figure -> 123
//            0
class Fig4 : public Figure{
public:
    Fig4(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0)
    {
        // Je rajoute squaresIndex
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex+1,bottomOrdIndex-1));
        // Je trace  avec setSquares()
        setSquares();
                
        m_leftAbsIndex = bottomAbsIndex-1;
        m_rightAbsIndex = bottomAbsIndex+1;
    }


    friend int getChangeIndex<Fig4>(const Fig4 &fig);
    friend void decreaseChangeIndex<Fig4>(Fig4 &fig); 
    friend void increaseChangeIndex<Fig4> (Fig4 &fig);
    friend void setChangeIndex<Fig4> (Fig4 &fig, int n);
    friend int getNbPosition<Fig4> (const Fig4 &fig);
    friend auto getChangeArray<Fig4>(const Fig4& fig);
    friend auto getChangeAbsArray<Fig4>(const Fig4& fig);
    friend void rotateLeft<Fig4>(Fig4 &fig);
    friend void rotateRight<Fig4>(Fig4 &fig);

private:

    static constexpr std::array<std::array< sf::Vector2i,4>,4> m_changeArray = {{   {sf::Vector2i(1, 0), sf::Vector2i(2, -1), sf::Vector2i(0, 0), sf::Vector2i(0,0)},{sf::Vector2i(0, 0), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1)}, {sf::Vector2i(0, -1), sf::Vector2i(0,0), sf::Vector2i(1, -2), sf::Vector2i(0, 0)},{sf::Vector2i(-1, 1), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(1, -1)}  }};
    static constexpr std::array<sf::Vector2i,4> m_changeAbsArray = { sf::Vector2i(-1,0) , sf::Vector2i(1,0), sf::Vector2i(-1,0), sf::Vector2i(1,0)};
    int m_changeIndex;
    static const int m_nbPosition =4;
};


// Figure -> 3
//           2
//           10

class Fig5 : public Figure{
public:
    Fig5(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0)
    {
        // Je rajoute squaresIndex
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-2));
        // Je trace  avec setSquares()
        setSquares();
                
        m_leftAbsIndex = bottomAbsIndex-1;
        m_rightAbsIndex = bottomAbsIndex;
    }


    friend int getChangeIndex<Fig5>(const Fig5 &fig);
    friend void decreaseChangeIndex<Fig5>(Fig5 &fig); 
    friend void increaseChangeIndex<Fig5> (Fig5 &fig);
    friend void setChangeIndex<Fig5> (Fig5 &fig, int n);
    friend int getNbPosition<Fig5> (const Fig5 &fig);
    friend auto getChangeArray<Fig5>(const Fig5& fig);
    friend auto getChangeAbsArray<Fig5>(const Fig5& fig);
    friend void rotateLeft<Fig5>(Fig5 &fig);
    friend void rotateRight<Fig5>(Fig5 &fig);

private:

static constexpr std::array<std::array<sf::Vector2i, 4>, 4> m_changeArray = {{      {sf::Vector2i(0,-1), sf::Vector2i(0, -1), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1)},{sf::Vector2i(0, 0), sf::Vector2i(1, -1), sf::Vector2i(2, 0), sf::Vector2i(1, -1)},{sf::Vector2i(0, 0), sf::Vector2i(-1, 2), sf::Vector2i(0, 0), sf::Vector2i(-1, 2)},{sf::Vector2i(0, 1), sf::Vector2i(0, 0), sf::Vector2i(-1, -1), sf::Vector2i(1, -2)}       }};
    static constexpr std::array<sf::Vector2i,4> m_changeAbsArray = { sf::Vector2i(1,0) , sf::Vector2i(-1,0), sf::Vector2i(1,0), sf::Vector2i(-1,0)};
    int m_changeIndex;
    static const int m_nbPosition =4;
};


// Figure ->  32
//           10
class Fig6 : public Figure{
public:
    Fig6(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0)
    {
        // Je rajoute squaresIndex
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex+1,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
        // Je trace  avec setSquares()
        setSquares();
                
        m_leftAbsIndex = bottomAbsIndex-1;
        m_rightAbsIndex = bottomAbsIndex+1;
    }


    friend int getChangeIndex<Fig6>(const Fig6 &fig);
    friend void decreaseChangeIndex<Fig6>(Fig6 &fig); 
    friend void increaseChangeIndex<Fig6> (Fig6 &fig);
    friend void setChangeIndex<Fig6> (Fig6 &fig, int n);
    friend int getNbPosition<Fig6> (const Fig6 &fig);
    friend auto getChangeArray<Fig6>(const Fig6& fig);
    friend auto getChangeAbsArray<Fig6>(const Fig6& fig);
    friend void rotateLeft<Fig6>(Fig6 &fig);
    friend void rotateRight<Fig6>(Fig6 &fig);

private:

    static constexpr std::array<std::array< sf::Vector2i,4>,2> m_changeArray = {{   {sf::Vector2i(0, -2), sf::Vector2i(2, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)},{sf::Vector2i(0, 2), sf::Vector2i(-2, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)}   }};
    static constexpr std::array<sf::Vector2i,2> m_changeAbsArray = { sf::Vector2i(-1,0) , sf::Vector2i(1,0)};
    int m_changeIndex;
    static const int m_nbPosition =2;
};


// Figure -> 32
//           10
class Fig7 : public Figure{
public:
    Fig7(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0)
    {
        // Je rajoute squaresIndex
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
        m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
        // Je trace  avec setSquares()
        setSquares();
                
        m_leftAbsIndex = bottomAbsIndex-1;
        m_rightAbsIndex = bottomAbsIndex;
    }


    friend int getChangeIndex<Fig7>(const Fig7 &fig);
    friend void decreaseChangeIndex<Fig7>(Fig7 &fig); 
    friend void increaseChangeIndex<Fig7> (Fig7 &fig);
    friend void setChangeIndex<Fig7> (Fig7 &fig, int n);
    friend int getNbPosition<Fig7> (const Fig7 &fig);
    friend auto getChangeArray<Fig7>(const Fig7& fig);
    friend auto getChangeAbsArray<Fig7>(const Fig7& fig);
    friend void rotateLeft<Fig7>(Fig7 &fig);
    friend void rotateRight<Fig7>(Fig7 &fig);

private:

    static constexpr std::array<std::array< sf::Vector2i,4>,1> m_changeArray = {{   {sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)}   }};
    static constexpr std::array<sf::Vector2i,2> m_changeAbsArray = { sf::Vector2i(0,0)};
    int m_changeIndex;
    static const int m_nbPosition =1;
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

    Fig7 FirstFig1(50,sf::Color::Red,32,32,150,150);

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
                if(UpKey->scancode == sf::Keyboard::Scancode::H){
                    rotateLeft(FirstFig1);
                }
                if(UpKey->scancode == sf::Keyboard::Scancode::J){
                    rotateRight(FirstFig1);
                }
            }
        }
        
        window.clear();
        FirstCarre.move({1,1});
        window.draw(FirstCarre);
        FirstFig1.draw(window);
        window.display();
    }
    return 0;
}

