#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>


/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */
/********************************-------Déclaration des différentes formes de figures---------------------*************************************/
/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */

class Generator;

// Classe des couleurs
class TetrisColor{
public:
    static constexpr sf::Color Orange = sf::Color(255,165,0);
    static constexpr sf::Color Black = sf::Color::Black;
    static constexpr sf::Color Magenta = sf::Color::Magenta;
    static constexpr sf::Color Red = sf::Color::Red;
    static constexpr sf::Color Yellow = sf::Color::Yellow;
    static constexpr sf::Color Green = sf::Color::Green;
    static constexpr sf::Color Cyan = sf::Color::Cyan;
    static constexpr sf::Color Blue = sf::Color::Blue;
};

// Multiplication d'un vecteur par un scalaire
sf::Vector2i operator * (const float& side,const sf::Vector2i v){
    return sf::Vector2i(side*v.x,side*v.y);
}

sf::Vector2f operator * (const float& side,const sf::Vector2f v){
    return sf::Vector2f(side*v.x,side*v.y);
}

sf::Vector2i operator * (const sf::Vector2i v,const float& side){
    return sf::Vector2i(side*v.x,side*v.y);
}

sf::Vector2f operator * (const sf::Vector2f v,const float& side){
    return sf::Vector2f(side*v.x,side*v.y);
}

// Classe des carrés élémentaires
class Square: public sf::Drawable, public sf::Transformable { 

private:
    sf::VertexArray m_vertices;
    // Vecteur contenant les 4 coins
    float m_side;
    // Longueur d'un côté du carré
    float m_initX;
    // Abscisse initiale
    float m_initY;
    // Ordonnée initiale
    sf::Color m_color;
    // Couleur

public:

    Square():m_side(0){}


    Square(float newSide, sf::Color newcolor = sf::Color::Black, float newinitX = 0,float newinitY=0): m_side(newSide),m_color(newcolor),m_initX(newinitX),m_initY(newinitY)
    {
        m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        m_vertices.resize(4);

        m_vertices[0].position = sf::Vector2f(m_initX+m_side,m_initY);
        m_vertices[1].position = sf::Vector2f(m_initX,m_initY);
        m_vertices[2].position = sf::Vector2f(m_initX+m_side,m_initY +m_side);
        m_vertices[3].position = sf::Vector2f(m_initX,m_initY+m_side);
        // Position des coins: 10
        //                     32

        for(int i=0; i<4;i++){
            m_vertices[i].color = m_color;
        }
        setOrigin(sf::Vector2f(m_initX + m_side / 2.0f, m_initY + m_side / 2.0f));

        // Définit la position initiale de l'objet
        setPosition({m_initX, m_initY});
    }


    float getNewInitX() const{
        return m_initX;
    }

    float getNewInitY() const{
        return m_initY;
    }

    float getSide()const{
        return m_side;
    }

    sf::Color getColor() const{
        return m_color;
    }

    template<typename T>
    requires std::integral<T>
    void setSide(T newside){
        m_side = newside;
        //m_initX = this->getPosition().x;
        //m_initY = this->getPosition().y;
        m_vertices[0].position = sf::Vector2f(m_initX+m_side,m_initY);
        m_vertices[1].position = sf::Vector2f(m_initX,m_initY);
        m_vertices[2].position = sf::Vector2f(m_initX+m_side,m_initY +m_side);
        m_vertices[3].position = sf::Vector2f(m_initX,m_initY+m_side);
    }

    bool operator ==(const Square &it){
        if(it.getColor() == getColor() && it.getSide()==m_side){
            return true;
        }
        return false;
    }

    bool operator !=(const Square &it){
        if(it.getColor() != getColor() || it.getSide()!=m_side){
            return true;
        }
        return false;
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // apply the transform
        states.transform *= getTransform();

        // draw the vertex array
        target.draw(m_vertices, states);
    }


};




class Figure {

protected:

    sf::Color m_color;
    // Couleur de la pièce
    float m_side;
    // Longueur d'un côté de carré
    std::vector<Square> m_squaresVector; // Je crois qu'on en a pas besoin
    // Les différents carrés de la pièce, normalement au nombre de 4
    std::vector<sf::Vector2i> m_squaresIndex;
    // Index des différents carrés au sein de la pièce, permet d'identifier les carrés composant une pièce
    int m_bottomAbsIndex;
    // Abscisse du carré le plus en bas sous forme d'index
    int m_bottomOrdIndex;
    // Ordonnée du carré le plus en bas sous forme d'index
    int m_leftAbsIndex;
    // Abscisse du carré le plus à gauche sous forme d'index
    int m_rightAbsIndex;
    // Abscisse du carré le plus à droite sous forme d'index
    int m_topOrdIndex;
    // Ordonnée du carré le plus en haut sous forme d'index
    int m_bottomAbs;
    // Abscisse réelle du carré le plus en bas, noté 0 dans les schémas de la classe TetrisFigure (voir plus loin)
    int m_bottomOrd;
    // Ordonnée réelle du carré le plus en bas, noté 0 dans les schémas de la classe TetrisFigure (voir plus loin)

    // On va créer pour chaque classe fille une initialisation


public:

    Figure(float side,sf::Color color,int bottomAbsIndex, int bottomOrdIndex,int bottomAbs,int bottomOrd):m_side(side),m_color(color),m_bottomAbsIndex(bottomAbsIndex),m_bottomOrdIndex(bottomOrdIndex),m_bottomAbs(bottomAbs),m_bottomOrd(bottomOrd){}
    
    void goUp(){
        int size = m_squaresIndex.size();
        for(int i=0; i<size;i++){
            m_squaresIndex[i].y -=1;
            m_squaresVector[i].move(m_side * sf::Vector2f(0,-1));
        }
        m_bottomOrdIndex -=1;
        m_topOrdIndex -=1;
        m_bottomOrd -= m_side;
    }

    void goLeft(){
        int size = m_squaresIndex.size();
        for(int i=0 ; i<size;i++){
            m_squaresIndex[i].x -= 1;
            m_squaresVector[i].move(m_side * sf::Vector2f(-1,0));
        }
        m_bottomAbsIndex -= 1;
        m_leftAbsIndex -=1;
        m_rightAbsIndex -=1;
        m_bottomAbs -=m_side;
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
        m_bottomAbs += m_side;
    }

    void goDown(){
        int size = m_squaresIndex.size();
        for(int i=0; i<size;i++){
            m_squaresIndex[i].y +=1;
            m_squaresVector[i].move(m_side * sf::Vector2f(0,1));
        }
        m_bottomOrdIndex +=1;
        m_topOrdIndex +=1;
        m_bottomOrd += m_side;
    }

    sf::Color getColor()const{
        return m_color;
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

    int getBottomAbsIndex() const{
        return m_bottomAbsIndex;
    }

    int getBottomAbs() const{
        return m_bottomAbs;
    }

    int getBottomOrd() const{
        return m_bottomOrd;
    }

    int getLeftAbsIndex() const{
        return m_leftAbsIndex;
    }

    int getRightAbsIndex() const{
        return m_rightAbsIndex;
    }
    
    int getSide()const{
        return m_side;
    }

    void printPosition(){
        for(int i=0;i<m_squaresVector.size();i++){
            std::cout << i << " : " << m_squaresVector[i].getPosition().x << " " << m_squaresVector[i].getPosition().y << std::endl;
        }
    }
    sf::Vector2f getPosition() const{
        return sf::Vector2f(m_bottomAbs,m_bottomOrd);
    }

    void setBottomAbs(const int &newBottomAbs){
        setNewCoord(sf::Vector2f(newBottomAbs,m_bottomOrd),m_side);
    }
    
    void setBottomOrd(const int &newBottomOrd){
        setNewCoord(sf::Vector2f(m_bottomAbs,newBottomOrd),m_side);
    }

    void setNewCoord(sf::Vector2f newCoord,float newSide){


        float deltax = newCoord.x - m_bottomAbs ;
        float deltay = newCoord.y - m_bottomOrd;
        for(int i=0 ; i<m_squaresVector.size();i++){
            m_squaresVector[i].move(sf::Vector2f(deltax,deltay));
            m_squaresVector[i].setSide(static_cast<int>(newSide));
        }


        m_bottomAbs = newCoord.x;
        m_bottomOrd = newCoord.y;
    }


    // virtual void RotateLeft() = 0; // Chaque classe fille va implémenter sa rotation
    // virtual void RotateRight() = 0; // Chaque classe fille va implémenter sa rotation

    // Une figure c'est plusieurs carrés qu'on va générer avec des vertex et des triangles
    // C'est également une abscisse et une ordonnée pour chaque carré

    
};


class TetrisFigure: public Figure{

private:

    // Pour chacune des pièces, m_changeArray donne le déplacement de chaque carré élémentaire pour chaque rotation possible
    // Exemple: m_changeArray[m_figNum][changeIndex][i] donne pour la figure numéro m_figNum, la rotation correspondant à changeIndex
    // et le carré numéro i le déplacement à effectuer
    static constexpr std::array<std::array<std::array<sf::Vector2i, 4>, 4>, 7> m_changeArray = {{
        {{   
        // Figure -> 23
        //            01
        // Pièce Z, 2 rotations différentes
            {{sf::Vector2i(0, 0), sf::Vector2i(0, -2), sf::Vector2i(2, 0), sf::Vector2i(0, 0)}},{{sf::Vector2i(0, 0), sf::Vector2i(0, 2), sf::Vector2i(-2, 0), sf::Vector2i(0, 0)}}   
        }},
        {{   
        // Figure -> 3
        //           2
        //           1
        //           0 
        // Pièce I, 2 rotations différentes
            {{sf::Vector2i(0, 0), sf::Vector2i(1, 1), sf::Vector2i(-1, 2), sf::Vector2i(-2, 3)}},{{sf::Vector2i(0, 0), sf::Vector2i(-1, -1), sf::Vector2i(1, -2), sf::Vector2i(2, -3)}}   
        }},
        {{   
        // Figure ->  2
        //            1
        //           30 
        // Pièce J, 4 rotations différentes
            {{sf::Vector2i(0, 0), sf::Vector2i(-2, 0), sf::Vector2i(-2, 2), sf::Vector2i(0, 0)}},{{sf::Vector2i(0, -2), sf::Vector2i(1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, -2)}},{{sf::Vector2i(-2, 1), sf::Vector2i(1, 0), sf::Vector2i(1, 0),sf::Vector2i(0, 1) }},{{sf::Vector2i(2, 1), sf::Vector2i(0, 0), sf::Vector2i(0, -2), sf::Vector2i(0, 1)}}      
        }},
        {{   
        // Figure -> 123
        //            0
        // Pièce T, 4 rotations différentes
            {{sf::Vector2i(1, 0), sf::Vector2i(2, -1), sf::Vector2i(0, 0), sf::Vector2i(0,0)}},{{sf::Vector2i(0, 0), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1)}}, {{sf::Vector2i(0, -1), sf::Vector2i(0,0), sf::Vector2i(1, -2), sf::Vector2i(0, 0)}},{{sf::Vector2i(-1, 1), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(1, -1)}}  
        }},
        {{   
        // Figure -> 3
        //           2
        //           10
        // Pièce L, 4 rotations différentes
            {{sf::Vector2i(0,-1), sf::Vector2i(0, -1), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1)}},{{sf::Vector2i(0, 0), sf::Vector2i(1, -1), sf::Vector2i(2, 0), sf::Vector2i(1, -1)}},{{sf::Vector2i(0, 0), sf::Vector2i(-1, 2), sf::Vector2i(0, 0), sf::Vector2i(-1, 2)}},{{sf::Vector2i(0, 1), sf::Vector2i(0, 0), sf::Vector2i(-1, -1), sf::Vector2i(1, -2)}}       
        }},
        {{   
        // Figure ->  32
        //           10
        // Pièce S, 2 rotations différentes
            {{sf::Vector2i(0, -2), sf::Vector2i(2, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)}},{{sf::Vector2i(0, 2), sf::Vector2i(-2, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)}}   
        }},
        {{   
        // Figure -> 32
        //           10
        // Pièce O, 1 unique rotation
            {{sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)}}   
        }}
    }};

    // De la même manière, m_changeAbsArray donne le déplacement du carré le plus à gauche, du carré le plus à droite
    // et la modification de la longueur de la figure en abscisse (ce qui correspond à la différence des 2 termes précédents)
    static constexpr std::array<std::array<sf::Vector3i,4>,7> m_changeAbsArray = {{
        // Figure -> 23
        //            01
        {{ sf::Vector3i(1,0,-1) , sf::Vector3i(-1,0,1)}},
        // Figure -> 3
        //           2
        //           1
        //           0 
        {{ sf::Vector3i(-2,1,3) , sf::Vector3i(2,-1,-3)}},
        // Figure ->  2
        //            1
        //           30
        {{ sf::Vector3i(-1,0,1) , sf::Vector3i(1,0,-1), sf::Vector3i(-1,0,1), sf::Vector3i(1,0,-1)}},
        // Figure -> 123
        //            0
        {{ sf::Vector3i(1,0,-1) , sf::Vector3i(-1,0,1), sf::Vector3i(1,0,-1), sf::Vector3i(-1,0,1)}},
        // Figure -> 3
        //           2
        //           10
        {{ sf::Vector3i(-1,0,1) , sf::Vector3i(1,0,-1), sf::Vector3i(-1,0,1), sf::Vector3i(1,0,-1)}},
        // Figure ->  32
        //           10
        {{ sf::Vector3i(1,0,-1) , sf::Vector3i(-1,0,1)}},
        // Figure -> 32
        //           10
        {{ sf::Vector3i(0,0,0)}}
    }};

    int m_changeIndex;
    // Représente le nombre de rotations faites depuis la configuration de référence
    // Une rotation à gauche est comptée positivement, une rotation à droite négativement
    int m_figNum;
    // Le numéro de la figure permettant d'identifier la forme de la pièce manipulée
    static constexpr std::array<int,7> m_nbPositionArray = {2,2,4,4,4,2,1};
    // Nombre de rotations différentes pour chaque pièce 
    // Ordre des pièces: Z, I, J, T, L, S, O
    // Exemple: la rotation de la pièce O ne change rien donc il n'y a qu'une rotation, cette dernière ne fait rien


public:
    TetrisFigure(float side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd,int figNum):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0),m_figNum(figNum-1)
    {
        if(figNum==1){
            // Je rajoute squaresIndex
            // Figure Z -> 23
            //              01
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex)); // carré 0 de la figure ci-dessus
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex+1,bottomOrdIndex)); // carré 1
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1)); // carré 2
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1)); // carré 3
            // Je trace  avec setSquares()
            setSquares();
            m_topOrdIndex = bottomOrdIndex-1;
            m_leftAbsIndex = bottomAbsIndex-1;
            m_rightAbsIndex = bottomAbsIndex+1;            
        }
        else if(figNum==2){
            // Je rajoute squaresIndex
            // Figure I -> 3
            //             2
            //             1
            //             0 
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-2));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-3));
            // Je trace  avec setSquares()
            setSquares();
            m_topOrdIndex = bottomOrdIndex -3;
            m_leftAbsIndex = bottomAbsIndex;
            m_rightAbsIndex = bottomAbsIndex;
        }
        else if(figNum==3){
            // Je rajoute squaresIndex
            // Figure J ->  2
            //              1
            //             30
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-2));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex));
            // Je trace  avec setSquares()
            setSquares();
            
            m_topOrdIndex = bottomOrdIndex-2;
            m_leftAbsIndex = bottomAbsIndex-1;
            m_rightAbsIndex = bottomAbsIndex;           
        }
        else if(figNum==4){
            // Je rajoute squaresIndex
            // Figure T -> 123
            //              0
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex+1,bottomOrdIndex-1));
            // Je trace  avec setSquares()
            setSquares();
            
            m_topOrdIndex =  bottomOrdIndex-1;
            m_leftAbsIndex = bottomAbsIndex-1;
            m_rightAbsIndex = bottomAbsIndex+1;
        }
        else if(figNum==5){
            // Je rajoute squaresIndex
            // Figure L -> 3
            //             2
            //             10
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-2));
            // Je trace  avec setSquares()
            setSquares();
        
            m_topOrdIndex = bottomOrdIndex-2;
            m_leftAbsIndex = bottomAbsIndex-1;
            m_rightAbsIndex = bottomAbsIndex;            
        }
        else if(figNum==6){
            // Je rajoute squaresIndex
            // Figure S ->  32
            //             10
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex+1,bottomOrdIndex-1));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
            // Je trace  avec setSquares()
            setSquares();
            
            m_topOrdIndex = bottomOrdIndex-1;
            m_leftAbsIndex = bottomAbsIndex-1;
            m_rightAbsIndex = bottomAbsIndex+1;
        }
        else if(figNum==7){
            // Figure O -> 32
            //             10
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
            // Je trace  avec setSquares()
            setSquares();
                    
            m_topOrdIndex = bottomOrdIndex-1;
            m_leftAbsIndex = bottomAbsIndex-1;
            m_rightAbsIndex = bottomAbsIndex;
        }

    }


    int getChangeIndex() const{
        return m_changeIndex;
    }

    void decreaseChangeIndex(){
        m_changeIndex--;
        if(m_changeIndex<0){
            m_changeIndex=m_nbPositionArray[m_figNum]-1;
        }
    }
    void increaseChangeIndex(){
        m_changeIndex++;
        m_changeIndex = m_changeIndex%m_nbPositionArray[m_figNum];
        }

    void setChangeIndex(int n){
        if(n>=0 && n<=m_nbPositionArray[m_figNum]-1){
            m_changeIndex=n;
        }
    }

    int getNbPosition() const{
        return m_nbPositionArray[m_figNum];
    }

    auto getChangeArray() const{
        return m_changeArray[m_figNum];
    }

    auto getChangeAbsArray() const{
        return m_changeAbsArray[m_figNum];
    }

    int getTopOrdIndex() const{
        return m_topOrdIndex;
    }

    void rotateLeft() {
        int size = getNbSquares();
        int changeIndex = getChangeIndex();
        auto changeArray = getChangeArray();
        for(int i=0; i<size;i++){
            m_squaresIndex[i] += changeArray[changeIndex][i];
            m_squaresVector[i].move(m_side *static_cast<sf::Vector2f> (changeArray[changeIndex][i]));
        }
        auto changeAbsArray = getChangeAbsArray();
        m_leftAbsIndex +=  changeAbsArray[changeIndex].x;
        m_rightAbsIndex += changeAbsArray[changeIndex].y;
        increaseChangeIndex();

    }

    void rotateRight() {
        decreaseChangeIndex();
        int size = getNbSquares();
        int changeIndex = getChangeIndex();
        auto changeArray = getChangeArray();
        for(int i=0; i<size;i++){
            m_squaresIndex[i] -=  changeArray[changeIndex][i];
            m_squaresVector[i].move(m_side * -1.0f * static_cast<sf::Vector2f> (changeArray[changeIndex][i]));
        }
        auto changeAbsArray = getChangeAbsArray();
        m_leftAbsIndex -=  changeAbsArray[changeIndex].x;
        m_rightAbsIndex -= changeAbsArray[changeIndex].y;
    }



};


class Generator{

static constexpr std::array<sf::Color,7> m_tetrisColor = { TetrisColor::Yellow , TetrisColor::Blue, TetrisColor::Red , TetrisColor::Magenta , TetrisColor::Green,TetrisColor::Orange,TetrisColor::Cyan};
public:

static std::shared_ptr<TetrisFigure> newTetrisFigure(int side,int bottomAbsIndex,int bottomOrdIndex,int bottomAbs, int bottomOrd){
    int figNum = 1 + (std::rand() % 7);
    sf::Color color = m_tetrisColor[std::rand()%7];
    return std::make_shared<TetrisFigure>(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd,figNum);
}

 // Le générateur de figure 
};


class NextScreen{

private:
    std::shared_ptr<TetrisFigure> m_nextFig;
    sf::RectangleShape m_nextScreen;
    sf::Font m_font;
    sf::Vector2f m_setFig;
    sf::Vector2f m_setNext;
public:

    NextScreen(sf::Vector2f position, sf::Vector2f size,int side, sf::Vector2f setFig = sf::Vector2f(0,0),sf::Vector2f setNext = sf::Vector2f(0.0f,0.0f)):m_setFig(setFig),m_setNext(setNext){
        m_nextScreen.setPosition(position);
        m_nextScreen.setSize(size);
        m_nextScreen.setFillColor(sf::Color::Black);
        if(!m_font.openFromFile("font/MegamaxJonathanToo-YqOq2.ttf")){
            std::cout << "font file non trouvé" << std::endl;
        }
        m_nextFig = Generator::newTetrisFigure(side,5,1,position.x+m_setFig.x,position.y+m_setFig.y);
    }
    void draw(sf::RenderWindow& window){
        sf::Text next(m_font);

        next.setString("NEXT : ");
        next.setFillColor(sf::Color::White);
        next.setPosition(sf::Vector2f(m_nextScreen.getPosition().x,m_nextScreen.getPosition().y)+m_setNext);
        window.draw(m_nextScreen);
        window.draw(next);
        m_nextFig->draw(window);
    }

    void moveFig(sf::Vector2f vec){
        m_nextFig->setNewCoord(vec,m_nextFig->getSide());
    }

    sf::Vector2f getPosition(){
        return m_nextScreen.getPosition();
    }

    std::shared_ptr<TetrisFigure> transfertFig(int newSide, int newBottomAbsIndex,int newBottomOrdIndex ,int newBottomAbs,int newBottomOrd){
        m_nextFig->setNewCoord(sf::Vector2f(newBottomAbs,newBottomOrd),newSide);
        std::shared_ptr<TetrisFigure> newsetFig = m_nextFig;
        m_nextFig.reset();
        m_nextFig = Generator::newTetrisFigure(newSide,newBottomAbsIndex,newBottomOrdIndex,m_nextScreen.getPosition().x + m_setFig.x , m_nextScreen.getPosition().y + m_setFig.y);
        return newsetFig;

    }
};

class GameBoard{
    // vector<std::array<Square,11>
    // 22 longueurs 10 largeurs // Il faut des cases en plus invisibles pour charger l'objet   
private:

    bool m_rotateOrTranslate = false;
    int m_nbRow=22;
    int m_nbColumn=10;
    float m_initX;
    float m_initY;
    float m_side;
    unsigned int m_level; 
    unsigned int m_score;
    sf::Clock m_timerToGoDown;
    sf::Time m_timeToGoDown;
    static constexpr std::array m_timeForLevels = {sf::milliseconds(1000),
                                                    sf::milliseconds(925),
                                                    sf::milliseconds(850),
                                                    sf::milliseconds(775),
                                                    sf::milliseconds(700),
                                                    sf::milliseconds(675),
                                                    sf::milliseconds(600),
                                                    sf::milliseconds(525),
                                                    sf::milliseconds(450),
                                                    sf::milliseconds(375),
                                                    sf::milliseconds(300),
                                                    sf::milliseconds(250),
                                                    sf::milliseconds(200),
                                                    sf::milliseconds(100),
                                                    sf::milliseconds(50)};
    std::shared_ptr<TetrisFigure> m_activFig;
    bool m_gameEnded = false;


    sf::RectangleShape m_rectangle;
    
    
    std::array<std::array<int,10>,22> m_isSquare = {{
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
    }};

public:
    std::vector<std::vector<Square>> m_gameBoard = std::vector<std::vector<Square>>(22, std::vector<Square>(10));

    GameBoard(float initX,float initY,float side):m_nbRow(22),m_nbColumn(10),m_initX(initX),m_initY(initY),m_side(side),m_level(1),m_timeToGoDown(sf::milliseconds(1000)),m_score(0){
        m_activFig = Generator::newTetrisFigure(m_side,5,1,m_initX+5*m_side,m_initY+1*m_side);
        m_rectangle.setFillColor(sf::Color::Black);
        m_rectangle.setPosition({m_initX-m_side/2.0f,m_initY-m_side/2.0f});
        m_rectangle.setSize({10*m_side,22*m_side});

        m_timerToGoDown.restart();
    }

    float getInitX()const{
        return m_initX;
    }

    float getInitY()const{
        return m_initY;
    }

    float getSide()const{
        return m_side;
    }

    int getScore()const{
        return m_score;
    }
    
    int getLevel()const{
        return m_level;
    }

    bool gameEnded() const{
        return m_gameEnded;
    }

    void autoGoDown(NextScreen& nextScreen){
        if(m_timerToGoDown.getElapsedTime()>m_timeToGoDown){
            goDown(nextScreen);
            m_timerToGoDown.restart();
        }
    }
    
    void goDown(NextScreen& nextScreen){
        m_activFig->goDown();
        auto squares = m_activFig->getSquaresIndex();
        if(isCollision()){
            m_activFig->goUp();
            if(m_rotateOrTranslate==true){
                return;
            }

            collision(nextScreen);
        }
    }

    void goLeft(){
        m_activFig->goLeft();
        if(isCollision()){
            rotateOrTranslate(false);
            m_activFig->goRight();
            return;
        }
        rotateOrTranslate(true);
    }


    void goRight(){
        m_activFig->goRight();
        if(isCollision()){
            rotateOrTranslate(false);
            m_activFig-> goLeft();
            return;
        }
        rotateOrTranslate(true);
    }

    void rotateLeft(){
        isLeftRotationCollision();
    }

    void rotateRight(){
        isRightRotationCollision();
    }

    void rotateOrTranslate(bool truth){
        m_rotateOrTranslate=truth;
    }

    bool isRotateOrTranslate() const{
        return m_rotateOrTranslate;
    }

    void draw(sf::RenderWindow& window){
        window.draw(m_rectangle);
        sf::VertexArray vLine(sf::PrimitiveType::Lines, 2);
        vLine[0].position = sf::Vector2f(m_rectangle.getPosition().x , m_rectangle.getPosition().y);
        vLine[1].position = sf::Vector2f(m_rectangle.getPosition().x , m_rectangle.getPosition().y+m_side*23);
        vLine[0].color = sf::Color::White;
        vLine[1].color = sf::Color::White;
        for(int i=0;i<=10;i++){
            window.draw(vLine);
            vLine[0].position += (sf::Vector2f(m_side,0));
            vLine[1].position += (sf::Vector2f(m_side,0));   
        }


        sf::VertexArray hLine(sf::PrimitiveType::Lines, 2);
        hLine[0].position = sf::Vector2f(m_rectangle.getPosition().x ,m_rectangle.getPosition().y );
        hLine[1].position = sf::Vector2f(m_rectangle.getPosition().x +10*m_side ,m_rectangle.getPosition().y);
        hLine[0].color = sf::Color::White;
        hLine[1].color = sf::Color::White;
        for(int j=0;j<=22;j++){
            window.draw(hLine);
            hLine[0].position += (sf::Vector2f(0,m_side));
            hLine[1].position += (sf::Vector2f(0,m_side));
        }
        for(int j=0;j<22;j++){
            for(int i=0 ;i<10;i++){
                if(m_isSquare[j][i]!=0){
                    window.draw(m_gameBoard[j][i]);
                }
            }
        }
        m_activFig->draw(window);
    
    }

private:


    void updateScore(std::vector<int> vec){
        if(vec.size()==1){
            m_score+=40*(m_level+1);
        }
        else if(vec.size()==2){
            m_score+=100*(m_level+1);
        }
        else if(vec.size()==3){
            m_score +=300*(m_level+1);
        }
        else if(vec.size()==4){
            m_score+=1200*(m_level+1);
        }

    }

    void updateLevel(){
        if (m_score>= 500 + 500*(m_level+1)*m_level/2){
            m_level++;
            updateTimerToGoDown();
        }
        m_timerToGoDown.restart();
    }

    void updateTimerToGoDown(){
        m_timeToGoDown = sf::Time( m_timeForLevels[m_level-1]);
       
    }
    bool completedRow(int row){
        for(int j=0;j<10;j++){
            if(m_isSquare[row][j]==0){
                return false;
            }
        }
        return true;
    }

    std::vector<int> getCompletedRow(){
        std::vector<int> vec;
        for(int i=m_activFig->getBottomOrdIndex();i>= m_activFig->getTopOrdIndex() ; i-- ){
            if(completedRow(i)){
                vec.emplace_back(i);
            }
        }
        return vec;
    }

    int internSwap(std::vector<int> vec){
        int emptyRow =1;
        int nonEmptyRow = 0;
        int bottomIndex = m_activFig->getBottomOrdIndex();
        for(int i=vec[0]-1;i>vec.back(); i--){
            auto it = std::find(vec.begin(), vec.end(), i);
            if(it==vec.end()){
                for(int j=0 ; j<10;j++){
                    if(m_isSquare[i][j]==1){
                        m_gameBoard[i][j].move(sf::Vector2f(0,m_side*(emptyRow)));
                     }
                }
                std::swap(m_isSquare[bottomIndex - nonEmptyRow],m_isSquare[i]);
                std::swap(m_gameBoard[bottomIndex - nonEmptyRow],m_gameBoard[i]);
                nonEmptyRow++;
            }
            else{
                emptyRow++;
            }
        }
        return nonEmptyRow;
    }

    void restingSwap(std::vector<int>vec,int nonEmptyRow){
        int bottomIndex = vec[0]-nonEmptyRow;
        int topIndex = vec.back() -1;
        for(int i=topIndex;i>=0;i--){
            for(int j=0 ; j<10;j++){
                if(m_isSquare[i][j]==1){
                    m_gameBoard[i][j].move(sf::Vector2f(0,vec.size()*m_side));
                }
            }
            std::swap(m_isSquare[bottomIndex + i-topIndex],m_isSquare[i]);
            std::swap(m_gameBoard[bottomIndex + i-topIndex],m_gameBoard[i]);            
        }
    }

    void erase(std::vector<int> vec){
        for(auto& value : vec){
            m_isSquare[value].fill(0);
            m_gameBoard[value].clear();
            m_gameBoard[value].resize(10);
        }
    }


    void collision(NextScreen& nextScreen){
        auto SquaresIndex = m_activFig->getSquaresIndex();
        if(m_activFig->getTopOrdIndex()<0 || (m_activFig->getBottomOrdIndex()==1 && m_activFig->getBottomAbsIndex()==5 &&  m_activFig->getBottomOrdIndex()-m_activFig->getTopOrdIndex()>=1)){
            m_gameEnded =true;
        }
        if(!m_gameEnded){
            for(int i=0 ; i <SquaresIndex.size();i++){
                m_gameBoard[SquaresIndex[i].y][SquaresIndex[i].x]= Square(m_side,m_activFig->getColor(),m_initX + SquaresIndex[i].x*m_side , m_initY + SquaresIndex[i].y*m_side  );
                m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x] = 1;
            }
            std::vector<int> rowCompleted = getCompletedRow();
            if(rowCompleted.size()>0){
                erase(rowCompleted);
                int nonEmptyRow = internSwap(rowCompleted);
                restingSwap(rowCompleted,nonEmptyRow);
                updateScore(rowCompleted);
                updateLevel();
            }
                m_activFig.reset();
                m_activFig = nextScreen.transfertFig(m_side,5,1,m_initX+5*m_side,m_initY+1*m_side);
        }
    }




    bool isCollision(){
        if(m_activFig->getRightAbsIndex()>9 || m_activFig->getLeftAbsIndex()<0 || m_activFig->getBottomOrdIndex()>21){
            return true;
        }
        auto SquaresIndex = m_activFig-> getSquaresIndex();
        for(int i=0 ; i<SquaresIndex.size();i++){
            if(SquaresIndex[i].y>=0){
                if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x]!=0){
                    return true;
                }
            }
        }
        return false;
    }

    bool isLeftRotationCollision(){
        m_activFig->rotateLeft();
        if(isCollision()){
            m_activFig->goRight();
            if(isCollision()){
                m_activFig->goRight();
                if(isCollision()){
                    m_activFig->goLeft();
                    m_activFig->goLeft();
                    m_activFig->rotateRight();
                    rotateOrTranslate(false);
                    return true;
                }
            }
        }
        rotateOrTranslate(true);
        return false;
    }


    bool isRightRotationCollision(){
        m_activFig->rotateRight();
        if(isCollision()){
            m_activFig->goRight();
            if(isCollision()){
                m_activFig->goRight();
                if(isCollision()){
                    m_activFig->goLeft();
                    m_activFig->goLeft();
                    m_activFig->rotateLeft();
                    rotateOrTranslate(false);
                    return true;
                }
            }
        }
        rotateOrTranslate(true);
        return false;
    }


};


class ScoreScreen{

private:

    sf::Font m_font;
    sf::RectangleShape m_scoreScreen;
    
    int m_score;
    bool m_getScore=false;
    sf::Vector2f m_setScore;

    int m_level;
    bool m_getLevel = false;
    sf::Vector2f m_setLevel;

public:

    ScoreScreen(sf::Vector2f position, sf::Vector2f size,sf::Vector2f setScore = sf::Vector2f(0.0f,0.0f),sf::Vector2f setLevel = sf::Vector2f(0.0f,0.0f)):m_score(0),m_setScore(setScore),m_setLevel(setLevel){

        m_scoreScreen.setPosition(position);
        m_scoreScreen.setSize(size);
        m_scoreScreen.setFillColor(sf::Color::Black);
        if(!m_font.openFromFile("font/MegamaxJonathanToo-YqOq2.ttf")){
            std::cout << "font file non trouvé" << std::endl;
        }

    }


void setScore(unsigned int score){
    m_score = score;
    m_getScore = true;
}

void setLevel(unsigned int level){
    m_level = level;
    m_getLevel = true;
}
void setSize(const sf::Vector2f& vec){
    m_scoreScreen.setSize(vec);
}

void setPosition(const sf::Vector2f& vec){
    m_scoreScreen.setPosition(vec);
}

void moveScore(const sf::Vector2f& vec){
    m_setScore = m_setScore + vec;
}


sf::Vector2f getPosition()const{
    return m_scoreScreen.getPosition();
}

void draw(sf::RenderWindow& window){
    window.draw(m_scoreScreen);
    if(m_getScore){
        std::string strScore = std::to_string(m_score);
        strScore = "Score : " + strScore;
        sf::Text score(m_font);
        score.setString(strScore);
        score.setFillColor(sf::Color::White);
        score.setPosition(sf::Vector2f(m_scoreScreen.getPosition().x,m_scoreScreen.getPosition().y)+m_setScore);
        window.draw(score);
    }
    if(m_getLevel){
        std::string strLevel = std::to_string(m_level);
        strLevel = "Level : " + strLevel; 
        sf::Text level(m_font);
        level.setString(strLevel);
        level.setFillColor(sf::Color::White);
        level.setPosition(sf::Vector2f(m_scoreScreen.getPosition().x,m_scoreScreen.getPosition().y)+m_setLevel);
        window.draw(level);
    }
}

};



class ScreenGame{

    NextScreen nextScreen;
    ScoreScreen scoreScreen;
    GameBoard gameBoard;

public:

    ScreenGame(float initX,float initY,float side):gameBoard(initX,initY,side),nextScreen(sf::Vector2f(initX+side*12,initY+side*13),sf::Vector2f(10*side,6*side),side,sf::Vector2f(8*side,4*side),sf::Vector2f(side,3*side)),scoreScreen(sf::Vector2f(initX+side*12,initY+side*3),sf::Vector2f(10*side,6*side),sf::Vector2f(0.5*side,3*side),sf::Vector2f(0.5*side,1*side)){
        scoreScreen.setLevel(0);
        scoreScreen.setScore(0);
    }

    void goDown(){
        gameBoard.goDown(nextScreen);
        // goDown( (std::shared_ptr<TetrisFigure>) (*foo) (int a, int b, int c, int d, int e) = Generator::newTetrisFigure)
    }

    void autoGoDown(){
        gameBoard.autoGoDown(nextScreen);
    }

    void goLeft(){
        gameBoard.goLeft();
    }

    void goRight(){
        gameBoard.goRight();
    }


    void rotateLeft(){
        gameBoard.rotateLeft();
    }

    void rotateRight(){
        gameBoard.rotateRight();
    }

    bool gameEnded() const{
        return gameBoard.gameEnded();
    }

    void rotateOrTranslate(bool truth){
        gameBoard.rotateOrTranslate(truth);
    }

    bool isRotateOrTranslate(){
        return gameBoard.isRotateOrTranslate();
    }

    void draw(sf::RenderWindow& window){
        scoreScreen.setScore(gameBoard.getScore()); 
        scoreScreen.setLevel(gameBoard.getLevel());
        gameBoard.draw(window);
        scoreScreen.draw(window);
        nextScreen.draw(window);

    }

};



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
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::Right){
                            gameboard.goRight();
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::H){
                            gameboard.rotateLeft();
                        }
                        if(UpKey->scancode == sf::Keyboard::Scancode::J){
                            gameboard.rotateRight();
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

