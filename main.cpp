#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>


/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */
/********************************-------Déclaration des différentes formes de figures---------------------*************************************/
/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */

class Generator;

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


class Square: public sf::Drawable, public sf::Transformable { 

private:
    sf::VertexArray m_vertices;
    float m_side;
    float m_initX;
    float m_initY;
    sf::Color m_color;


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
    float m_side;
    std::vector<Square> m_squaresVector; // Je crois qu'on en a pas besoin
    std::vector<sf::Vector2i> m_squaresIndex;
    int m_bottomAbsIndex;
    int m_bottomOrdIndex;
    int m_leftAbsIndex;
    int m_rightAbsIndex;
    int m_topOrdIndex;
    int m_bottomAbs;
    int m_bottomOrd;

    // On va créer pour chaque classe fille une initialisation


public:

    Figure(float side,sf::Color color,int bottomAbsIndex, int bottomOrdIndex,int bottomAbs,int bottomOrd):m_side(side),m_color(color),m_bottomAbsIndex(bottomAbsIndex),m_bottomOrdIndex(bottomOrdIndex),m_bottomAbs(bottomAbs),m_bottomOrd(bottomOrd){}
    
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
        m_topOrdIndex +=1;
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

    int getBottomAbs() const{
        return m_bottomAbs;
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

    
};


class TetrisFigure: public Figure{

private:

    static constexpr std::array<std::array<std::array<sf::Vector2i, 4>, 4>, 7> m_changeArray = {{
        {{   
        // Figure -> 23
        //            01
            {{sf::Vector2i(0, 0), sf::Vector2i(0, -2), sf::Vector2i(2, 0), sf::Vector2i(0, 0)}},{{sf::Vector2i(0, 0), sf::Vector2i(0, 2), sf::Vector2i(-2, 0), sf::Vector2i(0, 0)}}   
        }},
        {{   
        // Figure -> 3
        //           2
        //           1
        //           0 
            {{sf::Vector2i(0, 0), sf::Vector2i(1, 1), sf::Vector2i(-1, 2), sf::Vector2i(-2, 3)}},{{sf::Vector2i(0, 0), sf::Vector2i(-1, -1), sf::Vector2i(1, -2), sf::Vector2i(2, -3)}}   
        }},
        {{   
        // Figure ->  2
        //            1
        //           30 
            {{sf::Vector2i(0, 0), sf::Vector2i(-2, 0), sf::Vector2i(-2, 2), sf::Vector2i(0, 0)}},{{sf::Vector2i(0, -2), sf::Vector2i(1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, -2)}},{{sf::Vector2i(-2, 1), sf::Vector2i(1, 0), sf::Vector2i(1, 0),sf::Vector2i(0, 1) }},{{sf::Vector2i(2, 1), sf::Vector2i(0, 0), sf::Vector2i(0, -2), sf::Vector2i(0, 1)}}      
        }},
        {{   
        // Figure -> 123
        //            0
            {{sf::Vector2i(1, 0), sf::Vector2i(2, -1), sf::Vector2i(0, 0), sf::Vector2i(0,0)}},{{sf::Vector2i(0, 0), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1)}}, {{sf::Vector2i(0, -1), sf::Vector2i(0,0), sf::Vector2i(1, -2), sf::Vector2i(0, 0)}},{{sf::Vector2i(-1, 1), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(1, -1)}}  
        }},
        {{   
        // Figure -> 3
        //           2
        //           10
            {{sf::Vector2i(0,-1), sf::Vector2i(0, -1), sf::Vector2i(-1, 1), sf::Vector2i(-1, 1)}},{{sf::Vector2i(0, 0), sf::Vector2i(1, -1), sf::Vector2i(2, 0), sf::Vector2i(1, -1)}},{{sf::Vector2i(0, 0), sf::Vector2i(-1, 2), sf::Vector2i(0, 0), sf::Vector2i(-1, 2)}},{{sf::Vector2i(0, 1), sf::Vector2i(0, 0), sf::Vector2i(-1, -1), sf::Vector2i(1, -2)}}       
        }},
        {{   
        // Figure ->  32
        //           10
            {{sf::Vector2i(0, -2), sf::Vector2i(2, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)}},{{sf::Vector2i(0, 2), sf::Vector2i(-2, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)}}   
        }},
        {{   
        // Figure -> 32
        //           10
            {{sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0), sf::Vector2i(0, 0)}}   
        }}
    }};

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
    int m_figNum;
    static constexpr std::array<int,7> m_nbPositionArray = {2,2,4,4,4,2,1};


public:
    TetrisFigure(float side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd,int figNum):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0),m_figNum(figNum-1)
    {
        if(figNum==1){
            // Je rajoute squaresIndex
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex+1,bottomOrdIndex));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex-1,bottomOrdIndex-1));
            m_squaresIndex.emplace_back(sf::Vector2i(bottomAbsIndex,bottomOrdIndex-1));
            // Je trace  avec setSquares()
            setSquares();
            m_topOrdIndex = bottomOrdIndex-1;
            m_leftAbsIndex = bottomAbsIndex-1;
            m_rightAbsIndex = bottomAbsIndex+1;            
        }
        else if(figNum==2){
            // Je rajoute squaresIndex
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


class GameBoard{
    // vector<std::array<Square,11>
    // 22 longueurs 10 largeurs // Il faut des cases en plus invisibles pour charger l'objet   
private:

    
    int m_nbRow=22;
    int m_nbColumn=10;
    float m_initX;
    float m_initY;
    float m_side;
    unsigned int m_level;
    unsigned int m_score;
    sf::Clock m_timerToGoDown;
    sf::Time m_timeToGoDown;
    std::shared_ptr<TetrisFigure> m_activFig;
    sf::RectangleShape m_rectangle;
    // Ou alors std::array<std::array<Square,10>,23> tableau_m (j'ai mis 23 car on aura deux case à cacher); 
    // ON va lui mettre un std::vector de figure (des pointeurs) 
    
    
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

    GameBoard(float initX,float initY,float side):m_nbRow(22),m_nbColumn(10),m_initX(initX),m_initY(initY),m_side(side),m_level(1),m_timeToGoDown(sf::milliseconds(1000)){
        m_activFig = Generator::newTetrisFigure(m_side,5,1,m_initX+5*m_side,m_initY+0*m_side);
        m_rectangle.setFillColor(sf::Color::Black);
        m_rectangle.setPosition({m_initX-m_side/2.0f,m_initY-m_side/2.0f});
        m_rectangle.setSize({10*m_side,22*m_side});
    }

    void goDown(){
        if(isBottomCollision()){
            collision();
            return;
        }
        m_activFig -> goDown();
    }

    void goLeft(){
        std::cout << "Indice avant left move" << m_activFig->getLeftAbsIndex() << std::endl;
        if(!isLeftCollision()){
            m_activFig -> goLeft();
            std::cout << "Indice apres left move" << m_activFig->getLeftAbsIndex() << std::endl;
        }
    }

    void goRight(){
        std::cout << "Indice avant left move" << m_activFig->getRightAbsIndex() << std::endl;
        if(!isRightCollision()){
            m_activFig-> goRight();
            std::cout << "Indice apres left move" << m_activFig->getRightAbsIndex() << std::endl;
        }
    }

    void rotateLeft(){
        isLeftRotationCollision();
    }

    void rotateRight(){
        isRightRotationCollision();
    }

    void draw(sf::RenderWindow& window){
        window.draw(m_rectangle);
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


    void collision(){
        auto SquaresIndex = m_activFig->getSquaresIndex();
        for(int i=0 ; i <SquaresIndex.size();i++){
            m_gameBoard[SquaresIndex[i].y][SquaresIndex[i].x]= Square(m_side,m_activFig->getColor(),m_initX + SquaresIndex[i].x*m_side , m_initY + SquaresIndex[i].y*m_side  );
            m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x] = 1;
        }
        m_activFig.reset();
        m_activFig = Generator::newTetrisFigure(m_side,5,1,m_initX+5*m_side,m_initY+0*m_side);
    }


    bool isBottomCollision(){
        if(m_activFig->getBottomOrdIndex()>=21){
            return true;
        }
        auto SquaresIndex = m_activFig -> getSquaresIndex();
        for(int i=0;i<SquaresIndex.size();i++){
            if(SquaresIndex[i].y>=0){
                if(m_isSquare[SquaresIndex[i].y+1][SquaresIndex[i].x]!=0){
                    return true;
                }
            }
        }
        return false;
    }

    bool isLeftCollision(){
        if(m_activFig->getLeftAbsIndex()<=0){
            return true;
        }
        auto SquaresIndex = m_activFig -> getSquaresIndex();
        for(int i=0;i<SquaresIndex.size();i++){
            if(SquaresIndex[i].y>=0){
                if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x-1]!=0){
                    return true;
                }
            }
        }
        return false;
    }

    bool isRightCollision(){
        if(m_activFig->getRightAbsIndex()>=9){
            return true;
        }
        auto SquaresIndex = m_activFig-> getSquaresIndex();
        for(int i=0 ; i<SquaresIndex.size();i++){
            if(SquaresIndex[i].y>=0){
                if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x+1]!=0){
                    return true;
                }
            }
        }
        return false;
    }

    bool isLeftRotationCollision(){
        m_activFig->rotateLeft();
        if(m_activFig->getLeftAbsIndex()<0){
            m_activFig->goRight();
            if(m_activFig->getLeftAbsIndex()<0){
                m_activFig->goRight();
                auto SquaresIndex = m_activFig->getSquaresIndex();
                for(int i=0 ; i<SquaresIndex.size();i++){
                    if(SquaresIndex[i].y>=0){
                        if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x]!=0){
                            m_activFig -> goLeft();
                            m_activFig -> goLeft();
                            m_activFig -> rotateRight();
                            return true;
                        }
                    }
                }
                return false;
            }
            else{
                auto SquaresIndex = m_activFig->getSquaresIndex();
                for(int i=0 ;i<SquaresIndex.size();i++){
                    if(SquaresIndex[i].y>=0){
                        if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x]!=0){
                            m_activFig->goRight();
                            SquaresIndex = m_activFig->getSquaresIndex();
                            for(int j=0;j<SquaresIndex.size();j++){
                                if(SquaresIndex[j].y>=0){
                                    if(m_isSquare[SquaresIndex[j].y][SquaresIndex[j].x]!=0){
                                        m_activFig->goLeft();
                                        m_activFig->goLeft();
                                        m_activFig->rotateRight();
                                        return false;
                                    }
                                }
                            }
                        }
                    }                    
                }
                return true;
            }
        }
        else{
            auto SquaresIndex = m_activFig->getSquaresIndex();
            for(int i=0; i <SquaresIndex.size();i++){
                if(SquaresIndex[i].y>=0){
                    if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x]!=0){
                        m_activFig->goRight();
                        SquaresIndex = m_activFig->getSquaresIndex();
                        for(int j=0; j <SquaresIndex.size();j++){
                            if(SquaresIndex[j].y>=0){
                                if(m_isSquare[SquaresIndex[j].y][SquaresIndex[j].x]!=0){
                                    m_activFig->goRight();
                                    SquaresIndex = m_activFig->getSquaresIndex();
                                    for(int l=0;l<SquaresIndex.size();l++){
                                        if(SquaresIndex[l].y>=0){
                                            if(m_isSquare[SquaresIndex[l].y][SquaresIndex[l].x]!=0){
                                                m_activFig->goLeft();
                                                m_activFig->goLeft();
                                                m_activFig->rotateRight();
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        return false;
                    }
                }
            }
            return false;
        }   
    }


    bool isRightRotationCollision(){
        m_activFig->rotateRight();
        if(m_activFig->getLeftAbsIndex()<0){
            m_activFig->goRight();
            if(m_activFig->getLeftAbsIndex()<0){
                m_activFig->goRight();
                auto SquaresIndex = m_activFig->getSquaresIndex();
                for(int i=0 ; i<SquaresIndex.size();i++){
                    if(SquaresIndex[i].y>=0){
                        if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x]!=0){
                            m_activFig -> goLeft();
                            m_activFig -> goLeft();
                            m_activFig -> rotateRight();
                            return true;
                        }
                    }
                }
                return false;
            }
            else{
                auto SquaresIndex = m_activFig->getSquaresIndex();
                for(int i=0 ;i<SquaresIndex.size();i++){
                    if(SquaresIndex[i].y>=0){
                        if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x]!=0){
                            m_activFig->goRight();
                            SquaresIndex = m_activFig->getSquaresIndex();
                            for(int j=0;j<SquaresIndex.size();j++){
                                if(SquaresIndex[j].y>=0){
                                    if(m_isSquare[SquaresIndex[j].y][SquaresIndex[j].x]!=0){
                                        m_activFig->goLeft();
                                        m_activFig->goLeft();
                                        m_activFig->rotateRight();
                                        return false;
                                    }
                                }
                            }
                        }
                    }                    
                }
                return true;
            }
        }
        else{
            auto SquaresIndex = m_activFig->getSquaresIndex();
            for(int i=0; i <SquaresIndex.size();i++){
                if(SquaresIndex[i].y>=0){
                    if(m_isSquare[SquaresIndex[i].y][SquaresIndex[i].x]!=0){
                        m_activFig->goRight();
                        SquaresIndex = m_activFig->getSquaresIndex();
                        for(int j=0; j <SquaresIndex.size();j++){
                            if(SquaresIndex[j].y>=0){
                                if(m_isSquare[SquaresIndex[j].y][SquaresIndex[j].x]!=0){
                                    m_activFig->goRight();
                                    SquaresIndex = m_activFig->getSquaresIndex();
                                    for(int l=0;l<SquaresIndex.size();l++){
                                        if(SquaresIndex[l].y>=0){
                                            if(m_isSquare[SquaresIndex[l].y][SquaresIndex[l].x]!=0){
                                                m_activFig->goLeft();
                                                m_activFig->goLeft();
                                                m_activFig->rotateRight();
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        return false;
                    }
                }
            }
            return false;
        }   
    }




};




int main(){
    std::srand(std::time(nullptr));
    sf::RenderWindow window(sf::VideoMode({1200,800}),"Fenetre de test");

    window.setPosition({0,0});

    sf::RectangleShape rectangle({1200.f, 800.f});
    rectangle.setFillColor(sf::Color::White);
    GameBoard gameboard(200,50,32);
    //TetrisFigure fig(50,sf::Color::Red,0,0,500,400,3);
    //window.setFramerateLimit(60);   
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
                    // std::cout << fig.getLeftAbsIndex() << std::endl;
                    // std::cout << fig.getRightAbsIndex() << std::endl;
                    // std::cout << fig.getTopOrdIndex() << std::endl;
                    // std::cout << fig.getBottomOrdIndex() << std::endl;
                }
                if(UpKey->scancode == sf::Keyboard::Scancode::J){
                    gameboard.rotateRight();
                    // fig.rotateRight();
                    // std::cout << fig.getLeftAbsIndex() << std::endl;
                    // std::cout << fig.getRightAbsIndex() << std::endl;
                    // std::cout << fig.getTopOrdIndex() << std::endl;
                    // std::cout << fig.getBottomOrdIndex() << std::endl;
                }
            }
        }
        window.clear();
        window.draw(rectangle);
        gameboard.draw(window);     
        //fig.draw(window);
        window.display();
    }
    return 0;
}

