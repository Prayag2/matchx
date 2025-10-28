#pragma once

#include <functional>
#include <memory>
#include <deque>
#include <string>
#include <map>
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
    using BookType = std::map<Price, std::deque<std::shared_ptr<Order>>, Compare>;
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
     * @return Quantity
     */
    Quantity quantityAtPriceAsks(Price price) const;

    /**
     * @brief Returns quantity available at the specified Price level for Bids
     * @param price Price
     * @return Quantity
     */
    Quantity quantityAtPriceBids(Price price) const;

    /**
     * @brief Generates a JSON string that contains trade data
     * @param makerOrder ID of the resting order
     * @param takerOrder ID of the order which initiated the trade
     * @return std::string in JSON format
     */
    std::string generateTradeData(std::shared_ptr<Order> makerOrder, std::shared_ptr<Order> takerOrder, Price executedPrice, Quantity executedQuantity);

    /**
     * @brief Generates a JSON string that contains market data and L2 order book updates
     * @param updateObj the update object which made changes to the order book
     * @param updatedAsks set of prices which whos quantity were changed for asks
     * @param updatedBids set of prices which whos quantity were changed for bids
     * @return std::string in JSON format
     */
    std::string generateMarketData(std::shared_ptr<OrderBookUpdate> updateObj, std::unordered_set<Price> updatedAsks, std::unordered_set<Price> updatedBids);

    /**
     * @brief Returns the symbol of this order book
     * @return std::string
     */
    const std::string& symbol() const;

    /**
     * @brief Gets the next trade ID and updates the internal counter
     * @return std::string in this format: "<symbol>:<integer id>"
     */
    std::string getNextTradeID();
};
