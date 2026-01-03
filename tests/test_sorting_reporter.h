
#include "../train/sorting_reporter.h"
#include "../train/sorting_hill.h"
#include <cassert>
#include <memory>
#include <iostream>
#include <sstream>

inline void test_sorting_reporter_output() {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    std::vector<std::unique_ptr<SortingHandler>> handlers;
    handlers.push_back(std::make_unique<SortingReporterImpl>());
    
    SortingHill hill(2, std::move(handlers));
    SortingReporterImpl reporter;
    
    reporter.StartShift(hill);
    std::string output = buffer.str();
    assert(output.find("Начало") != std::string::npos);
    assert(output.find("смены") != std::string::npos);
    
    buffer.str("");
    
    reporter.EndShift(hill);
    output = buffer.str();
    assert(output.find("Рабочая смена окончена") != std::string::npos);
    assert(output.find("ОТЧЁТ") != std::string::npos);
    
    std::cout.rdbuf(old);
    
    std::cout << "Output tests passed!" << std::endl;;
}

inline void test_sorting_reporter_statistics() {
    std::vector<std::unique_ptr<SortingHandler>> handlers;
    handlers.push_back(std::make_unique<SortingReporterImpl>());
    
    SortingHill hill(3, std::move(handlers));
    SortingReporterImpl reporter;
    OperationInfo info;
    
    hill.HandleEvent(EventType::kPreparePath);
    hill.HandleEvent(EventType::kLocoArrived);
    
    Wagon wagon{111, WagonType::kDanger};
    hill.AddWagonToQueue(wagon);
    
    reporter.PreparePath(hill, info);
    reporter.HandleLocomotive(hill, Locomotive{LocoType::kDiesel24}, info);
    reporter.HandleWagon(hill, wagon, info);
    
    auto stats = hill.GetStats();
    assert(stats.GetPathSetuped() == 1);
    
    auto locomotives = stats.GetLocomotivesArrived();
    assert(locomotives[LocoType::kDiesel24] > 0);
    
    auto wagonStats = stats.GetWagonStats();
    assert(wagonStats[WagonType::kDanger] > 0);
    
    std::cout << "Statistics reporting tests passed!" << std::endl;;
}

inline void test_sorting_reporter_train_sending() {
    std::vector<std::unique_ptr<SortingHandler>> handlers;
    handlers.push_back(std::make_unique<SortingReporterImpl>());
    
    SortingHill hill(2, std::move(handlers));
    SortingReporterImpl reporter;
    OperationInfo info;
    
    hill.HandleEvent(EventType::kPreparePath);
    hill.HandleEvent(EventType::kLocoArrived);
    
    for (int i = 0; i < 10; i++) {
        Wagon wagon{200 + i, WagonType::kFreight};
        hill.AddWagon(wagon);
        hill.HandleEvent(EventType::kWagonArrived);
    }
    
    hill.HandleEvent(EventType::kTrainReady);
    
    reporter.SendTrain(hill, info);
    
    std::cout << "Train sending reports tests passed!" << std::endl;;
}

inline int run_tests_sorting_reporter() {
    std::cout << "TEST SortingReporter" << std::endl;
    std::cout << "-----------" << std::endl;
    test_sorting_reporter_output();
    test_sorting_reporter_statistics();
    test_sorting_reporter_train_sending();
    std::cout << "=== ALL SortingReporter TESTS PASSED ===\n" << std::endl;;
    return 0;
}