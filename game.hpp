#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>

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

// Multiplication d'un vecteur par un scalaire
sf::Vector2i operator * (const float& side,const sf::Vector2i v);
sf::Vector2f operator * (const float& side,const sf::Vector2f v);
sf::Vector2i operator * (const sf::Vector2i v,const float& side);
sf::Vector2f operator * (const sf::Vector2f v,const float& side);



// Classe des carrés élémentaires
class Square: public sf::Drawable, public sf::Transformable { 

public:

    Square():m_side(0){}


    Square(float newSide, sf::Color newcolor = sf::Color::Black, float newinitX = 0,float newinitY=0);


    float getNewInitX() const;
    float getNewInitY() const;
    float getSide()const;
    sf::Color getColor() const;

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
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


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


};



class Figure {


public:

    Figure(float side,sf::Color color,int bottomAbsIndex, int bottomOrdIndex,int bottomAbs,int bottomOrd);
    void goUp();

    void goLeft();

    void goRight();

    void goDown();

    sf::Color getColor()const;
    
    auto getSquaresIndex() const;

    void printSquaresIndex() const;

    void setSquares();

    int getNbSquares() const;

    void draw(sf::RenderWindow& window);


    int getBottomOrdIndex() const;

    int getBottomAbsIndex() const;

    int getBottomAbs() const;

    int getBottomOrd() const;

    int getLeftAbsIndex() const;

    int getRightAbsIndex() const;
    
    int getSide()const;

    void printPosition();


    sf::Vector2f getPosition() const;

    void setBottomAbs(const int &newBottomAbs);
    
    void setBottomOrd(const int &newBottomOrd);

    void setNewCoord(sf::Vector2f newCoord,float newSide);

    // virtual void RotateLeft() = 0; // Chaque classe fille va implémenter sa rotation
    // virtual void RotateRight() = 0; // Chaque classe fille va implémenter sa rotation

    // Une figure c'est plusieurs carrés qu'on va générer avec des vertex et des triangles
    // C'est également une abscisse et une ordonnée pour chaque carré

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

    
};


class TetrisFigure: public Figure{

private:


public:
    TetrisFigure(float side, sf::Color color,int bottomAbsIndex, int bottomOrdIndex, int bottomAbs, int bottomOrd,int figNum);


    int getChangeIndex() const;

    void decreaseChangeIndex();

    void increaseChangeIndex();

    void setChangeIndex(int n);

    int getNbPosition() const;

    auto getChangeArray() const;

    auto getChangeAbsArray() const;

    int getTopOrdIndex() const;

    void rotateLeft();

    void rotateRight();

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


};
    

class Generator{

public:

static std::shared_ptr<TetrisFigure> newTetrisFigure(int side,int bottomAbsIndex,int bottomOrdIndex,int bottomAbs, int bottomOrd){
    int figNum = 1 + (std::rand() % 7);
    sf::Color color = m_tetrisColor[std::rand()%7];
    return std::make_shared<TetrisFigure>(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd,figNum);
}

private:
static constexpr std::array<sf::Color,7> m_tetrisColor = { TetrisColor::Yellow , TetrisColor::Blue, TetrisColor::Red , TetrisColor::Magenta , TetrisColor::Green,TetrisColor::Orange,TetrisColor::Cyan};

    // Le générateur de figure 
};


class NextScreen{


public:

    NextScreen(sf::Vector2f position, sf::Vector2f size,int side, sf::Vector2f setFig = sf::Vector2f(0,0),sf::Vector2f setNext = sf::Vector2f(0.0f,0.0f));

    void draw(sf::RenderWindow& window);

    void moveFig(sf::Vector2f vec);

    sf::Vector2f getPosition();

    std::shared_ptr<TetrisFigure> transfertFig(int newSide, int newBottomAbsIndex,int newBottomOrdIndex ,int newBottomAbs,int newBottomOrd);

private:
    std::shared_ptr<TetrisFigure> m_nextFig;
    sf::RectangleShape m_nextScreen;
    sf::Font m_font;
    sf::Vector2f m_setFig;
    sf::Vector2f m_setNext;
};



class GameBoard{
// vector<std::array<Square,11>
// 22 longueurs 10 largeurs // Il faut des cases en plus invisibles pour charger l'objet   


public:
std::vector<std::vector<Square>> m_gameBoard = std::vector<std::vector<Square>>(22, std::vector<Square>(10));

GameBoard(float initX,float initY,float side);

float getInitX()const;

float getInitY()const;

float getSide()const;

int getScore()const;

int getLevel()const;

bool gameEnded() const;

void autoGoDown(NextScreen& nextScreen);

void goDown(NextScreen& nextScreen);

void goLeft();


void goRight();

void rotateLeft();

void rotateRight();

void rotateOrTranslate(bool truth);

bool isRotateOrTranslate() const;

void draw(sf::RenderWindow& window);

private: 

void updateScore(std::vector<int> vec);

void updateLevel();

void updateTimerToGoDown();

bool completedRow(int row);

std::vector<int> getCompletedRow();

int internSwap(std::vector<int> vec);

void restingSwap(std::vector<int>vec,int nonEmptyRow);

void erase(std::vector<int> vec);



void collision(NextScreen& nextScreen);



bool isCollision();

bool isLeftRotationCollision();

bool isRightRotationCollision();

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


};


class ScoreScreen{


public:

ScoreScreen(sf::Vector2f position, sf::Vector2f size,sf::Vector2f setScore = sf::Vector2f(0.0f,0.0f),sf::Vector2f setLevel = sf::Vector2f(0.0f,0.0f));


void setScore(unsigned int score);

void setLevel(unsigned int level);

void setSize(const sf::Vector2f& vec);

void setPosition(const sf::Vector2f& vec);

void moveScore(const sf::Vector2f& vec);


sf::Vector2f getPosition()const;

void draw(sf::RenderWindow& window);

private:

    sf::Font m_font;
    sf::RectangleShape m_scoreScreen;
    
    int m_score;
    bool m_getScore=false;
    sf::Vector2f m_setScore;

    int m_level;
    bool m_getLevel = false;
    sf::Vector2f m_setLevel;


};


class ScreenGame{

public:

    ScreenGame(float initX,float initY,float side);


    void draw(sf::RenderWindow& window);

    NextScreen nextScreen;
    ScoreScreen scoreScreen;
    GameBoard gameBoard;

};
    