#pragma once

#include <tif/tif.hpp>

/**
 * @class FillOrKillTIF
 * @brief An order is filled if and only if it can be filled in its entirety. If some part of it can not be filled,
 *        the entire order is cancelled
 */
class FillOrKillTIF : public TimeInForce {
public:
    void execute(
        std::shared_ptr<OrderBook> orderBook,
        std::shared_ptr<OrderBookUpdate> orderBookUpdate,
        std::shared_ptr<Order> order
    ) const override;

    TimeInForce::Type type() const override;
};
