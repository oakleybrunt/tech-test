#ifndef SCALARRESULTS_H
#define SCALARRESULTS_H

#include "IScalarResultReceiver.h"
#include "ScalarResult.h"
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <vector>

class ScalarResults : public IScalarResultReceiver {
public:
  virtual ~ScalarResults();
  std::optional<ScalarResult> operator[](const std::string &tradeId) const;

  bool containsTrade(const std::string &tradeId) const;

  virtual void addResult(const std::string &tradeId, double result) override;

  virtual void addError(const std::string &tradeId,
                        const std::string &error) override;

  class Iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = ScalarResult;
    using difference_type = std::ptrdiff_t;
    using pointer = ScalarResult *;
    using reference = ScalarResult &;

    using Inner = std::vector<ScalarResult>::const_iterator;

    Iterator() = default;
    explicit Iterator(Inner it) : it_(it) {};

    // Iterator must be constructable from ScalarResults parent
    // Move to next item in list
    Iterator &operator++() {
      ++it_;
      return *this;
    };
    // Return current item
    ScalarResult operator*() const { return *it_; };

    // Check equality
    bool operator!=(const Iterator &other) const { return it_ != other.it_; };

  private:
    Inner it_;
  };

  Iterator begin() const;
  Iterator end() const;

private:
  std::map<std::string, double> results_;
  std::map<std::string, std::string> errors_;
  // Constructable list of ScalarResult items
  mutable std::vector<ScalarResult> items_;
};

#endif // SCALARRESULTS_H
