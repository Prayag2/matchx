#include <engine/matching_engine.hpp>
#include <book/order_book.hpp>
#include <book/order_book_update.hpp>
#include <common/utils.hpp>
#include <stdexcept>
#include <plog/Log.h>
#include <chrono>
#include <string>
#include <unordered_set>

OrderBook::OrderBook(const std::string& symbol, MatchingEngine* engineRef) 
    : m_symbol{symbol}, m_engineRef{engineRef} {}

const OrderBook::AskBook& OrderBook::asks() const {
    return m_asks;
}

const OrderBook::BidBook& OrderBook::bids() const {
    return m_bids;
};

std::string OrderBook::generateTradeData(
        std::shared_ptr<Order> makerOrder,
        std::shared_ptr<Order> takerOrder,
        Price executedPrice,
        Quantity executedQuantity
) {
    std::string timeString{Utils::getTimeString(Utils::now())};
    std::string result{std::format(
        "{{\"timestamp\": \"{0:}\",\"symbol\":\"{1:}\",\"trade_id\":\"{2:}\",\"price\":\"{3:}\",\"quantity\":\"{4:}\",\"maker_order_id\": \"{5:}\",\"taker_order_id\":\"{6:}\"}}",
        timeString,
        makerOrder->symbol(),
        getNextTradeID(),
        executedPrice,
        executedQuantity,
        makerOrder->id(),
        takerOrder->id()
    )};

    return result;
}

std::string OrderBook::generateMarketData(std::shared_ptr<OrderBookUpdate> updateObj, std::unordered_set<Price> updatedAsks, std::unordered_set<Price> updatedBids) {
    using namespace std::literals;

    std::string timeString{Utils::getTimeString(updateObj->m_timestamp)};

    std::string asks{"["};
    for (Price price : updatedAsks) {
        asks += std::format("[\"{0:}\",\"{1:}\"],", price, quantityAtPriceAsks(price));
    }

    // remove extra comma
    if (asks.length() > 1)
        asks.pop_back();
    asks += "]";

    std::string bids{"["};
    for (Price price : updatedBids) {
        bids += std::format("[\"{0:}\",\"{1:}\"],", price, quantityAtPriceBids(price));
    }

    // remove extra comma
    if (bids.length() > 1)
        bids.pop_back();
    bids += "]";

    Price bestBid{-1};
    if (!m_bids.empty()) {
        bestBid = m_bids.begin()->first;
    }

    Price bestOffer{-1};
    if (!m_asks.empty()) {
        bestOffer = m_asks.begin()->first;
    }

    std::string result{std::format(
        "{{\"best_bid\":\"{0:}\",\"best_offer\":\"{1:}\",\"l2_update\":{{\"timestamp\":\"{2:}\",\"symbol\":\"{3:}\",\"asks\":{4:},\"bids\":{5:}}}}}",
        bestBid,
        bestOffer,
        timeString,
        m_symbol,
        asks,
        bids
    )};

    return result;
}

void OrderBook::update(std::shared_ptr<OrderBookUpdate> updateObj) {
    auto reduceQuantities = [&](auto& orderBookMap, auto& qtyMap, Quantity change, std::unordered_set<Price>& updatedPrices){
        while (change > 0) {
            while (!orderBookMap.empty() && orderBookMap.begin()->second.empty()) {
                orderBookMap.erase(orderBookMap.begin());
            }

            if (orderBookMap.empty()) {
                throw std::runtime_error("Update applied even though the order book is empty. This is a bug.");
            }

            /// We need to follow FIFO
            auto curPair{orderBookMap.begin()};

            auto firstOrder{curPair->second.front()}; 
            Quantity executedQuantity{std::min(firstOrder->quantity(), change)};

            firstOrder->setQuantity(firstOrder->quantity() - executedQuantity);
            qtyMap[curPair->first] -= executedQuantity;

            /// broadcast trade execution data
            m_engineRef->broadcast(MatchingEngine::TRADE_EXECUTION, generateTradeData(
                updateObj->originalOrder(),
                firstOrder,
                curPair->first,
                executedQuantity
            ));

            updatedPrices.insert(curPair->first);

            if (firstOrder->quantity() <= 0) {
                curPair->second.pop_front();
            }

            if (curPair->second.empty()) {
                orderBookMap.erase(curPair);
                qtyMap.erase(qtyMap.begin());
            }

            change -= executedQuantity;
        }
    };

    std::unordered_set<Price> updatedAsks, updatedBids;
    switch(updateObj->quantitySide()) {
        case Order::BUY:
            reduceQuantities(m_bids, m_quantityAtPriceBids, updateObj->quantity(), updatedBids);
            break;
        case Order::SELL:
            reduceQuantities(m_asks, m_quantityAtPriceAsks, updateObj->quantity(), updatedAsks);
            break;
    }

    // Adding new orders
    if (updateObj->m_newOrder) {
        Price price{updateObj->m_newOrder->price()};
        switch(updateObj->m_newOrder->side()) {
            case Order::BUY:
                m_bids[price].push_back(updateObj->m_newOrder);
                m_quantityAtPriceBids[price] += updateObj->m_newOrder->quantity();
                updatedBids.insert(price);
                break;
            case Order::SELL:
                m_asks[price].push_back(updateObj->m_newOrder);
                m_quantityAtPriceAsks[price] += updateObj->m_newOrder->quantity();
                updatedAsks.insert(price);
                break;
        }
    }

    /// Broadcasting market data
    m_engineRef->broadcast(MatchingEngine::MARKET, generateMarketData(updateObj, updatedAsks, updatedBids));
}

const std::string& OrderBook::symbol() const {
    return m_symbol;
}

Quantity OrderBook::quantityAtPriceAsks(Price price) const {
    if (m_quantityAtPriceAsks.find(price) == m_quantityAtPriceAsks.end())
        return 0;

    return m_quantityAtPriceAsks.at(price);
}

Quantity OrderBook::quantityAtPriceBids(Price price) const {
    if (m_quantityAtPriceBids.find(price) == m_quantityAtPriceBids.end())
        return 0;

    return m_quantityAtPriceBids.at(price);
}

std::string OrderBook::getNextTradeID() {
    return std::format("{0:}:{1:}", symbol(), m_tradeIDCounter++);
}
