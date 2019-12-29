//
// Created by Quentin Liardeaux on 12/25/19.
//

#include "GameData.hpp"

GameData::GameData(UserData userData)
        : m_gameRunning(false)
        , m_entityBuilder()
        , m_userData(userData)
        , m_idGame(0)
        , m_maxPlayers(0) {}

std::shared_ptr<GameData> GameData::from(const InputOptionsHandler& inputs) {
    try {
        UserData userData(static_cast<uint16_t>(inputs.getServerPort()),
                          static_cast<uint16_t>(inputs.getClientPort()),
                          inputs.getRemoteAddress(), inputs.getLocalAddress(),
                          inputs.getNickname(), inputs.getPassword(),
                          inputs.getSessionName());

        return std::make_shared<GameData>(userData);
    } catch (std::logic_error &error) {
        return nullptr;
    }
}

void GameData::addPlayer(size_t playerId) {
    auto it = m_players.find(playerId);
    if (it == m_players.end())
        m_players[playerId] = dynamic_cast<Ship *>(m_entityBuilder.create(EntityType::SHIP));
}

void GameData::removePlayer(size_t playerId) {
    auto it = m_players.find(playerId);
    if (it != m_players.end()) {
        delete it->second;
        m_players.erase(it);
    }
}

void GameData::addEnemy(size_t enemyId) {
    auto it = m_enemies.find(enemyId);
    if (it == m_enemies.end())
        m_enemies[enemyId] = dynamic_cast<Enemy *>(m_entityBuilder.create(EntityType::ENEMY));
}

void GameData::removeEnemy(size_t enemyId) {
    auto it = m_enemies.find(enemyId);
    if (it != m_enemies.end()) {
        delete it->second;
        m_enemies.erase(it);
    }
}

void GameData::addBullet(size_t bulletId) {
    auto it = m_bullets.find(bulletId);
    if (it == m_bullets.end()) {
        m_bullets[bulletId] = dynamic_cast<Bullet *>(m_entityBuilder.create(EntityType::BULLET));
        m_bullets[bulletId]->setPosition(m_players[this->getUserId()]->getPosition());
    }
}

void GameData::removeBullet(size_t bulletId) {
    auto it = m_bullets.find(bulletId);
    if (it != m_bullets.end()) {
        delete it->second;
        m_bullets.erase(it);
    }
}

void GameData::addExplosion(const pos_t &position) {
    m_explosions.push_back(
        dynamic_cast<Explosion *>(m_entityBuilder.create(EntityType::EXPLOSION))
    );
    m_explosions.back()->setPosition(position.x, position.y);
}

void GameData::removeExplosions() {
    auto it = m_explosions.begin();
    while (it != m_explosions.end()) {
        if ((*it)->isAnimationFinished()) {
            delete *it;
            it = m_explosions.erase(it);
        } else {
            it++;
        }
    }
}

void GameData::updateRoomInfo(size_t idGame, uint8_t maxPlayers) {
    m_idGame = idGame;
    m_maxPlayers = maxPlayers;
}

void GameData::setRemoteEndpoint(uint16_t port) {
    m_userData.m_remoteEndpoint = BoostUdp::endpoint(boost::asio::ip::address::from_string(m_userData.getRemoteIpAddress()), port);
}
