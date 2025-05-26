// Copyright 2023 Google LLC
#pragma once

#include <string>
#include "../observable.hpp"
#include "../universe.hpp"

class AverageSphereDistance : public Observable {
public:
    AverageSphereDistance(std::string id, int radius, int num_origins);
    void process() override;

private:
    int radius;
    int num_origins;
};
