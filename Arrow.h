#pragma once
#include<tuple>
#include<set>
#ifndef ARROW_H
#define ARROW_H

struct Arrow {
    int startX, startY;
    int endX, endY;
    bool operator <(const Arrow& other) const {
        return std::tie(startX, startY, endX, endY) < std::tie(other.startX, other.startY, other.endX, other.endY);
    }
};
#endif