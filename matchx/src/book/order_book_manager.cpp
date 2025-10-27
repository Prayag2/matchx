#include <engine/matching_engine.hpp>
#include <book/order_book_manager.hpp>
#include <book/order_book.hpp>
#include <common/constants.hpp>
#include <plog/Log.h>

OrderBookManager::OrderBookManager(MatchingEngine* engineRef)
    : m_engineRef{engineRef}
{
    /// @todo read symbols from a config file and load them dynamically
    m_orderBooks[NULL_SYMBOL] = std::make_shared<OrderBook>("null-null", engineRef);
    m_orderBooks["btc-usdt"] = std::make_shared<OrderBook>("btc-usdt", engineRef);
    m_orderBooks["eth-usdt"] = std::make_shared<OrderBook>("eth-usdt", engineRef);
}

std::shared_ptr<OrderBook> OrderBookManager::orderBook(const std::string& symbol) const {
    if (m_orderBooks.find(symbol) == m_orderBooks.end()) {
        PLOGW << "Invalid symbol (" << symbol << ") requested. Returning null order book.";
        return m_orderBooks.at(NULL_SYMBOL);
    }

    return m_orderBooks.at(symbol);
}
