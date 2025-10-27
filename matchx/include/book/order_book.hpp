#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <common/types.hpp>

class Order;
class OrderBookUpdate;
class MatchingEngine;

/**
 * @class OrderBook
 * @brief This data structure holds current orders of both sides, buy and sell.
 */
class OrderBook {
public:
    template<typename Compare>
    using BookType = std::map<Price, std::list<std::shared_ptr<Order>>, Compare>;
    using AskBook = BookType<std::less<Price>>;
    using BidBook = BookType<std::greater<Price>>;

private:
    std::string m_symbol{};
    AskBook m_asks{};
    BidBook m_bids{};
    std::map<Price, Quantity> m_quantityAtPriceAsks{};
    std::map<Price, Quantity> m_quantityAtPriceBids{};

    std::string m_lastL2Update{};

    ID m_tradeIDCounter{0};

    MatchingEngine* m_engineRef{nullptr};

public:
    OrderBook(const std::string& m_symbol, MatchingEngine* engineRef);

    /**
     * @brief Returns the AskBook which contains all "asks" sorted in non decreasing order
     * @return An std::map which maps Price to a list of orders selling at that price
     */
    const AskBook& asks() const;

    /**
     * @brief Returns the BidBook which contains all "asks" sorted in non decreasing order
     * @return An std::map which maps Price to a list of orders selling at that price
     */
    const BidBook& bids() const;

    /**
     * @brief Updates the order book using an update object
     * @param Shared pointer to an update object
     */
    void update(std::shared_ptr<OrderBookUpdate> updateObj);

    /**
     * @brief Returns quantity available at the specified Price level for Asks
     * @param price Price
     */
    Quantity quantityAtPriceAsks(Price price) const;

    /**
     * @brief Returns quantity available at the specified Price level for Bids
     * @param price Price
     */
    Quantity quantityAtPriceBids(Price price) const;

    /**
     * @brief Generates a JSON string that contains trade data
     * @param makerOrder The order which initiated the transaction
     * @param takerOrder The order which 
     */
    std::string generateTradeData(std::shared_ptr<Order> makerOrder, std::shared_ptr<Order> takerOrder, Price executedPrice, Quantity executedQuantity);
    std::string generateMarketData(std::shared_ptr<OrderBookUpdate> updateObj, std::unordered_set<Price> updatedAsks, std::unordered_set<Price> updatedBids);

    const std::string& symbol() const;

    std::string getNextTradeID();
};
