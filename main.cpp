#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>


/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */
/********************************-------Déclaration des différentes formes de figures---------------------*************************************/
/******************************************************************************************************************************************** */
/******************************************************************************************************************************************** */


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

private:
    sf::VertexArray m_vertices;
    size_t m_side;
    size_t m_initX;
    size_t m_initY;
    sf::Color m_color;


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
    size_t m_side;
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
        {{ sf::Vector3i(1,0,1) , sf::Vector3i(-1,-1,-1), sf::Vector3i(1,1,1), sf::Vector3i(-1,0,-1)}},
        // Figure -> 123
        //            0
        {{ sf::Vector3i(-1,0,-1) , sf::Vector3i(1,0,1), sf::Vector3i(-1,0,-1), sf::Vector3i(1,0,1)}},
        // Figure -> 3
        //           2
        //           10
        {{ sf::Vector3i(1,0,1) , sf::Vector3i(-1,0,-1), sf::Vector3i(1,0,1), sf::Vector3i(-1,0,-1)}},
        // Figure ->  32
        //           10
        {{ sf::Vector3i(-1,0,-1) , sf::Vector3i(1,0,1)}},
        // Figure -> 32
        //           10
        {{ sf::Vector3i(0,0,0)}}
    }};

    int m_changeIndex;
    int m_figNum;
    static constexpr std::array<int,7> m_nbPositionArray = {2,2,4,4,2,1};


public:
    TetrisFigure(size_t side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd,int figNum):Figure(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd),m_changeIndex(0),m_figNum(figNum-1)
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
            m_leftAbsIndex = bottomAbsIndex;
            m_rightAbsIndex = bottomAbsIndex+1;           
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


class GameBoard{
    // vector<std::array<Square,11>
    // 22 longueurs 10 largeurs // Il faut des cases en plus invisibles pour charger l'objet   
private:

    
    size_t m_nbRow;
    size_t m_nbColumn;
    size_t m_initX;
    size_t m_initY;
    size_t m_side;
    unsigned int m_level;
    std::shared_ptr<TetrisFigure> m_activFig;
    
    // Ou alors std::array<std::array<Square,10>,23> tableau_m (j'ai mis 23 car on aura deux case à cacher); 
    // ON va lui mettre un std::vector de figure (des pointeurs) 


public:

    std::vector<std::vector<Square>> m_gameBoard;


    GameBoard(size_t nbRow,size_t nbColumn,size_t initX,size_t initY,size_t side):m_nbRow(nbRow),m_nbColumn(nbColumn),m_initX(initX),m_initY(initY),m_side(side),m_level(0){
        m_gameBoard.resize(m_nbRow, std::vector<Square>(m_nbColumn, Square(m_side))); 
             
    }

    bool goDown(){
        if(!isBottomCollision()){
            m_activFig->goDown();
            return true;
        }
        collision();
        return false;
    }

    bool goLeft(){
        if(!isLeftCollision()){
            m_activFig->goLeft();
            return true;
        }
        return false;
    }

    bool goRight(){
        if(!isRightCollision()){
            m_activFig->goRight();
            return true;
        }
        return false;
    }

    sf::Vector2f getInit() const{
        return sf::Vector2f(m_initX,m_initY);
    }

    unsigned int getLevel() const{
        return m_level;
    }

    void draw(sf::RenderWindow& window ){
        for(int i=0;i<m_nbRow;i++){
            for(int j=0;j<m_nbRow;j++){
                window.draw(m_gameBoard[i][j]);
            }
        }
        m_activFig->draw(window);
    }

    

private:

    bool isBottomCollision(){
        int bottomOrdIndex = m_activFig->getBottomOrdIndex();
        if(bottomOrdIndex == m_nbRow-1){
            return true;
        }
        auto squaresIndex = m_activFig->getSquaresIndex();
        for(int i=0; i<squaresIndex.size(); i++){
            if(squaresIndex[i].y==bottomOrdIndex  && m_gameBoard[squaresIndex[i].x][squaresIndex[i].y+1] != Square(m_side)  ){
                return true;
            }
        }
        return false;
    }

    bool isLeftCollision(){
        int leftAbsIndex = m_activFig->getLeftAbsIndex();
        if(leftAbsIndex <= 0){
            return true;
        }
        auto squaresIndex = m_activFig->getSquaresIndex();
        for(int i=0;i<squaresIndex.size();i++){
            if(squaresIndex[i].x==leftAbsIndex  && m_gameBoard[squaresIndex[i].x-1][squaresIndex[i].y] != Square(m_side)  ){
                return true;
            }
        }
        return false;            
    }

    bool isRightCollision(){
        int rightAbsIndex = m_activFig->getRightAbsIndex();
        if(rightAbsIndex >= m_nbColumn-1){
            return true;
        }
        auto squaresIndex = m_activFig->getSquaresIndex();
        for(int i=0;i<squaresIndex.size();i++){
            if(squaresIndex[i].x==rightAbsIndex  && m_gameBoard[squaresIndex[i].x+1][squaresIndex[i].y] != Square(m_side)  ){
                return true;
            }
        }
        return false;            
    }


    void collision(){
        auto squaresIndex = m_activFig->getSquaresIndex();
        for(int i=0 ; i<squaresIndex.size();i++){
            m_gameBoard[squaresIndex[i].x][squaresIndex[i].y] = Square(m_side,m_activFig->getColor(),m_initX + squaresIndex[i].x*m_side , m_initY + squaresIndex[i].y*m_side  );
        }

        m_activFig.reset();
    }



};

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

class Generator{

static constexpr std::array<sf::Color,7> m_tetrisColor = { TetrisColor::Yellow , TetrisColor::Blue, TetrisColor::Red , TetrisColor::Magenta , TetrisColor::Green,TetrisColor::Orange,TetrisColor::Cyan};
public:

std::shared_ptr<TetrisFigure> newTetrisFigure(int side,int bottomAbsIndex,int bottomOrdIndex,int bottomAbs, int bottomOrd){
    int figNum = 1 + (std::rand() % 7);
    sf::Color color = m_tetrisColor[std::rand()%7];
    return std::make_shared<TetrisFigure>(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd,figNum);
}
 // Le générateur de figure 
};

int main(){
    std::srand(std::time(nullptr));
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

    TetrisFigure FirstFig1(50,sf::Color::Red,32,32,150,150,3);
    
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
                    FirstFig1.rotateLeft();
                }
                if(UpKey->scancode == sf::Keyboard::Scancode::J){
                    FirstFig1.rotateRight();
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

