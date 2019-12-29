//
// Created by Quentin Liardeaux on 12/19/19.
//

#include "GameRoom.hpp"

void GameRoom::addPlayer(const boost::shared_ptr<Client>& newClient) {
    for (auto& client : m_clients) {
        client->sendPlayerJoinGame(newClient->getId(), newClient->getNickname());
        newClient->sendPlayerJoinGame(client->getId(), client->getNickname());
    }
    m_clients.push_back(newClient);
    if (m_clients.size() == MIN_PLAYERS) {
        for (auto& client : m_clients)
            client->startGame();
        m_isRunning = true;
        m_thread = std::thread(&GameRoom::run, this);
    }
}

void GameRoom::removePlayer(size_t idPlayer) {
    for (auto it = m_clients.begin() ; it != m_clients.end() ; it++) {
        if ((*it)->getId() == idPlayer) {
            m_clients.erase(it);
            break;
        }
    }
    for (auto& client : m_clients)
        client->sendPlayerQuitGame(idPlayer);
}

void GameRoom::run() {
    while (m_isRunning) {
        //std::cout << "running" << std::endl;
        for (auto& client : m_clients)
            client->update();
        for (auto& client : m_clients) {
            client->sendPlayerState(client->getId(), client->getPosition(), client->getVelocity());
            for (auto& other : m_clients) {
                if (client->getId() != other->getId())
                    client->sendPlayerState(other->getId(), other->getPosition(), other->getVelocity());
            }
        }
    }
}

void GameRoom::stop() {
    m_isRunning = false;
    m_thread.join();
}