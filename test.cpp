#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Network.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <cstdint>
#include <iostream>
#include <cmath>
#include <memory>
#include <optional>
#include <vector>
#include <algorithm>
#include <cstring>
int main(){

    // std::optional<sf::IpAddress> myLocalAddress = sf::IpAddress::getLocalAddress();
    // std::optional <sf::IpAddress> myPublicAddress = sf::IpAddress::getPublicAddress();

    // std::cout << "Adresse locale : " << myLocalAddress->toString() << std::endl;
    // std::cout << "Adresse publique : " << myPublicAddress->toString() << std::endl;
    // std::cout << "Adresse locale : " << myLocalAddress->toInteger() << std::endl;
    // std::cout << "Adresse publique : " << myPublicAddress->toInteger() << std::endl;
     
    // sf::TcpSocket socket;

    // // const std::string publique = "172.18.162.252";
    // // sf::IpAddress ipLocal(local);
    // // sf::IpAddress ipPublic(publique);

    // if(socket.connect({46,193,69,142},53000,sf::seconds(10)) != sf::Socket::Status::Done){
    //     if(socket.connect({172,18,162,252},53000,sf::seconds(10)) != sf::Socket::Status::Done);
    //     std::cout << "Erreur:impossible de se connecteur au serveur " << std::endl;
    //     return -1;
    // }

    // std::cout << "Connecté au serv !" << std::endl;

    // char buffer[128];
    // std::size_t reveived;
    // if(socket.receive(buffer,sizeof(buffer),reveived) == sf::Socket::Status::Done){
    //     std::cout << "Message du serv : " << buffer << std::endl;
    // }


    std::optional<sf::IpAddress> myLocalAdress = sf::IpAddress::getLocalAddress();
    std::optional<sf::IpAddress> myPublicAdress = sf::IpAddress::getPublicAddress();

    std::cout << "Local Adress : " << myLocalAdress->toString() << std::endl;
    std::cout << "Public Adress : " << myPublicAdress->toString() << std::endl;
    
    sf::TcpListener listener;
    if(listener.listen(10000) != sf::Socket::Status::Done){
        std::cout << "Erreur : impossible d'écouter sur le port 10000" << std::endl;
        return -1;
    }

    std::vector<sf::TcpSocket> clients;
    sf::SocketSelector selector;
    selector.add(listener);
    while(1){
        if(selector.wait()){
            if(selector.isReady(listener)){
                sf::TcpSocket client;
                if(listener.accept(client) == sf::Socket::Status::Done){
                    std::cout << "nouveau client" << std::endl;
                    selector.add(client);
                    clients.push_back(std::move(client));
                }
                else{

                }
            }
            else{
                for(sf::TcpSocket& client : clients){
                    if(selector.isReady(client)){
                        sf::Packet packet;
                        if(client.receive(packet)==sf::Socket::Status::Done){

                        }
                    }
                }
            }
        }
    }

    sf::Packet packet;
    std::size_t received;

    sf::UdpSocket socket;
    if(socket.bind(54000) != sf::Socket::Status::Done){

    }
    
    std::optional<sf::IpAddress> recipient({172,18,162,152});
    unsigned short port = 53000;
    char buf[100];
    while(1){
         if(socket.receive(packet,recipient,port) != sf::Socket::Status::Done){

         }
         packet >> buf;
         std::cout << buf << std::endl;

         if(strcmp(buf,"exit")){
             return 0;
         }
    }

    


    return 0;
    
}