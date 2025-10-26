#pragma once

#include <string>
#include <deque>
#include <memory>
#include <orders/order.hpp>
#include <common/types.hpp>

/**
 * @class OrderBookUpdate
 * @brief This class represents change in an order book. If a change is not required,
 *        this object can be discarded. Otherwise, it can be applied to an order book.
 *        This is also used to send L2 updates.
 */
class OrderBookUpdate {
private:
    Time m_timestamp;
    Quantity m_quantityDiffAsks;
    Quantity m_quantityDiffBids;
    std::deque<std::shared_ptr<Order>> m_newOrders;
    std::string m_symbol;

public:
    /**
     * @brief Reduces quantity of orders by `difference` 
     * @param by The amount by which quantity should be reduced (orders are removed if quantity is 0).
     *           This should be a positive integer.
     * @param side The side to do it to: asks/bids
     */
    void reduceQuantity(Quantity by, Order::Side side);

    /**
     * @brief Adds a new order
     * @param order A shared pointer to the order
     */
    void addOrder(std::shared_ptr<Order> order);

    /**
     * @brief Generates a JSON formatted string that represents an L2 update
     * @return Returns a string
     */
    std::string stringify() const;

    /// This class is a friend of OrderBook
    friend OrderBook;
};
