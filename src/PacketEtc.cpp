#include "PacketEtc.hpp"

sf::Packet& operator <<( sf::Packet& packet , const long long int& data ) {
    long long int toWrite = htonl(data);
    append(&toWrite, sizeof(toWrite));
    return packet;
}

sf::Packet& operator >>( sf::Packet& packet , long long int& data ) {
    if (checkSize(sizeof(data))) {
        data = ntohl(*reinterpret_cast<const long long int*>(&m_data[m_readPos]));
        m_readPos += sizeof(data);
    }

    return packet;
}
