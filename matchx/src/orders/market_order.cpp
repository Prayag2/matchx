#include <memory>
#include <book/order_book_update.hpp>
#include <orders/market_order.hpp>
#include <tif/tif.hpp>

MarketOrder::MarketOrder(
    ID id,
    Time timestamp,
    const std::string& symbol,
    Price price,
    Quantity quantity,
    Side side,
    std::unique_ptr<TimeInForce> timeInForce
) {
    m_id = id;
    m_timestamp = timestamp;
    m_symbol = symbol;
    m_price = price;
    m_quantity = quantity;
    m_type = Order::MARKET;
    m_side = side;
    m_timeInForce = std::move(timeInForce);
}

std::shared_ptr<OrderBookUpdate> MarketOrder::match(std::shared_ptr<OrderBook> orderBook) const {
    return std::shared_ptr<OrderBookUpdate>();
}
