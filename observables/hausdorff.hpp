#ifndef hausdorff_hpp
#define hausdorff_hpp

#include "../observable.hpp"
#include "../universe.hpp"

class Hausdorff : public Observable {
    public:
        Hausdorff(std::string id) : Observable(id) { name = "hausdorff"; };

        void process();

	private:
		int max_epsilon;
};

#endif
