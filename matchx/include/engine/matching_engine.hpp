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
};
