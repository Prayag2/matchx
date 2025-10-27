#pragma once

#include <tif/tif.hpp>

class GoodTillCancelledTIF : public TimeInForce {
public:
    void execute(
        std::shared_ptr<OrderBook> orderBook,
        std::shared_ptr<OrderBookUpdate> orderBookUpdate,
        std::shared_ptr<Order> order
    ) const override;

    TimeInForce::Type type() const override;
};
