#include <orders/order_parser.hpp>
#include <orders/order.hpp>
#include <orders/market_order.hpp>
#include <orders/limit_order.hpp>
#include <common/types.hpp>
#include <sstream>
#include <unordered_map>
#include <plog/Log.h>
#include <tif/gtc_tif.hpp>
#include <tif/ioc_tif.hpp>
#include <tif/fok_tif.hpp>

/// @todo improve this later
std::shared_ptr<Order> OrderParser::parse(const std::string& str) const {
    ID id{};
    Time timestamp{};
    Price price{};
    Quantity quantity{};
    std::string symbol{};
    std::string type{};
    std::string side{};
    std::string timeInForce{};

    std::unordered_map<std::string, Order::Type> getType;
    std::unordered_map<std::string, Order::Side> getSide;

    getType["MARKET"] = Order::MARKET;
    getType["LIMIT"] = Order::LIMIT;
    getSide["BUY"] = Order::BUY;
    getSide["SELL"] = Order::SELL;

    auto getTIF = [](const std::string& tif) -> std::unique_ptr<TimeInForce> {
        if (tif == "IOC")
            return std::make_unique<ImmediateOrCancelTIF>();

        if (tif == "FOK")
            return std::make_unique<FillOrKillTIF>();

        if (tif == "GTC")
            return std::make_unique<GoodTillCancelledTIF>();

        throw ParseException("Invalid TimeInForce sent");
    };

    /// format: `id timestamp symbol price quantity type side timeinforce`
    std::istringstream stream{str};
    try {
        read(stream, id);
        read(stream, timestamp);
        read(stream, symbol);
        read(stream, price);
        read(stream, quantity);
        read(stream, type);
        read(stream, side);
        read(stream, timeInForce);

        if (type == "MARKET") {
            return std::make_shared<MarketOrder>(
                id, timestamp, symbol, price, quantity, getSide.at(side), getTIF(timeInForce));
        } else if (type == "LIMIT") {
            return std::make_shared<LimitOrder>(
                id, timestamp, symbol, price, quantity, getSide.at(side), getTIF(timeInForce));
        } else {
            throw ParseException("Invalid order type");
        }
    } catch (const ParseException& e) {
        PLOGW << "Invalid parse string found. Returning empty order..";
        return Order::makeEmpty();
    }
}

template <typename T>
void OrderParser::read(std::istringstream& stream, T& variable) {
    bool result{static_cast<bool>(stream >> variable)};

    if (!result) {
        throw ParseException("Invalid parse string");
    }
}

ParseException::ParseException(const std::string& message)
    : m_message{message} {}

const char* ParseException::what() const noexcept {
    return m_message.c_str();
}
