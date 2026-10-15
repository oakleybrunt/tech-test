#include "ScalarResults.h"
#include <optional>
#include <stdexcept>

ScalarResults::~ScalarResults() = default;

    std::optional<ScalarResult>
    ScalarResults::operator[](const std::string &tradeId) const {
    if (!containsTrade(tradeId)) {
        return std::nullopt;
    }

    std::optional<double> priceResult = std::nullopt;
    std::optional<std::string> error = std::nullopt;

    auto resultIt = results_.find(tradeId);
    if (resultIt != results_.end()) {
        priceResult = resultIt->second;
    }

    auto errorIt = errors_.find(tradeId);
    if (errorIt != errors_.end()) {
        error = errorIt->second;
    }

    return ScalarResult(tradeId, priceResult, error);
}

bool ScalarResults::containsTrade(const std::string &tradeId) const {
    return results_.find(tradeId) != results_.end() ||
            errors_.find(tradeId) != errors_.end();
}

void ScalarResults::addResult(const std::string &tradeId, double result) {
    results_[tradeId] = result;
}

void ScalarResults::addError(const std::string &tradeId,
                             const std::string &error) {
    errors_[tradeId] = error;
}

// Both maps are sorted (not unorderd_map) by tradeId. The current trade is
// whichever iterator is on the smalled ID. If both are on the same ID, the
// trade is found in both maps (has both result and error)

// True if results_ still has items _and_ its ID is the smallest (or tied)
bool ScalarResults::Iterator::iterateResults() const {
    return rIt_ != rEnd_ && (eIt_ == eEnd_ || rIt_->first <= eIt_->first);
    }

    // True if errors_ still has items _and_ its ID is the smallest (or tied)
    bool ScalarResults::Iterator::iterateErrors() const {
    return eIt_ != eEnd_ && (rIt_ == rEnd_ || eIt_->first <= rIt_->first);
    }

    ScalarResults::Iterator &ScalarResults::Iterator::operator++() {
    const bool itR = iterateResults();
    const bool itE = iterateErrors();

    if (itR)
        ++rIt_;
    if (itE)
        ++eIt_;
    return *this;
}

ScalarResult ScalarResults::Iterator::operator*() const {
    const bool itR = iterateResults();
    const bool itE = iterateErrors();

    // If results_ is next to be iterated return that tradeId, otherwise return
    // the current tradeId from the errors_ map
    const std::string &tradeId = itR ? rIt_->first : eIt_->first;

    std::optional<double> result = std::nullopt;
    std::optional<std::string> error = std::nullopt;

    if (itR)
        result = rIt_->second;
    if (itE)
        error = eIt_->second;

    return ScalarResult(tradeId, result, error);
}

bool ScalarResults::Iterator::operator!=(const Iterator &other) const {
    return rIt_ != other.rIt_ || eIt_ != other.eIt_;
}

ScalarResults::Iterator ScalarResults::begin() const {
    return Iterator(results_.begin(), results_.end(), errors_.begin(),
                    errors_.end());
}

ScalarResults::Iterator ScalarResults::end() const {
    return Iterator(results_.end(), results_.end(), errors_.end(), errors_.end());
}
