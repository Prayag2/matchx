#pragma once

#include <orders/order.hpp>

class LimitOrder : public Order {
public:
    LimitOrder(
        ID id,
        Time timestamp,
        const std::string& symbol,
        Price price,
        Quantity quantity,
        Side side,
        std::unique_ptr<TimeInForce> timeInForce
    );

protected:
    std::shared_ptr<OrderBookUpdate> matchBuy(std::shared_ptr<OrderBook> orderBook) override;
    std::shared_ptr<OrderBookUpdate> matchSell(std::shared_ptr<OrderBook> orderBook) override;
};
