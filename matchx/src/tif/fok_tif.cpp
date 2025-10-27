#include <tif/fok_tif.hpp>
#include <orders/order.hpp>
#include <book/order_book_update.hpp>

void FillOrKillTIF::execute(
    std::shared_ptr<OrderBook> orderBook,
    std::shared_ptr<OrderBookUpdate> orderBookUpdate,
    std::shared_ptr<Order> order
) const {
    /// No partial updates allowed here
    if (orderBookUpdate->quantity() != order->quantity()) {
        return;
    }

    orderBookUpdate->removeOrder();
    orderBook->update(orderBookUpdate);
}

TimeInForce::Type FillOrKillTIF::type() const {
    return TimeInForce::FillOrKill;
}
