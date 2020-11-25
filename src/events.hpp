#pragma once

#include <vector>

enum EventType
{
    CONTROLLABLE,
    UNCONTROLLABLE,
    EXPECTED
};

using EventsMap = std::vector<EventType>;
