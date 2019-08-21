#ifndef hausdorff_hpp
#define hausdorff_hpp

#include "../observable.hpp"
#include "../universe.hpp"

class Hausdorff : public Observable {
    public:
        Hausdorff() : Observable() { name = "hausdorff"; };

        void process();

	private:
		int max_epsilon;
};

#endif
