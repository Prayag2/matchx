#include <memory>
#include <orders/order.hpp>
#include <book/order_book_update.hpp>
#include <orders/limit_order.hpp>
#include <tif/tif.hpp>

LimitOrder::LimitOrder(
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
    m_type = Order::LIMIT;
    m_side = side;
    m_timeInForce = std::move(timeInForce);
}

std::shared_ptr<OrderBookUpdate> LimitOrder::matchBuy(std::shared_ptr<OrderBook> orderBook) {
    const OrderBook::AskBook& book{orderBook->asks()};
    std::shared_ptr<OrderBookUpdate> updateObj{std::make_shared<OrderBookUpdate>(shared_from_this())};

    Quantity qty{m_quantity};
    for (auto iter = book.begin(); iter != book.end(); iter++) {
        if (qty <= 0)
            break;

        if (m_price < iter->first) {
            break;
        }

        for (auto& order : iter->second) {
            qty -= std::min(qty, order->quantity());

            if (qty <= 0)
                break;
        }
    }

    updateObj->reduceQuantity(m_quantity - qty, Order::SELL);

    if (qty > 0) {
        updateObj->addOrder(shared_from_this());
    }

    return updateObj;
}

std::shared_ptr<OrderBookUpdate> LimitOrder::matchSell(std::shared_ptr<OrderBook> orderBook) {
    const OrderBook::BidBook& book{orderBook->bids()};
    std::shared_ptr<OrderBookUpdate> updateObj{std::make_shared<OrderBookUpdate>(shared_from_this())};

    Quantity qty{m_quantity};
    for (auto iter = book.begin(); iter != book.end(); iter++) {
        if (qty <= 0)
            break;

        if (m_price > iter->first) {
            break;
        }

        for (auto& order : iter->second) {
            qty -= std::min(qty, order->quantity());

            if (qty <= 0)
                break;
        }
    }

    updateObj->reduceQuantity(m_quantity - qty, Order::BUY);

    if (qty > 0) {
        updateObj->addOrder(shared_from_this());
    }

    return updateObj;
}
