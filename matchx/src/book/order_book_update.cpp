#include <book/order_book_update.hpp>
#include <memory>
#include <plog/Log.h>

OrderBookUpdate::OrderBookUpdate(std::shared_ptr<Order> order)
    : m_originalOrder{order} {}

void OrderBookUpdate::reduceQuantity(Quantity difference, Order::Side side) {
    if (difference < 0) {
        PLOGW << "Quantity difference is negative, however quantity can not increase. Setting change to 0.";
        difference = 0;
    }

    m_quantityChange += difference;
    m_quantityChangeSide = side;
}

void OrderBookUpdate::addOrder(std::shared_ptr<Order> order) {
    m_newOrder = std::move(order);
}

void OrderBookUpdate::removeOrder() {
    m_newOrder = nullptr;
}

Quantity OrderBookUpdate::quantity() const {
    return m_quantityChange;
}

Order::Side OrderBookUpdate::quantitySide() const {
    return m_quantityChangeSide;
}

std::shared_ptr<Order> OrderBookUpdate::originalOrder() const {
    return m_originalOrder;
}
