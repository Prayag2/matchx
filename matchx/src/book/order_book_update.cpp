#include <book/order_book_update.hpp>
#include <plog/Log.h>

void OrderBookUpdate::reduceQuantity(Quantity difference, Order::Side side) {
    if (difference < 0) {
        PLOGW << "Quantity difference is negative, however quantity can not increase. Setting change to 0.";
        difference = 0;
    }

    switch(side) {
        case Order::BUY:
            m_quantityDiffBids += difference;
            break;
        case Order::SELL:
            m_quantityDiffAsks += difference;
            break;
        default:
            PLOGW << "Invalid side chosen: " << side;
    }
}

void OrderBookUpdate::addOrder(std::shared_ptr<Order> order) {
    m_newOrders.push_back(order);
}

std::string OrderBookUpdate::stringify() const {
    return "";
}
