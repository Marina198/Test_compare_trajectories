//
﻿#include <iostream>
#include <map>
#include <string_view>
#include <functional>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <utility>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <limits>
#include <variant>

struct Trajectory
{
    struct Point
    {
        int x;
        int y;
    };

    using TimeMarker = int;
    using TimePoint = std::pair<TimeMarker, Point>;
    using Time2PointMap = std::multimap<TimeMarker, Point>;

    friend std::istream& operator>>(std::istream& is, TimePoint& time_point) {
        auto& [time_marker, point] = time_point;
        is >> point.x >> point.y;
        is >> time_marker;
        return is;
    }

    friend std::istream& operator>>(std::istream& is, Trajectory& traj) {
        int num_points;
        is >> num_points;

        std::copy_n(std::istream_iterator<TimePoint>(is), num_points, std::inserter(traj.points_, traj.points_.end()));

        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const Trajectory& traj) {
        for (const auto [time_marker, point] : traj.points_) {
            os << point.x << ' ' << point.y << ' ' << time_marker << ' ';
        }
        return os;
    }

    Time2PointMap points_;
};

class App {
public:
    using MetricNameToFunctorMap = std::map<std::string_view, std::function<double(const Trajectory&)>>;
    App(int argc, char* argv[], MetricNameToFunctorMap metrics, std::istream& is = std::cin, std::ostream& os = std::cout)
        : is_{ is }
        , os_{ os }
        , metrics_{ std::move(metrics) } {
        if (argc < 3)
            throw std::runtime_error("Usage: Test_compare_trajectories <file_name> <metric1> <metric2> ... <metricN>");

        if (!std::filesystem::exists(argv[1]))
            throw std::runtime_error("File not found: " + std::string(argv[1]));

        file_name_ = argv[1];

        auto check_metric = [this](char* metric) {
            auto check_metric = [this](char* metric) {
                try {
                    applied_metrics_.emplace_back(metrics_.at(metric));
                }
                catch (const std::out_of_range&) {
                    throw std::runtime_error("Unknown metric: " + std::string(metric));
                }
            };
        };

        // std::span<char*> vMetrics(argv + 2, argv + argc); // C++20
        std::vector<char*> vMetrics(argv + 2, argv + argc);

        for (auto& metric : vMetrics) {
            check_metric(metric);
        }
    }

    virtual ~App() = default;

    int Run() {
        std::ifstream file(file_name_);

        int num_trajectories;
        file >> num_trajectories;

        while (num_trajectories--)
        {
            Trajectory traj;
            file >> traj;
            trajectories_.emplace_back(traj);
        }

        is_ >> reference_trajectory_;

        for (auto& metric : applied_metrics_) {
            ApplyMetric(metric);
        }

        return 0;
    }
protected:
    using MetricResults = std::vector<std::pair<double, size_t>>;
    virtual void Result(const std::string_view metric_name, double reference_result, MetricResults trajectories_results) {
        std::sort(trajectories_results.begin(), trajectories_results.end(),
            [reference_result](const auto& lhs, const auto& rhs) {
                return std::fabs(lhs.first - reference_result) < std::fabs(rhs.first - reference_result);
            });

        int num_closest = trajectories_results.size() < 3 ? trajectories_results.size() : 3;
        os_ << std::fixed << std::setprecision(2);
        os_ << num_closest << " closest trajectories by " << metric_name << ": " << reference_result << ",  to reference trajectory:\n";
        std::for_each_n(trajectories_results.begin(), num_closest, [this, &metric_name, reference_result](const auto& traj) {
            auto [metric, index] = traj;
            os_ << "Trajectory: " << index << ", " << metric_name << ": " << metric << ", diff: " << std::fabs(traj.first - reference_result) << '\n';
            });
    }

private:
    template<typename MetricFunctor>
    MetricResults CalculateTrajectoriesResults(MetricFunctor& metric_functor) {
        MetricResults trajectories_results;
        for (size_t i = 0; i != trajectories_.size(); ++i) {
            trajectories_results.emplace_back(metric_functor(trajectories_[i]), i);
        }
        return trajectories_results;
    }

    void ApplyMetric(MetricNameToFunctorMap::value_type& metric) {
        auto& [metric_name, metric_functor] = metric;
        MetricResults trajectories_results = CalculateTrajectoriesResults(metric_functor);
        auto reference_result = metric_functor(reference_trajectory_);
        Result(metric_name, reference_result, std::move(trajectories_results));
    }

private:
    std::istream& is_;
    std::ostream& os_;
    MetricNameToFunctorMap metrics_;
    std::string file_name_;
    std::vector<MetricNameToFunctorMap::value_type> applied_metrics_;
    std::vector<Trajectory> trajectories_;
    Trajectory reference_trajectory_;
    // todo: add cache of calculated metrics
};

//todo: fabric method for metrics
auto lengthMetric = [](const Trajectory& traj) -> double {
    const auto& points = traj.points_;

    auto it = points.begin();
    auto itprev = it;
    std::advance(it, 1);
    double length = 0.0;

    std::for_each(it, points.end(),
        [&itprev, &length](const auto& current_point_pair) {
            auto prev_point_pair = *itprev;
            const auto& current_point = current_point_pair.second;
            const auto& prev_point = prev_point_pair.second;

            length += sqrt(pow(current_point.x - prev_point.x, 2) + pow(current_point.y - prev_point.y, 2));
            ++itprev;
        });
    return length;
};

auto speedMetric = [](const Trajectory& traj) -> double {
    auto points = traj.points_;
    double result = 0.0;

    if (auto total_time = points.rbegin()->first - points.begin()->first; total_time != 0) {
        result = lengthMetric(traj) / total_time;
    }

    return result;
};

auto timeMetric = [](const Trajectory& traj) -> double {
    return static_cast<double>(traj.points_.rbegin()->first - traj.points_.begin()->first);
};

int main(int argc, char* argv[])
{
    App::MetricNameToFunctorMap metrics{
        {"length", lengthMetric},
        {"speed", speedMetric},
        {"time", timeMetric}
    };

    try {
        return App(argc, argv, metrics).Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}