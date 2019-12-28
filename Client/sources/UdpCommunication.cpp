//
// Created by Quentin Liardeaux on 12/25/19.
//

#include "UdpCommunication.hpp"

boost::shared_ptr<UdpCommunication> UdpCommunication::create(std::shared_ptr<GameData> gameData) {
    return boost::shared_ptr<UdpCommunication>(new UdpCommunication(std::move(gameData)));
}

UdpCommunication::UdpCommunication(std::shared_ptr<GameData> gameData)
        : m_isRunning(false)
        , m_gameData(std::move(gameData))
        , m_responses()
        , m_ioService()
        , m_socket(m_ioService)
        , m_mutex() {
}

void UdpCommunication::start() {
    if (m_isRunning)
        return;
    m_socket.open(BoostUdp::v4());
    m_socket.bind(m_gameData->getUserData().getLocalEndpoint());
    //m_socket.connect(*m_gameData->getRemoteEndpoint());
    //m_socket.open(boost::asio::ip::udp::v4());
    m_isRunning = true;

    m_responsesThread = boost::thread(&UdpCommunication::dispatch, shared_from_this());
}

void UdpCommunication::stop() {
    m_isRunning = false;
    m_responsesThread.join();
    m_socket.close();
}

void UdpCommunication::update() {
    checkServerPackets();
}

void UdpCommunication::dispatch() {
    while (m_isRunning) {
        auto msg = receiveMessage();
        m_mutex.lock();
        //m_responses.push(std::move(msg));
        m_mutex.unlock();
    }
}

std::queue<std::unique_ptr<Message>> UdpCommunication::getServerResponses() {
    m_mutex.lock();

    auto queue = std::move(m_responses);

    m_responses = std::queue<std::unique_ptr<Message>>();
    m_mutex.unlock();
    return queue;
}

void UdpCommunication::checkServerPackets()
{
    auto responses = getServerResponses();

    while (!responses.empty()) {
        auto response = responses.front().get();

        handlePacket(*response);
        responses.pop();
    }
}

void UdpCommunication::handlePacket(const Message& msg)
{
    switch (msg.getId()) {
        case STATE_ENTITY:
            receiveEntityState(dynamic_cast<const EntityState&>(msg));
            break;
        case SYNC_DIST:
            receiveSyncDistance(dynamic_cast<const SyncDistance&>(msg));
            break;
        default:
            break;
    }
}

void UdpCommunication::receiveSyncDistance(const SyncDistance& msg) {
    std::cout << "receive sync distance" << std::endl;
}

void UdpCommunication::receiveEntityState(const EntityState &msg) {
    std::cout << "receive entity state" << msg.getEntityId() << std::endl;
    std::cout << msg.getPosition().x << " " << msg.getPosition().y << std::endl;
    std::cout << msg.getVelocity().x << " " << msg.getVelocity().y << std::endl;
    std::cout << "----------" << std::endl;
}

void UdpCommunication::playerMove(float x, float y) {
    Direction dir = RIGHT;

    if (x == 0 && y == 0)
        return;
    if (x < 0) {
        dir = LEFT;
    } else if (y < 0) {
        dir = BOTTOM;
    } else if (y > 0) {
        dir = TOP;
    }
    std::cout << "move" << std::endl;
    sendMessage(DirectionState(dir));
}

void UdpCommunication::playerShot() {
    std::cout << "shot" << std::endl;
    sendMessage(FireEntity(
            m_gameData->getUserId() * 1000 + IdProvider::instance()->getNextId(),
            FireType::NORMAL));
}

void UdpCommunication::sendMessage(const Message& msg) {
    if (auto endpoint = m_gameData->getRemoteEndpoint()) {
        uint8_t *data = new uint8_t[msg.getSize()];
        auto serializedData = msg.serialize();

        for (size_t i = 0; i < msg.getSize(); i++)
            data[i] = serializedData[i];
        m_socket.send_to(boost::asio::buffer(data, msg.getSize()), *endpoint);
        delete[] data;
    }
}

std::unique_ptr<Message> UdpCommunication::createMessage() {
    boost::asio::ip::udp::endpoint endpoint;
    packet_header_t hdr = {0, 0, 0};
    uint8_t *data = nullptr;
    std::unique_ptr<Message> msg = nullptr;
    boost::asio::socket_base::bytes_readable command(true);

    //auto receive1 = m_socket.receive_from(boost::asio::buffer(&hdr, PACKET_HDR_SIZE), tmp);
    auto receive2 = m_socket.receive_from(boost::asio::buffer(&hdr, PACKET_HDR_SIZE), endpoint);
    data = new uint8_t[hdr.packet_size];
    auto receive3 = m_socket.receive_from(boost::asio::buffer(data, hdr.packet_size), endpoint);
    std::cout << "receive " << " " << hdr.packet_size << " " << receive2 << " " << receive3 << std::endl;
    m_socket.io_control(command);
    std::size_t bytes_readable = command.get();
    std::cout << "readable " << bytes_readable << std::endl;
    for (size_t i = 0 ; i < hdr.packet_size ; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    /*switch (hdr.packet_id) {
        case STATE_ENTITY:
            msg = EntityState::from(data);
            break;
        case SYNC_DIST:
            msg = SyncDistance::from(data);
            break;
        default:
            break;
    }*/
    delete[] data;
    return msg;
}

std::unique_ptr<Message> UdpCommunication::receiveMessage() {
    return createMessage();
}