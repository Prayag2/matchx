#include <book/order_book.hpp>
#include <book/order_book_update.hpp>
#include <stdexcept>

OrderBook::OrderBook(const std::string& symbol) 
    : m_symbol{symbol} {}


const std::list<std::shared_ptr<Order>>& OrderBook::bestAsks() const {
    return m_asks.begin()->second;
}

const std::list<std::shared_ptr<Order>>& OrderBook::bestBids() const {
    return m_bids.begin()->second;
};

void OrderBook::update(std::shared_ptr<OrderBookUpdate> updateObj) {
    auto reduceQuantities = [&](auto& orderBookMap, Quantity change){
        while (change > 0) {
            while (!orderBookMap.empty() && orderBookMap.begin()->second.empty()) {
                orderBookMap.erase(orderBookMap.begin());
            }

            if (orderBookMap.empty()) {
                throw std::runtime_error("Invalid update applied which is impossible. This is a bug.");
            }

            /// We need to follow FIFO
            auto firstOrder{orderBookMap.begin()->second.front()}; 
            Quantity difference{std::min(firstOrder->quantity(), change)};

            firstOrder->setQuantity(firstOrder->quantity() - difference);
            if (firstOrder->quantity() <= 0) {
                orderBookMap.begin()->second.pop_front();
            }

            change -= difference;
        }
    };

    Quantity changeInAsk{updateObj->m_quantityDiffAsks};
    reduceQuantities(m_asks, changeInAsk);

    Quantity changeInBids{updateObj->m_quantityDiffBids};
    reduceQuantities(m_bids, changeInBids);

    // Adding new orders
    for (std::shared_ptr<Order> order : updateObj->m_newOrders) {
        switch(order->side()) {
            case Order::BUY:
                m_bids[order->price()].push_back(order);
            case Order::SELL:
                m_asks[order->price()].push_back(order);
            default:
                throw std::runtime_error("Invalid side selected which should be impossible. This is a bug.");
        }
    }
}

const std::string& OrderBook::symbol() const {
    return m_symbol;
}
