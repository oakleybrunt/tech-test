#ifndef CORPBONDPRICINGENGINE_H
#define CORPBONDPRICINGENGINE_H

#include "BasePricingEngine.h"

class CorpBondPricingEngine : public BasePricingEngine {
public:
  CorpBondPricingEngine() {
    setDelay(800);
    addSupportedTradeType("CorpBond");
  }
};

#endif // CORPBONDPRICINGENGINE_H
