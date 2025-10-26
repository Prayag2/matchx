#pragma once

#include <memory>
#include <string>
#include <common/types.hpp>

class OrderBook;
class OrderBookUpdate;
class TimeInForce;

class Order {
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

public:
    ID id() const;
    Price price() const;
    Quantity quantity() const;
    Type type() const;
    Side side() const;
    const TimeInForce& timeInForce() const;

    const std::string& symbol() const;
    void setQuantity(Quantity newQuantity);

    virtual std::shared_ptr<OrderBookUpdate> match(std::shared_ptr<OrderBook> orderBook) const = 0;

    static std::shared_ptr<Order> makeEmpty();
};
