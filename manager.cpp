#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include "usefull.hpp"
#include "game.hpp"
#include "welcome.hpp"
#include "pause.hpp"
#include "settings.hpp"


/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/*
                    CAHIER DES CHARGES DU MANAGER

    1) Savoir quel screen est au 1er plan et est à gérer
    
    2) Faire les transitions entre les différents screen

    3) Les touches notammment pour gameCcp passent par lui (les touches proviennent de settings)

    4) Doit pvr gérer solo et OneVOne

    5) C'est lui qui gère l'audio le volume est donné par settings (btw peut être changer la classe settings pour qu'elle possède un arg "taux de volume")

    6) Accepte une taille de fenêtre et la partage à tous les autres screen

*/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/


class Manager: public sf::Drawable, public sf::Transformable{
public:
ScreenGame screenGameSolo;
SettingsScreen settingsScreen;
WelcomeScreen welcomeScreen;
PauseScreen pauseScreen;
ScreenGame screenGameOneVOneJ1;
ScreenGame screenGameOneVOneJ2;
sf::Audio audio;




Manager()

}