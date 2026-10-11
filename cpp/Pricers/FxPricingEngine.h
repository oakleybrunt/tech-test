#ifndef FXPRICINGENGINE_H
#define FXPRICINGENGINE_H

#include "BasePricingEngine.h"

class FxPricingEngine : public BasePricingEngine {
public:
  FxPricingEngine() {
    setDelay(200);
    addSupportedTradeType("FxSpot");
    addSupportedTradeType("FxFwd");
  }
};

#endif // FXPRICINGENGINE_H
