#pragma once

#include <memory>
#include <string>
#include <common/types.hpp>
#include <book/order_book.hpp>

class OrderBookUpdate;
class TimeInForce;

/**
 * @class Order
 * @brief This class describes an Order
 */
class Order : public std::enable_shared_from_this<Order> {
public:
    enum Type { MARKET, LIMIT };
    enum Side { BUY, SELL };

protected:
    std::string m_symbol;
    Time m_timestamp;
    ID m_id;
    Price m_price;
    Quantity m_quantity;
    std::unique_ptr<TimeInForce> m_timeInForce;
    Type m_type;
    Side m_side;

    /**
     * @brief Used to match BUY orders in the order book
     * @param orderBook The associated orderBook
     * @return An OrderBookUpdate object which can be used to commit changes to the order book
     */
    virtual std::shared_ptr<OrderBookUpdate> matchBuy(std::shared_ptr<OrderBook> orderBook) = 0;

    /**
     * @brief Used to match SELL orders in the order book
     * @param orderBook The associated orderBook
     * @return An OrderBookUpdate object which can be used to commit changes to the order book
     */
    virtual std::shared_ptr<OrderBookUpdate> matchSell(std::shared_ptr<OrderBook> orderBook) = 0;

public:
    /**
     * @brief Get order ID
     * @return ID
     */
    ID id() const;

    /**
     * @brief Get price mentioned
     * @return Price
     */
    Price price() const;

    /**
     * @brief Get quantity ordered
     * @return Quantity
     */
    Quantity quantity() const;

    /**
     * @brief Get the type of order: Market or Limit
     * @return Order::Type enum
     */
    Type type() const;

    /**
     * @brief Get order side: BUY or SELL
     * @return Order::Side enum
     */
    Side side() const;
    
    /**
     * @brief Get a reference to its timeinforce object
     * @return Reference to TimeInForce object
     */
    const TimeInForce& timeInForce() const;

    /**
     * @brief Get order symbol
     * @return std::string symbol
     */
    const std::string& symbol() const;

    /**
     * @brief Update the quantity of this order
     * @param newQuantity The new quantity
     */
    void setQuantity(Quantity newQuantity);

    /**
     * @brief Match orders. Calls one of matchBuy or matchSell depending on the order side
     * @param orderBook A shared pointer to the associated orderBook
     * @return Returns an OrderBookUpdate object
     */
    std::shared_ptr<OrderBookUpdate> match(std::shared_ptr<OrderBook> orderBook);

    /**
     * @brief Returns an empty Order
     * @return A shared pointer to the created order
     */
    static std::shared_ptr<Order> makeEmpty();
};
