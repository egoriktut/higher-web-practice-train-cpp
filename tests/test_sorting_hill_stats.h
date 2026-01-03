// test_sorting_hill_stats_simple.cpp
#include "../train/sorting_hill.h"
#include <cassert>
#include <iostream>
#include <ostream>

inline void test_sorting_hill_stats_basic() {
    SortingHillStats stats;
    
    assert(stats.GetPathSetuped() == 0);
    assert(stats.GetTrinesPlaned() == 0);
    
    stats.incPath();
    stats.incPath();
    assert(stats.GetPathSetuped() == 2);
    
    stats.incTrainsPlaned();
    stats.incTrainsPlaned();
    stats.incTrainsPlaned();
    assert(stats.GetTrinesPlaned() == 3);
    
    std::cout << "Basic stats tests passed!\n";
}

inline void test_sorting_hill_stats_locomotives() {
    SortingHillStats stats;
    Locomotive diesel{LocoType::kDiesel24};
    Locomotive electro{LocoType::kElectro32};
    
    stats.incLoco(diesel);
    stats.incLoco(electro);
    stats.incLoco(electro);
    
    auto locomotives = stats.GetLocomotivesArrived();
    assert(locomotives[LocoType::kDiesel24] == 1);
    assert(locomotives[LocoType::kElectro32] == 2);
    assert(locomotives[LocoType::kElectro16] == 0);
    
    std::cout << "Locomotive stats tests passed!" << std::endl;
}

inline void test_sorting_hill_stats_wagons() {
    SortingHillStats stats;
    
    stats.incWagon(Wagon{1, WagonType::kFreight});
    stats.incWagon(Wagon{2, WagonType::kPass});
    stats.incWagon(Wagon{3, WagonType::kPass});
    stats.incWagon(Wagon{4, WagonType::kEmpty});
    stats.incWagon(Wagon{5, WagonType::kDanger});
    
    auto wagons = stats.GetWagonStats();
    assert(wagons[WagonType::kFreight] == 1);
    assert(wagons[WagonType::kPass] == 2);
    assert(wagons[WagonType::kEmpty] == 1);
    assert(wagons[WagonType::kDanger] == 1);
    
    for (int i = 0; i < 5; i++) {
        stats.incWagon(Wagon{10 + i, WagonType::kFreight});
    }
    
    wagons = stats.GetWagonStats();
    assert(wagons[WagonType::kFreight] == 6);
    
    std::cout << "Wagon stats tests passed!" << std::endl;
}

inline void test_sorting_hill_stats_trains() {
    SortingHillStats stats;
    
    stats.incTrainsSent(TrainType::kFreight);
    stats.incTrainsSent(TrainType::kPass);
    stats.incTrainsSent(TrainType::kFreight);
    stats.incTrainsSent(TrainType::kDanger);
    
    auto trains = stats.GetTrainsSentAmount();
    assert(trains[TrainType::kFreight] == 2);
    assert(trains[TrainType::kPass] == 1);
    assert(trains[TrainType::kDanger] == 1);
    
    for (int i = 0; i < 3; i++) {
        stats.incTrainsSent(TrainType::kPass);
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