#include <cstdlib>
#include <memory>
#include <book/order_book.hpp>
#include <orders/order.hpp>
#include <orders/order_parser.hpp>
#include <engine/matching_engine.hpp>
#include <common/constants.hpp>
#include <tif/tif.hpp>
#include <plog/Log.h>

MatchingEngine::MatchingEngine() {
    m_orderParser = std::make_unique<OrderParser>();
    m_orderBookManager = std::make_unique<OrderBookManager>();

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
