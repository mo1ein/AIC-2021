#ifndef AIC21_CLIENT_CPP_CONTROLLER_H
#define AIC21_CLIENT_CPP_CONTROLLER_H

using namespace std;

#include <string>
#include <thread>
#include <memory>

#include "Network/Network.h"
#include "Models/Game.h"
#include "AI/AI.h"
#include "Core/EventQueue.h"

using namespace std;

/**
 * High level client logic is implemented here
 */
class Controller final {
public:

    /**
     * @param host The server host to connect to
     * @param port The server port
     * @param token Authentication token to send to server
     * @param retry_delay Number of milliseconds to wait before retrying connection
     */
    Controller(const string& host, uint16_t port, const string& token, unsigned retry_delay);

    /**
     * Destructor
     */
    ~Controller();

    Controller(const Controller&) = delete;
    Controller& operator= (const Controller&) = delete;

    /**
     * Start the game
     */
    void run();

private:

    /**
     * The loop to dequeue events and send them across the network
     */
    void event_handling_loop() noexcept;

    /// Authentication token to send to server
    string m_token;

    /// Number of milliseconds to wait before retrying connection
    unsigned m_retry_delay;

    /// The network connection used for talking to server
    Network m_network;

    /// The event queue holding events to send
    EventQueue m_event_queue;

    /// Thread object that will run @see event_handling_loop
    thread m_event_handling_thread;

    /// Vector of threads that are made by receiving every pick or turn message
    vector<thread*> m_thread_list;

    /// Instance of the game world
    Game m_game;

    /// Instance of the client AI
    AI m_client;

    /// Functions which one of them will be called in a new thread
    static void turn_event(AI* client, Game* tmp_game, EventQueue* m_event_queue);
    static void end_event(AI* client, Game* tmp_game, EventQueue* m_event_queue);

public:
    static int thread_count;

};

#endif // AIC21_CLIENT_CPP_CONTROLLER_H
