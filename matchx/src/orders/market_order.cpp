#include <memory>
#include <book/order_book.hpp>
#include <book/order_book_update.hpp>
#include <orders/market_order.hpp>
#include <tif/tif.hpp>
#include <tif/ioc_tif.hpp>
#include <plog/Log.h>

MarketOrder::MarketOrder(
    ID id,
    Time timestamp,
    const std::string& symbol,
    Quantity quantity,
    Side side,
    std::unique_ptr<TimeInForce> timeInForce
) {
    m_id = id;
    m_timestamp = timestamp;
    m_symbol = symbol;
    m_quantity = quantity;
    m_side = side;
    m_type = Order::MARKET;
    m_price = 0;

    /// Market orders do not support GTC as they can not rest in the order book
    if (timeInForce->type() == TimeInForce::GoodTillCancelled) {
        PLOGW << "GoodTillCancelled chosen for a MarketOrder which should be impossible. Using ImmediateOrCancel instead.";
        m_timeInForce = std::make_unique<ImmediateOrCancelTIF>();
    } else {
        m_timeInForce = std::move(timeInForce);
    }
}

std::shared_ptr<OrderBookUpdate> MarketOrder::matchBuy(std::shared_ptr<OrderBook> orderBook) {
    const OrderBook::AskBook& book{orderBook->asks()};
    std::shared_ptr<OrderBookUpdate> updateObj{std::make_shared<OrderBookUpdate>(shared_from_this())};

    Quantity qty{m_quantity};
    for (auto iter = book.begin(); iter != book.end(); iter++) {
        if (qty <= 0)
            break;

        for (auto& order : iter->second) {
            qty -= std::min(qty, order->quantity());

            if (qty <= 0)
                break;
        }
    }

    updateObj->reduceQuantity(m_quantity - qty, Order::SELL);

    return updateObj;
}

std::shared_ptr<OrderBookUpdate> MarketOrder::matchSell(std::shared_ptr<OrderBook> orderBook) {
    const OrderBook::BidBook& book{orderBook->bids()};
    std::shared_ptr<OrderBookUpdate> updateObj{std::make_shared<OrderBookUpdate>(shared_from_this())};

    Quantity qty{m_quantity};
    for (auto iter = book.begin(); iter != book.end(); iter++) {
        if (qty <= 0)
            break;

        for (auto& order : iter->second) {
            qty -= std::min(qty, order->quantity());

            if (qty <= 0)
                break;
        }
    }

    updateObj->reduceQuantity(m_quantity - qty, Order::BUY);

    return updateObj;
}
