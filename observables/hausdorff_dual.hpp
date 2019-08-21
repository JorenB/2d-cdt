#ifndef hausdorff_dual_hpp
#define hausdorff_dual_hpp

#include "../observable.hpp"
#include "../universe.hpp"

class HausdorffDual : public Observable {
    public:
        HausdorffDual() : Observable() { name = "hausdorff_dual"; };

        void process();

	private:
		int max_epsilon;
};

#endif
