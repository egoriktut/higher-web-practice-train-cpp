// test_sorting_hill_stats_simple.cpp
#include "../train/sorting_hill.h"
#include <cassert>
#include <iostream>
#include <ostream>

inline void test_sorting_hill_stats_basic() {
    SortingHillStats stats;
    
    assert(stats.GetPathSetuped() == 0);
    assert(stats.GetTrinesPlaned() == 0);
    
    stats.IncPath();
    stats.IncPath();
    assert(stats.GetPathSetuped() == 2);
    
    stats.IncTrainsPlaned();
    stats.IncTrainsPlaned();
    stats.IncTrainsPlaned();
    assert(stats.GetTrinesPlaned() == 3);
    
    std::cout << "Basic stats tests passed!\n";
}

inline void test_sorting_hill_stats_locomotives() {
    SortingHillStats stats;
    Locomotive diesel{LocoType::kDiesel24};
    Locomotive electro{LocoType::kElectro32};
    
    stats.IncLoco(diesel);
    stats.IncLoco(electro);
    stats.IncLoco(electro);
    
    auto locomotives = stats.GetLocomotivesArrived();
    assert(locomotives[LocoType::kDiesel24] == 1);
    assert(locomotives[LocoType::kElectro32] == 2);
    assert(locomotives[LocoType::kElectro16] == 0);
    
    std::cout << "Locomotive stats tests passed!" << std::endl;
}

inline void test_sorting_hill_stats_wagons() {
    SortingHillStats stats;
    
    stats.IncWagon(Wagon{1, WagonType::kFreight});
    stats.IncWagon(Wagon{2, WagonType::kPass});
    stats.IncWagon(Wagon{3, WagonType::kPass});
    stats.IncWagon(Wagon{4, WagonType::kEmpty});
    stats.IncWagon(Wagon{5, WagonType::kDanger});
    
    auto wagons = stats.GetWagonStats();
    assert(wagons[WagonType::kFreight] == 1);
    assert(wagons[WagonType::kPass] == 2);
    assert(wagons[WagonType::kEmpty] == 1);
    assert(wagons[WagonType::kDanger] == 1);
    
    for (int i = 0; i < 5; i++) {
        stats.IncWagon(Wagon{10 + i, WagonType::kFreight});
    }
    
    wagons = stats.GetWagonStats();
    assert(wagons[WagonType::kFreight] == 6);
    
    std::cout << "Wagon stats tests passed!" << std::endl;
}

inline void test_sorting_hill_stats_trains() {
    SortingHillStats stats;
    
    stats.IncTrainsSent(TrainType::kFreight);
    stats.IncTrainsSent(TrainType::kPass);
    stats.IncTrainsSent(TrainType::kFreight);
    stats.IncTrainsSent(TrainType::kDanger);
    
    auto trains = stats.GetTrainsSentAmount();
    assert(trains[TrainType::kFreight] == 2);
    assert(trains[TrainType::kPass] == 1);
    assert(trains[TrainType::kDanger] == 1);
    
    for (int i = 0; i < 3; i++) {
        stats.IncTrainsSent(TrainType::kPass);
    }
    
    trains = stats.GetTrainsSentAmount();
    assert(trains[TrainType::kPass] == 4);
    
    std::cout << "Train stats tests passed!" << std::endl;
}

inline int run_tests_sorting_hill_stats() {
    std::cout << "TEST SortingHillStats" << std::endl;
    std::cout << "-----------" << std::endl;
    test_sorting_hill_stats_basic();
    test_sorting_hill_stats_locomotives();
    test_sorting_hill_stats_wagons();
    test_sorting_hill_stats_trains();
    std::cout << "=== ALL SortingHillStats TESTS PASSED ===\n" << std::endl;
    return 0;
}