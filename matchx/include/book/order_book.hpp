#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>
#include <common/types.hpp>

class Order;
class OrderBookUpdate;

/**
 * @class OrderBook
 * @brief This data structure holds current orders of both sides, buy and sell.
 */
class OrderBook {
private:
    std::string m_symbol;
    std::map<Price, std::list<std::shared_ptr<Order>>> m_asks;
    std::map<Price, std::list<std::shared_ptr<Order>>, std::greater<Price>> m_bids;

public:
    OrderBook(const std::string& m_symbol);

    const std::list<std::shared_ptr<Order>>& bestAsks() const;
    const std::list<std::shared_ptr<Order>>& bestBids() const;

    void update(std::shared_ptr<OrderBookUpdate> updateObj);

    const std::string& symbol() const;
};
