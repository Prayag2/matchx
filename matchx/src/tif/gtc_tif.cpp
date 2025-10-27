#include <tif/gtc_tif.hpp>
#include <book/order_book.hpp>

void GoodTillCancelledTIF::execute(
    std::shared_ptr<OrderBook> orderBook,
    std::shared_ptr<OrderBookUpdate> orderBookUpdate,
    std::shared_ptr<Order> order
) const {
    orderBook->update(orderBookUpdate);
}

TimeInForce::Type GoodTillCancelledTIF::type() const {
    return TimeInForce::GoodTillCancelled;
}
