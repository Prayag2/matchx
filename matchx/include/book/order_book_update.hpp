#pragma once

#include <string>
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
    Quantity m_quantityChange;
    Order::Side m_quantityChangeSide;
    std::string m_symbol;

    std::shared_ptr<Order> m_newOrder{nullptr};
    std::shared_ptr<Order> m_originalOrder{nullptr};

public:
    OrderBookUpdate() = delete;

    OrderBookUpdate(std::shared_ptr<Order> originalOrder);

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
     * @brief Remove order
     */
    void removeOrder();

    /**
     * @brief Get original order
     * @return Shared pointer to the order
     */
    std::shared_ptr<Order> originalOrder() const;

    /**
     * @brief Get change in quantity
     * @returns A Quantity value
     */
    Quantity quantity() const;

    /**
     * @brief Returns the side where quantity will be changed
     * @return Order::Side enum
     */
    Order::Side quantitySide() const;

    /// This class is a friend of OrderBook
    friend OrderBook;
};
