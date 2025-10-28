#pragma once

#include <tif/tif.hpp>

/**
 * @class GoodTillCancelledTIF
 * @brief An order is filled as much as possible and the remaining rests in the order book.
 *        This can only be applied for Limit orders as Market orders do not rest in the order book
 */
class GoodTillCancelledTIF : public TimeInForce {
public:
    void execute(
        std::shared_ptr<OrderBook> orderBook,
        std::shared_ptr<OrderBookUpdate> orderBookUpdate,
        std::shared_ptr<Order> order
    ) const override;

    TimeInForce::Type type() const override;
};
