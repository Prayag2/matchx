#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <common/types.hpp>

class OrderBook;
class MatchingEngine;

class OrderBookManager {
private:
    std::unordered_map<std::string, std::shared_ptr<OrderBook>> m_orderBooks;
    MatchingEngine* m_engineRef{nullptr};

public:
    OrderBookManager(MatchingEngine* engineRef);

    std::shared_ptr<OrderBook> orderBook(const std::string& symbol) const;
};
