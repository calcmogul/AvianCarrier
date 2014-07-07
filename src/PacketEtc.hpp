#ifndef PACKET_ETC_HPP
#define PACKET_ETC_HPP

#include <SFML/Network/Packet.hpp>

sf::Packet& operator <<( sf::Packet& packet , const long long int& data );
sf::Packet& operator >>( sf::Packet& packet , long long int& data );

#endif // PACKET_ETC_HPP
