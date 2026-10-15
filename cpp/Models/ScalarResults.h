#ifndef SCALARRESULTS_H
#define SCALARRESULTS_H

#include "IScalarResultReceiver.h"
#include "ScalarResult.h"
#include <map>
#include <vector>
#include <optional>
#include <string>
#include <iterator>

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

        using ResultsIt = std::map<std::string, double>::const_iterator;
        using ErrorsIt = std::map<std::string, std::string>::const_iterator;

        Iterator() = default;
        explicit Iterator(ResultsIt rIt, ResultsIt rEnd, ErrorsIt eIt,
                        ErrorsIt eEnd)
            : rIt_(rIt), rEnd_(rEnd), eIt_(eIt), eEnd_(eEnd) {}

        // Iterator must be constructable from ScalarResults parent
        Iterator &operator++();
        ScalarResult operator*() const;
        bool operator!=(const Iterator &other) const;

  private:
        bool iterateResults() const;
        bool iterateErrors() const;

        ResultsIt rIt_, rEnd_;
        ErrorsIt eIt_, eEnd_;
    };

    Iterator begin() const;
    Iterator end() const;

private:
    std::map<std::string, double> results_;
    std::map<std::string, std::string> errors_;
};

#endif // SCALARRESULTS_H
