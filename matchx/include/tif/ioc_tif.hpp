#pragma once

#include <tif/tif.hpp>

/**
 * @class ImmediateOrCancelTIF
 * @brief An order is filled as much as possible and the rest is cancelled
 */
class ImmediateOrCancelTIF : public TimeInForce {
public:
    void execute(
        std::shared_ptr<OrderBook> orderBook,
        std::shared_ptr<OrderBookUpdate> orderBookUpdate,
        std::shared_ptr<Order> order
    ) const override;

    TimeInForce::Type type() const override;
};
