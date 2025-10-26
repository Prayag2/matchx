#pragma once

#include <memory>
class OrderBook;
class OrderBookUpdate;
class Order;

class TimeInForce {
public:
    virtual void execute(
        std::shared_ptr<OrderBook> orderBook,
        std::shared_ptr<OrderBookUpdate> orderBookUpdate,
        std::shared_ptr<Order> order
    ) const = 0;
};
