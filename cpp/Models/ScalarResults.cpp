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

ScalarResults::Iterator ScalarResults::begin() const {
  // Start by constructing items_
  // 1. reset items_ to avoid returning stale results or missing new ones
  items_.clear();

  // 2. Populate items_ with ScalarResult objects that have a result. May not
  // have an error.
  for (const auto &result : results_) {
    std::optional<std::string> error = std::nullopt;
    // Get error associated with tradeId (result.first)
    auto e = errors_.find(result.first);
    // If it actually returns something, get the error
    if (e != errors_.end()) {
      error = e->second;
    }
    items_.push_back(ScalarResult(result.first, result.second, error));
  }

  // 3. Now add ones with just an error (no result)
  for (const auto &error : errors_) {
    if (results_.find(error.first) == results_.end()) {
      items_.push_back(ScalarResult(error.first, std::nullopt, error.second));
    }
  }

  return Iterator(items_.begin());
}

ScalarResults::Iterator ScalarResults::end() const {
  return Iterator(items_.end());
}
