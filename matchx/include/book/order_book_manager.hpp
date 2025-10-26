#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class OrderBook;

class OrderBookManager {
private:
    std::unordered_map<std::string, std::shared_ptr<OrderBook>> m_orderBooks;

public:
    OrderBookManager();

    std::shared_ptr<OrderBook> orderBook(const std::string& symbol) const;
};
