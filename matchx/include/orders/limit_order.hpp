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

    std::shared_ptr<OrderBookUpdate> match(std::shared_ptr<OrderBook> orderBook) const override;
};
