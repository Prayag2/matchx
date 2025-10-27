#include <tif/fok_tif.hpp>
#include <orders/order.hpp>
#include <orders/market_order.hpp>
#include <common/constants.hpp>

ID Order::id() const {
    return m_id;
}

Price Order::price() const {
    return m_price;
}

Quantity Order::quantity() const {
    return m_quantity;
}

Order::Type Order::type() const {
    return m_type;
}

Order::Side Order::side() const {
    return m_side;
}

const std::string& Order::symbol() const {
    return m_symbol;
}

const TimeInForce& Order::timeInForce() const {
    return *m_timeInForce;
}

void Order::setQuantity(Quantity newQuantity) {
    m_quantity = newQuantity;
}

std::shared_ptr<Order> Order::makeEmpty() {
    return std::make_shared<MarketOrder>(
        0, 0, NULL_SYMBOL, 0, BUY, std::make_unique<FillOrKillTIF>()
    );
}

std::shared_ptr<OrderBookUpdate> Order::match(std::shared_ptr<OrderBook> orderBook) {
    switch(side()) {
        case Order::BUY:
            return matchBuy(orderBook);
        case Order::SELL:
            return matchSell(orderBook);
    }
}
