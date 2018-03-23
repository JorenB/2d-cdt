#ifndef simplex_hpp
#define simplex_hpp

class Simplex {
    public:
        int getKey() const { return key_; }          // Return key
        void setKey(int key) { key_ = key; }							// Set key

    private:
        int key_;														// Label or next free entry in pool
};

#endif
