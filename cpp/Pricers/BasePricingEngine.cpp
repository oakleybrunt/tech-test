#include "BasePricingEngine.h"
#include <random>
#include <thread>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <limits>

BasePricingEngine::BasePricingEngine() : delay_(5000) {}

void BasePricingEngine::price(const ITrade &trade,
                              IScalarResultReceiver *resultReceiver) {
    if (resultReceiver == nullptr) {
        throw std::invalid_argument("resultReceiver_");
    }

    priceTrade(trade, resultReceiver);
}

bool BasePricingEngine::isTradeTypeSupported(
        const std::string &tradeType) const {
    return supportedTypes_.find(tradeType) != supportedTypes_.end();
}

void BasePricingEngine::addSupportedTradeType(const std::string &tradeType) {
    supportedTypes_[tradeType] = 0;
}

int BasePricingEngine::getDelay() const { return delay_; }

void BasePricingEngine::setDelay(int delay) { delay_ = delay; }

void BasePricingEngine::priceTrade(const ITrade &trade,
                                   IScalarResultReceiver *resultReceiver) {
    if (!isTradeTypeSupported(trade.getTradeType())) {
        if (trade.getTradeId().empty()) {
        throw std::invalid_argument("Trade does not have a valid ID");
        }

        resultReceiver->addError(trade.getTradeId(), "Trade type not supported");
        return;
    }

    std::cout << ("Started pricing trade: " + trade.getTradeId() + "\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_));
    double result = calculateResult();

    std::string tradeId = trade.getTradeId();
    auto &tradesToError = getTradesToError();
    auto &tradesToWarn = getTradesToWarn();

    if (auto error = tradesToError.find(tradeId); error != tradesToError.end()) {
        resultReceiver->addError(tradeId, error->second);
    } else {
        resultReceiver->addResult(tradeId, result);
        if (auto warn = tradesToWarn.find(tradeId); warn != tradesToWarn.end()) {
        resultReceiver->addError(tradeId, warn->second);
        }
    }

    std::cout << ("Completed pricing trade: " + trade.getTradeId() + '\n');
}

double BasePricingEngine::calculateResult() {
    return random_.nextDouble() * 100.0;
}

BasePricingEngine::Random::Random()
        : gen_(rd_()), dist_(0, std::numeric_limits<unsigned int>::max()) {}

double BasePricingEngine::Random::nextDouble() {
    return static_cast<double>(dist_(gen_)) /
            static_cast<double>(std::numeric_limits<unsigned int>::max());
}

const std::map<std::string, std::string> &
BasePricingEngine::getTradesToError() {
    static const std::map<std::string, std::string> tradesToError = {
        {"GOV006", "Undefined error in pricing"}};
    return tradesToError;
}

const std::map<std::string, std::string> &BasePricingEngine::getTradesToWarn() {
    static const std::map<std::string, std::string> tradesToWarn = {
        {"FWD001", "Unable to calibrate model to value date"}};
    return tradesToWarn;
}
