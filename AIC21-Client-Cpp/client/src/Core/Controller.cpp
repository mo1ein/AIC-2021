#include <iostream>
#include <thread>
#include <chrono>

#include "Controller.h"
#include "Utility/Utility.h"
#include "Utility/Logger.h"

#include "Network/NetworkError.h"
#include "Core/Message/Parse/ParseError.h"
#include "Core/Message/Parse/GameConfigMessage.h"
#include "Core/Message/Parse/CurrentStateMessage.h"
#include "Core/Message/Parse/ShutdownMessage.h"

#include "Core/Message/Create/AuthenticationMessage.h"
#include "Core/Message/Create/CreateChatBoxMessage.h"
#include "Core/Message/Create/CreateMovementMessage.h"
#include "Core/Message/Create/CreateEndTurnMessage.h"

int Controller::thread_count = 0;

int getDirectionNum(Direction direction);

Controller::Controller(const string &host, uint16_t port, const string &token, unsigned retry_delay)
        : m_token(token)
        , m_retry_delay(retry_delay)
        , m_network(host, port)
        , m_game(m_event_queue) {
    Logger::Get(LogLevel_DEBUG) << "Server is " << host << ":" << port << endl;
    Logger::Get(LogLevel_DEBUG) << "Authentication token is " << token << endl;
    Logger::Get(LogLevel_DEBUG) << "Retry delay is " << retry_delay << endl;
}

Controller::~Controller() {
    if (m_event_handling_thread.joinable())
        m_event_handling_thread.join();
    if (m_network.is_connected())
        m_network.disconnect();
}

void Controller::run() try {
    Logger::Get(LogLevel_TRACE) << "Enter Controller::run" << endl;

    // Connect to the server

    constexpr size_t MAX_RETRY_COUNT = 20;
    for (size_t i = 1; i <= MAX_RETRY_COUNT && !m_network.is_connected(); ++i)
        try {
            Logger::Get(LogLevel_INFO) << "Trying to connect #" << i << endl;
            m_network.connect();
        }
        catch (NetworkError &e) {
            if (i == MAX_RETRY_COUNT)
                throw;
            else
                this_thread::sleep_for(chrono::milliseconds(m_retry_delay));
        }

    Logger::Get(LogLevel_INFO) << "Connected" << endl;
    Logger::Get(LogLevel_TRACE) << "Sending authentication message" << endl;
    m_network.send(AuthenticationMessage(m_token).toString()); // about the authentication process

    // Now wait for init message
    Logger::Get(LogLevel_TRACE) << "Waiting for init message" << endl;

    // Start the event handling thread
    m_event_handling_thread = std::thread(&Controller::event_handling_loop, this);

    while (m_network.is_connected()) {
        Logger::Get(LogLevel_TRACE) << "Waiting for 3,4,7 message" << endl;

        auto message = Message::CreateFromJsonString(m_network.receive());

        if (GameConfigMessage *init_message = dynamic_cast<GameConfigMessage *>(message.get())) {
            Logger::Get(LogLevel_TRACE) << "Parsing init message" << endl;
            m_game.initGameConfig(init_message);
        }
        else if (CurrentStateMessage *turn_message = dynamic_cast<CurrentStateMessage *>(message.get())) {
            Logger::Get(LogLevel_TRACE) << "Parsing turn message" << endl;
            Game *_game = new Game(m_game);  //copying from the initial game
            Logger::Get(LogLevel_TRACE) << "Created new game from last game" << endl;
            _game->setCurrentState(turn_message); //updating the new game

            Logger::Get(LogLevel_INFO) << "Received Action message from server" << endl;

            thread *actionThread = new thread(Controller::turn_event, &m_client, _game, &(this->m_event_queue));
            m_thread_list.push_back(actionThread);
        }
        else if (ShutdownMessage *shutdown_message = dynamic_cast<ShutdownMessage *>(message.get())) {
            Logger::Get(LogLevel_INFO) << "Received shutdown message from server" << endl;
            // TODO : Decide about shutdown
            break;
        }
    }

    Logger::Get(LogLevel_INFO) << "Closing the connection" << endl;
    m_event_queue.terminate();
    m_network.disconnect();

    Logger::Get(LogLevel_INFO) << "Joining all threads" << endl;
    for (thread *_thread : m_thread_list) {
        _thread->join();
        delete _thread;
    }

    Logger::Get(LogLevel_TRACE) << "Exit Controller::run" << endl;
}
catch (json::exception&) {
    throw ParseError("Malformed json string");
}

void Controller::event_handling_loop() noexcept {
    while (m_network.is_connected()) {
        auto message = m_event_queue.pop();
        if (!message)
            break;
        m_network.send(message->toString());
    }
}

void Controller::turn_event(AI *client, Game *tmp_game, EventQueue *m_event_queue) {
    int THREAD_NUM = Controller::thread_count++;
    try {
        Logger::Get(LogLevel_DEBUG) << "Launched action Thread #" << THREAD_NUM << endl;
        Answer* answer = client->turn(tmp_game);
        Logger::Get(LogLevel_TRACE) << "Recieved client answer" << endl;

        m_event_queue->push(CreateMovementMessage(getDirectionNum(answer->getDirection())));
        if (!answer->getMessage().empty())
            m_event_queue->push(CreateChatBoxMessage(answer->getMessage(), answer->getMessageValue()));
        delete answer;
    }
    catch (const char *err_msg) {
        Logger::Get(LogLevel_ERROR) << "Error in action Thread #" << THREAD_NUM << endl
                                    << err_msg << endl;
    }

    Logger::Get(LogLevel_TRACE) << "action:Sending end message" << endl;
    m_event_queue->push(CreateEndTurnMessage());

    delete tmp_game;
    Logger::Get(LogLevel_DEBUG) << "End of action Thread #" << THREAD_NUM << endl;
}

int getDirectionNum(Direction direction) {
    switch (direction) {
        case UP:
            return 2;
        case DOWN:
            return 4;
        case LEFT:
            return 3;
        case RIGHT:
            return 1;
        case CENTER:
            return 0;
        default:
            return -1;
    }
}
