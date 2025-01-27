#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "game.hpp"

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
float Square::getNewInitX() const{
    return m_initX;
}

float Square::getNewInitY() const{
    return m_initY;
}

float Square::getSide()const{
    return m_side;
}

sf::Color Square::getColor() const{
    return m_color;
}

bool Square::operator ==(const Square &it){
    if(it.getColor() == getColor() && it.getSide()==m_side){
        return true;
    }
    return false;
}

bool Square::operator !=(const Square &it){
    if(it.getColor() != getColor() || it.getSide()!=m_side){
        return true;
    }
    return false;
}

void Square::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // draw the vertex array
    target.draw(m_vertices, states);
}


// Classe Figure
void Figure::goUp(){
    int size = m_squaresIndex.size();
    for(int i=0; i<size;i++){
        m_squaresIndex[i].y -=1;
        m_squaresVector[i].move(m_side * sf::Vector2f(0,-1));
    }
    m_bottomOrdIndex -=1;
    m_topOrdIndex -=1;
    m_bottomOrd -= m_side;
}

void Figure::goLeft(){
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

void Figure::goRight(){
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

void Figure::goDown(){
    int size = m_squaresIndex.size();
    for(int i=0; i<size;i++){
        m_squaresIndex[i].y +=1;
        m_squaresVector[i].move(m_side * sf::Vector2f(0,1));
    }
    m_bottomOrdIndex +=1;
    m_topOrdIndex +=1;
    m_bottomOrd += m_side;
}

sf::Color Figure::getColor()const{
    return m_color;
}
auto Figure::getSquaresIndex() const{
    return m_squaresIndex;
}

void Figure::printSquaresIndex() const{
    for(int i=0 ; i<m_squaresIndex.size(); i++){
        std::cout << m_squaresIndex[i].x << " " << m_squaresIndex[i].y << std::endl;
    }
}

void Figure::setSquares(){
    for(int i=0 ; i<m_squaresIndex.size();i++){
        m_squaresVector.emplace_back(Square(m_side,m_color,m_bottomAbs + m_side * (m_squaresIndex[i].x-m_bottomAbsIndex) , m_bottomOrd + m_side * (m_squaresIndex[i].y-m_bottomOrdIndex) ));
    }
}

int Figure::getNbSquares() const{
    return m_squaresVector.size();
}


void Figure::draw(sf::RenderWindow& window){
    int size = m_squaresVector.size();
    for (int i=0;i<size;i++){
        window.draw(m_squaresVector[i]);
    } 
} // En gros grâce à m_origin m_originIndex m_side et m_squaresIndex on peut tout tracer;


int Figure::getBottomOrdIndex() const{
    return m_bottomOrdIndex;
}

int Figure::getBottomAbsIndex() const{
    return m_bottomAbsIndex;
}

int Figure::getBottomAbs() const{
    return m_bottomAbs;
}

int Figure::getBottomOrd() const{
    return m_bottomOrd;
}

int Figure::getLeftAbsIndex() const{
    return m_leftAbsIndex;
}

int Figure::getRightAbsIndex() const{
    return m_rightAbsIndex;
}

int Figure::getSide()const{
    return m_side;
}

void Figure::printPosition(){
    for(int i=0;i<m_squaresVector.size();i++){
        std::cout << i << " : " << m_squaresVector[i].getPosition().x << " " << m_squaresVector[i].getPosition().y << std::endl;
    }
}
sf::Vector2f Figure::getPosition() const{
    return sf::Vector2f(m_bottomAbs,m_bottomOrd);
}

void Figure::setBottomAbs(const int &newBottomAbs){
    setNewCoord(sf::Vector2f(newBottomAbs,m_bottomOrd),m_side);
}

void Figure::setBottomOrd(const int &newBottomOrd){
    setNewCoord(sf::Vector2f(m_bottomAbs,newBottomOrd),m_side);
}

void Figure::setNewCoord(sf::Vector2f newCoord,float newSide){


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


// Classe TetrisFigure
int TetrisFigure::getChangeIndex() const{
    return m_changeIndex;
}

void TetrisFigure::decreaseChangeIndex(){
    m_changeIndex--;
    if(m_changeIndex<0){
        m_changeIndex=m_nbPositionArray[m_figNum]-1;
    }
}
void TetrisFigure::increaseChangeIndex(){
    m_changeIndex++;
    m_changeIndex = m_changeIndex%m_nbPositionArray[m_figNum];
    }

void TetrisFigure::setChangeIndex(int n){
    if(n>=0 && n<=m_nbPositionArray[m_figNum]-1){
        m_changeIndex=n;
    }
}

int TetrisFigure::getNbPosition() const{
    return m_nbPositionArray[m_figNum];
}

auto TetrisFigure::getChangeArray() const{
    return m_changeArray[m_figNum];
}

auto TetrisFigure::getChangeAbsArray() const{
    return m_changeAbsArray[m_figNum];
}

int TetrisFigure::getTopOrdIndex() const{
    return m_topOrdIndex;
}

void TetrisFigure::rotateLeft() {
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

void TetrisFigure::rotateRight() {
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


// Class Generator
static std::shared_ptr<TetrisFigure> Generator::newTetrisFigure(int side,int bottomAbsIndex,int bottomOrdIndex,int bottomAbs, int bottomOrd){
    int figNum = 1 + (std::rand() % 7);
    sf::Color color = m_tetrisColor[std::rand()%7];
    return std::make_shared<TetrisFigure>(side,color,bottomAbsIndex,bottomOrdIndex,bottomAbs,bottomOrd,figNum);
}


// Class NextScreen
void NextScreen::draw(sf::RenderWindow& window){
    sf::Text next(m_font);

    next.setString("NEXT : ");
    next.setFillColor(sf::Color::White);
    next.setPosition(sf::Vector2f(m_nextScreen.getPosition().x,m_nextScreen.getPosition().y)+m_setNext);
    window.draw(m_nextScreen);
    window.draw(next);
    m_nextFig->draw(window);
}

void NextScreen::moveFig(sf::Vector2f vec){
    m_nextFig->setNewCoord(vec,m_nextFig->getSide());
}

sf::Vector2f NextScreen::getPosition(){
    return m_nextScreen.getPosition();
}

std::shared_ptr<TetrisFigure> NextScreen::transfertFig(int newSide, int newBottomAbsIndex,int newBottomOrdIndex ,int newBottomAbs,int newBottomOrd){
    m_nextFig->setNewCoord(sf::Vector2f(newBottomAbs,newBottomOrd),newSide);
    std::shared_ptr<TetrisFigure> newsetFig = m_nextFig;
    m_nextFig.reset();
    m_nextFig = Generator::newTetrisFigure(newSide,newBottomAbsIndex,newBottomOrdIndex,m_nextScreen.getPosition().x + m_setFig.x , m_nextScreen.getPosition().y + m_setFig.y);
    return newsetFig;

}


// Class GameBoard
float GameBoard::getInitX()const{
    return m_initX;
}

float GameBoard::getInitY()const{
    return m_initY;
}

float GameBoard::getSide()const{
    return m_side;
}

int GameBoard::getScore()const{
    return m_score;
}

int GameBoard::getLevel()const{
    return m_level;
}

bool GameBoard::gameEnded() const{
    return m_gameEnded;
}

void GameBoard::autoGoDown(NextScreen& nextScreen){
    if(m_timerToGoDown.getElapsedTime()>m_timeToGoDown){
        goDown(nextScreen);
        m_timerToGoDown.restart();
    }
}

void GameBoard::goDown(NextScreen& nextScreen){
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

void GameBoard::goLeft(){
    m_activFig->goLeft();
    if(isCollision()){
        rotateOrTranslate(false);
        m_activFig->goRight();
        return;
    }
    rotateOrTranslate(true);
}


void GameBoard::goRight(){
    m_activFig->goRight();
    if(isCollision()){
        rotateOrTranslate(false);
        m_activFig-> goLeft();
        return;
    }
    rotateOrTranslate(true);
}

void GameBoard::rotateLeft(){
    isLeftRotationCollision();
}

void GameBoard::rotateRight(){
    isRightRotationCollision();
}

void GameBoard::rotateOrTranslate(bool truth){
    m_rotateOrTranslate=truth;
}

bool GameBoard::isRotateOrTranslate() const{
    return m_rotateOrTranslate;
}

void GameBoard::draw(sf::RenderWindow& window){
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

void GameBoard::updateScore(std::vector<int> vec){
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

void GameBoard::updateLevel(){
    if (m_score>= 500 + 500*(m_level+1)*m_level/2){
        m_level++;
        updateTimerToGoDown();
    }
    m_timerToGoDown.restart();
}

void GameBoard::updateTimerToGoDown(){
    m_timeToGoDown = sf::Time( m_timeForLevels[m_level-1]);
    
}
bool GameBoard::completedRow(int row){
    for(int j=0;j<10;j++){
        if(m_isSquare[row][j]==0){
            return false;
        }
    }
    return true;
}

std::vector<int> GameBoard::getCompletedRow(){
    std::vector<int> vec;
    for(int i=m_activFig->getBottomOrdIndex();i>= m_activFig->getTopOrdIndex() ; i-- ){
        if(completedRow(i)){
            vec.emplace_back(i);
        }
    }
    return vec;
}

int GameBoard::internSwap(std::vector<int> vec){
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

void GameBoard::restingSwap(std::vector<int>vec,int nonEmptyRow){
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

void GameBoard::erase(std::vector<int> vec){
    for(auto& value : vec){
        m_isSquare[value].fill(0);
        m_gameBoard[value].clear();
        m_gameBoard[value].resize(10);
    }
}


void GameBoard::collision(NextScreen& nextScreen){
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




bool GameBoard::isCollision(){
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

bool GameBoard::isLeftRotationCollision(){
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


bool GameBoard::isRightRotationCollision(){
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


// Class ScoreScreen
void ScoreScreen::setScore(unsigned int score){
    m_score = score;
    m_getScore = true;
}

void ScoreScreen::setLevel(unsigned int level){
    m_level = level;
    m_getLevel = true;
}
void ScoreScreen::setSize(const sf::Vector2f& vec){
    m_scoreScreen.setSize(vec);
}

void ScoreScreen::setPosition(const sf::Vector2f& vec){
    m_scoreScreen.setPosition(vec);
}

void ScoreScreen::moveScore(const sf::Vector2f& vec){
    m_setScore = m_setScore + vec;
}


sf::Vector2f ScoreScreen::getPosition()const{
    return m_scoreScreen.getPosition();
}

void ScoreScreen::draw(sf::RenderWindow& window){
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


// Class ScreenGame
void ScreenGame::goDown(){
    gameBoard.goDown(nextScreen);
    // goDown( (std::shared_ptr<TetrisFigure>) (*foo) (int a, int b, int c, int d, int e) = Generator::newTetrisFigure)
}

void ScreenGame::autoGoDown(){
    gameBoard.autoGoDown(nextScreen);
}

void ScreenGame::goLeft(){
    gameBoard.goLeft();
}

void ScreenGame::goRight(){
    gameBoard.goRight();
}


void ScreenGame::rotateLeft(){
    gameBoard.rotateLeft();
}

void ScreenGame::rotateRight(){
    gameBoard.rotateRight();
}

bool ScreenGame::gameEnded() const{
    return gameBoard.gameEnded();
}

void ScreenGame::rotateOrTranslate(bool truth){
    gameBoard.rotateOrTranslate(truth);
}

bool ScreenGame::isRotateOrTranslate(){
    return gameBoard.isRotateOrTranslate();
}

void ScreenGame::draw(sf::RenderWindow& window){
    scoreScreen.setScore(gameBoard.getScore()); 
    scoreScreen.setLevel(gameBoard.getLevel());
    gameBoard.draw(window);
    scoreScreen.draw(window);
    nextScreen.draw(window);

}