#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <utility>

struct Point2D {
    int x, y;

    bool operator==(const Point2D& other) const {
        return x == other.x && y == other.y;
    }

    Point2D operator+(const Point2D& other) const {
        return {x + other.x, y + other.y};
    }

    static const Point2D NORTH, EAST, SOUTH, WEST;
};

const Point2D Point2D::NORTH = {0, -1};
const Point2D Point2D::EAST = {1, 0};
const Point2D Point2D::SOUTH = {0, 1};
const Point2D Point2D::WEST = {-1, 0};

namespace std {
    template<> struct hash<Point2D> {
        size_t operator()(const Point2D& p) const {
            return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
        }
    };

    template<> struct hash<pair<Point2D, Point2D>> {
        size_t operator()(const pair<Point2D, Point2D>& p) const {
            return hash<Point2D>()(p.first) ^ (hash<Point2D>()(p.second) << 1);
        }
    };
}

struct Location {
    std::vector<Point2D> positions;
    Point2D direction;

    Point2D position() const {
        return positions.back();
    }

    std::pair<Point2D, Point2D> key() const {
        return {position(), direction};
    }

    Location step() const {
        auto nextPositions = positions;
        nextPositions.push_back(position() + direction);
        return {nextPositions, direction};
    }

    Location clockwise() const {
        Point2D newDirection;
        if (direction == Point2D::NORTH) newDirection = Point2D::EAST;
        else if (direction == Point2D::EAST) newDirection = Point2D::SOUTH;
        else if (direction == Point2D::SOUTH) newDirection = Point2D::WEST;
        else if (direction == Point2D::WEST) newDirection = Point2D::NORTH;
        else throw std::logic_error("Invalid direction");
        return {positions, newDirection};
    }

    Location antiClockwise() const {
        Point2D newDirection;
        if (direction == Point2D::NORTH) newDirection = Point2D::WEST;
        else if (direction == Point2D::WEST) newDirection = Point2D::SOUTH;
        else if (direction == Point2D::SOUTH) newDirection = Point2D::EAST;
        else if (direction == Point2D::EAST) newDirection = Point2D::NORTH;
        else throw std::logic_error("Invalid direction");
        return {positions, newDirection};
    }
};

class Day16 {
public:
    Day16(const std::vector<std::string>& maze) : maze(maze) {
        start = find('S');
        end = find('E');
    }

    int solvePart1() {
        return traverseMaze();
    }

    int solvePart2() {
        return traverseMazeWithPath();
    }

private:
    std::vector<std::string> maze;
    Point2D start, end;

    Point2D find(char target) {
        for (size_t y = 0; y < maze.size(); ++y) {
            for (size_t x = 0; x < maze[y].size(); ++x) {
                if (maze[y][x] == target) {
                    return {static_cast<int>(x), static_cast<int>(y)};
                }
            }
        }
        throw std::logic_error("Target not found");
    }

    char at(const Point2D& p) const {
        return maze[p.y][p.x];
    }

    int traverseMaze() {
        using State = std::pair<Location, int>;
        auto cmp = [](const State& a, const State& b) { return a.second > b.second; };
        std::priority_queue<State, std::vector<State>, decltype(cmp)> queue(cmp);

        queue.push({{{start}, Point2D::EAST}, 0});
        std::unordered_map<std::pair<Point2D, Point2D>, int> seen;

        while (!queue.empty()) {
            auto [location, cost] = queue.top();
            queue.pop();

            if (location.position() == end) {
                return cost;
            } else if (seen[location.key()] > cost) {
                seen[location.key()] = cost;
                auto next = location.step();
                if (at(next.position()) != '#') {
                    queue.push({next, cost + 1});
                }
                queue.push({location.clockwise(), cost + 1000});
                queue.push({location.antiClockwise(), cost + 1000});
            }
        }

        throw std::logic_error("No path to goal");
    }

    int traverseMazeWithPath() {
        using State = std::pair<Location, int>;
        auto cmp = [](const State& a, const State& b) { return a.second > b.second; };
        std::priority_queue<State, std::vector<State>, decltype(cmp)> queue(cmp);

        queue.push({{{start}, Point2D::EAST}, 0});
        std::unordered_map<std::pair<Point2D, Point2D>, int> seen;
        int costAtGoal = -1;
        std::unordered_set<Point2D> allSpotsInAllPaths;

        while (!queue.empty()) {
            auto [location, cost] = queue.top();
            queue.pop();

            if (costAtGoal != -1 && cost > costAtGoal) {
                return allSpotsInAllPaths.size();
            } else if (location.position() == end) {
                costAtGoal = cost;
                allSpotsInAllPaths.insert(location.positions.begin(), location.positions.end());
            } else if (seen[location.key()] >= cost) {
                seen[location.key()] = cost;
                auto next = location.step();
                if (at(next.position()) != '#') {
                    queue.push({next, cost + 1});
                }
                queue.push({location.clockwise(), cost + 1000});
                queue.push({location.antiClockwise(), cost + 1000});
            }
        }

        return allSpotsInAllPaths.size();
    }
};
