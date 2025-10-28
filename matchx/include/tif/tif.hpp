#pragma once

#include <memory>
class OrderBook;
class OrderBookUpdate;
class Order;

/**
 * @class TimeInForce
 * @brief This is an instruction that is applied on top of orders which describes how to execute orders and
 *        what to do with them after execution.
 */
class TimeInForce {
public:
    /**
     * @brief This is a virtual function that executes the instruction
     * @param orderBook A shared pointer to the orderBook
     * @param orderBookUpdate The update object which let to this execution
     * @param order The order which initiated it
     */
    virtual void execute(
        std::shared_ptr<OrderBook> orderBook,
        std::shared_ptr<OrderBookUpdate> orderBookUpdate,
        std::shared_ptr<Order> order
    ) const = 0;

    /**
     * @enum Type
     * @brief There are three types of TimeInForce instructions here
     *        1. GoodTillCancelled will fill as much of the order as possible and the remaining order will rest in the book
     *        2. ImmediateOrCancel will fill as much of the order as possible and cancel the remaining
     *        3. FillOrKill will only execute the order if the entire order can be filled else cancel the entire order
     */
    enum Type { GoodTillCancelled, ImmediateOrCancel, FillOrKill };

    /**
     * @brief Returns the type of TimeInForce instruction
     * @return TimeInForce::Type enum
     */
    virtual Type type() const = 0;
};
