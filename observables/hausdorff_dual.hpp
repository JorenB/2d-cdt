#ifndef HAUSDORFF_DUAL_HPP
#define HAUSDORFF_DUAL_HPP

#include "../observable.hpp"
#include "../universe.hpp"

class HausdorffDual : public Observable {
    public:
        HausdorffDual(std::string id) : Observable(id) { name = "hausdorff_dual"; };

        void process();

	private:
		int max_epsilon;
};

#endif
