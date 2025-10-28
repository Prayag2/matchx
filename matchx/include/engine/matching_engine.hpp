#pragma once

#include <sw/redis++/redis++.h>
#include <book/order_book_manager.hpp>
#include <orders/order_parser.hpp>

/**
 * @class MatchingEngine
 * @brief The main matching engine class which runs the program
 */
class MatchingEngine {
private:
    std::unique_ptr<sw::redis::Redis> m_redis;
    std::unique_ptr<OrderBookManager> m_orderBookManager;
    std::unique_ptr<OrderParser> m_orderParser;

public:
    MatchingEngine();

    /**
     * @brief Starting point of the program, called from `main`.
     */
    void run();

    /**
     * @brief Benchmarks 1,000,000 orders and prints the number of orders processed in one second
     */
    void benchmark();

    /**
     * @enum Channel
     * @brief An enum which lists all Redis channels to send messages to
     */
    enum Channel {
        MARKET,
        TRADE_EXECUTION
    };

    /**
     * @brief Get string name of a channel
     * @param channel MatchingEngine::Channel
     * @return std::string
     */
    inline static constexpr std::string toString(Channel channel);

    /**
     * @brief Publishes this message to the specified channel
     * @param channel MatchingEngine::Channel type
     * @param message A string which contains the message to be sent
     */
    void broadcast(Channel channel, const std::string& message) const;
};
