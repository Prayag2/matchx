#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <common/types.hpp>

class OrderBook;
class MatchingEngine;

/**
 * @class OrderBookManager
 * @brief This class manages different kinds of order books
 */
class OrderBookManager {
private:
    std::unordered_map<std::string, std::shared_ptr<OrderBook>> m_orderBooks;
    MatchingEngine* m_engineRef{nullptr};

public:
    /**
     * @param engineRef is a pointer to the MatchingEngine
     */
    OrderBookManager(MatchingEngine* engineRef);

    /**
     * @brief Returns a pointer to the instance of an order book for the given symbol
     * @param symbol The string symbol
     * @return An std::shared_ptr to the corresponding OrderBook
     */
    std::shared_ptr<OrderBook> orderBook(const std::string& symbol) const;
};
