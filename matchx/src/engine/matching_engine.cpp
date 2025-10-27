#include <cstdlib>
#include <memory>
#include <iostream>
#include <book/order_book.hpp>
#include <orders/order.hpp>
#include <orders/order_parser.hpp>
#include <engine/matching_engine.hpp>
#include <common/constants.hpp>
#include <tif/tif.hpp>
#include <plog/Log.h>

MatchingEngine::MatchingEngine() {
    m_orderParser = std::make_unique<OrderParser>();
    m_orderBookManager = std::make_unique<OrderBookManager>(this);

    const char* host{std::getenv(REDIS_HOST)};
    const char* port{std::getenv(REDIS_PORT)};

    std::string uri{
        "tcp://"
        + std::string(host ? host : FALLBACK_REDIS_HOST)
        + ":"
        + std::string(port ? port : FALLBACK_REDIS_PORT)
    };

    PLOGI << "Connecting to Redis...";
    m_redis = std::make_unique<sw::redis::Redis>(uri);
}

void MatchingEngine::benchmark() {
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<uint64_t> idDist(1, 1'000'000);
    std::uniform_int_distribution<int64_t> priceDist(10'000, 100'000'000);
    std::uniform_int_distribution<int64_t> qtyDist(1, 10'000);
    std::uniform_int_distribution<int> symDist(0, 1);
    std::uniform_int_distribution<int> typeDist(0, 1);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int> tifDistLimit(0, 2); // GTC, FOK, IOC
    std::uniform_int_distribution<int> tifDistMarket(1, 2); // only FOK, IOC

    const std::string symbols[] = {"btc-usdt", "eth-usdt"};
    const std::string types[] = {"MARKET", "LIMIT"};
    const std::string sides[] = {"BUY", "SELL"};
    const std::string tifs[] = {"GTC", "FOK", "IOC"};

    const size_t totalOrders = 1'000'000;
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < totalOrders; ++i) {
        uint64_t id = idDist(rng);
        uint64_t epoch = std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::system_clock::now().time_since_epoch())
                             .count();
        std::string symbol = symbols[symDist(rng)];
        std::string orderType = types[typeDist(rng)];
        std::string side = sides[sideDist(rng)];
        std::string tif = orderType == "MARKET" ? tifs[tifDistMarket(rng)] : tifs[tifDistLimit(rng)];

        int64_t price = priceDist(rng);
        int64_t quantity = qtyDist(rng);

        std::ostringstream oss;
        oss << id << ' ' << epoch << ' ' << symbol << ' ' << price << ' ' << quantity << ' '
            << orderType << ' ' << side << ' ' << tif;

        std::string orderString = oss.str();

        std::shared_ptr<Order> order = m_orderParser->parse(orderString);
        std::shared_ptr<OrderBook> orderBook = m_orderBookManager->orderBook(order->symbol());

        std::shared_ptr<OrderBookUpdate> updateObj = order->match(orderBook);
        order->timeInForce().execute(orderBook, updateObj, order);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double durationSec = std::chrono::duration<double>(end - start).count();
    double ordersPerSec = totalOrders / durationSec;

    std::cout << "Processed " << totalOrders << " orders in " << durationSec
              << " seconds (" << ordersPerSec << " orders/sec)" << std::endl;
}

void MatchingEngine::run() {
    PLOGI << "Listening to Redis Queue: " << REDIS_QUEUE;
    while (true) {
        auto poppedItem{m_redis->brpop(REDIS_QUEUE, 0)};
        if (poppedItem.has_value()) {
            std::string orderString{poppedItem.value().second};

            std::shared_ptr<Order> order{m_orderParser->parse(orderString)};
            std::shared_ptr<OrderBook> orderBook{m_orderBookManager->orderBook(order->symbol())};

            std::shared_ptr<OrderBookUpdate> updateObj{order->match(orderBook)};
            order->timeInForce().execute(orderBook, updateObj, order);
        }
    }
}

inline constexpr std::string MatchingEngine::toString(MatchingEngine::Channel channel) {
    switch(channel) {
        case MatchingEngine::MARKET:
            return "channel:market";
        case MatchingEngine::TRADE_EXECUTION:
            return "channel:trade_execution";
    }
}

void MatchingEngine::broadcast(MatchingEngine::Channel channel, const std::string& message) const {
    m_redis->publish(toString(channel), message);
}
