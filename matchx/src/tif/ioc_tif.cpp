#include <tif/ioc_tif.hpp>
#include <book/order_book.hpp>
#include <book/order_book_update.hpp>

void ImmediateOrCancelTIF::execute(
    std::shared_ptr<OrderBook> orderBook,
    std::shared_ptr<OrderBookUpdate> orderBookUpdate,
    std::shared_ptr<Order> order
) const {
    orderBookUpdate->removeOrder();
    orderBook->update(orderBookUpdate);
}

TimeInForce::Type ImmediateOrCancelTIF::type() const {
    return TimeInForce::ImmediateOrCancel;
}
