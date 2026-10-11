#include "ScreenResultPrinter.h"
#include <iostream>

void ScreenResultPrinter::printResults(ScalarResults &results) {
  for (const auto &result : results) {
    // Write code here to print out the results such that we have:
    // TradeID : Result : Error
    std::cout << result.getTradeId();

    // Since getResult and getError return optionals, we can check if they hold
    // a value after copying them into local variables
    auto r = result.getResult();
    auto e = result.getError();
    // If there is no result then the output should be:
    // TradeID : Error
    // If there is no error the output should be:
    // TradeID : Result
    if (r) {
      std::cout << " : " << *r;
    }
    if (e) {
      std::cout << " : " << *e;
    }
    std::cout << std::endl;
  }
}
