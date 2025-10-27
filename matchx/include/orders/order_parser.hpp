#pragma once

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
class Order;

/**
 * @class OrderParser
 * @brief Parses strings representing an order and constructors Order objects
 */
class OrderParser {
public:
    /**
     * @brief Parses an order string and constructs an Order objectOrder side "BUY" or "SELL"
     * @param str The input string representing an order, with fields separated by ` ` (space), in the following order:
     * - `id` : ID of the order written as an unsigned binary number
     * - `timestamp` : Unix timestamp in microseconds written as uint64_t
     * - `symbol` : Trading symbol written in lowercase English letters separated by `-`. E.g. `btc-usdt`
     * - `price` : Order price multiplied by 100, written as int64_t
     * - `quantity` : Quantity of the order multiplied by 100, written as int64_t
     * - `type` : Order type "MARKET" or "LIMIT"
     * - `side` : Order side "BUY" or "SELL"
     * - `timeinforce` : Time in force "GTC", "IOC" or "FOK"
     * @return If the string is invalid, an empty Order object will be returned, else the Order object is returned
     */
    std::shared_ptr<Order> parse(const std::string& str) const;

private:
    /**
     * @brief Reads a value from the stream into a variable
     * @throws ParseException if there is some error
     * @param stream A string stream
     * @param variable The variable to store the value into
     */
    template <typename T>
    static void read(std::istringstream& stream, T& variable);
};

class ParseException : public std::exception {
private:
    std::string m_message;

public:
    explicit ParseException(const std::string& message);

    const char* what() const noexcept override;
};
