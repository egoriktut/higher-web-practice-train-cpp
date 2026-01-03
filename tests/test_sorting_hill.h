#include "../train/sorting_hill.h"
#include "../train/handler_interface.h"
#include "../train/enums.h"
#include <cassert>
#include <memory>
#include <iostream>

class SimplMeHandlerMock : public SortingHandler {
public:
    void StartShift(const SortingHill&) override {}
    void EndShift(const SortingHill&) override {}
    void PreparePath(SortingHill&, OperationInfo&) override {}
    void AllocatePathForTrain(SortingHill&, OperationInfo&) override {}
    void HandleLocomotive(SortingHill&, const Locomotive&, OperationInfo&) override {}
    void HandleWagon(SortingHill&, const Wagon&, OperationInfo&) override {}
    void SendTrain(SortingHill&, OperationInfo&) override {}
};

inline void test_sorting_hill_basic() {
    std::vector<std::unique_ptr<SortingHandler>> handlers;
    handlers.push_back(std::make_unique<SimplMeHandlerMock>());
    
    SortingHill hill(3, std::move(handlers));
    
    assert(hill.GetNumberOfPaths() == 3);
    assert(hill.GetNumberOfWagBuffer() == 0);
    assert(!hill.IsWagonBuffer());
    
    Wagon wagon{123, WagonType::kFreight};
    hill.AddWagon(wagon);
    assert(hill.GetNumberOfWagBuffer() == 1);
    assert(hill.IsWagonBuffer());
    
    assert(hill.CheckHasFreePaths());
    
    std::cout << "All basic tests passed!" << std::endl;
}

inline void test_sorting_hill_events() {
    std::vector<std::unique_ptr<SortingHandler>> handlers;
    handlers.push_back(std::make_unique<SimplMeHandlerMock>());
    
    SortingHill hill(2, std::move(handlers));
    
    assert(hill.CheckEvent(EventType::kPreparePath));
    hill.HandleEvent(EventType::kPreparePath);
    
    hill.HandleEvent(EventType::kLocoArrived);
    
    hill.AddWagon(Wagon{456, WagonType::kPass});
    hill.HandleEvent(EventType::kWagonArrived);
    
    hill.HandleEvent(EventType::kTrainReady);
    
    hill.HandleEvent(EventType::kShiftEnded);
    
    std::cout << "All event tests passed!" << std::endl;
}

inline void run_tests_sorting_hill() {
    std::cout << "TEST SortingHill" << std::endl;
    std::cout << "-----------" << std::endl;
    test_sorting_hill_basic();
    test_sorting_hill_events();
    std::cout << "=== ALL SortingHill TESTS PASSED ===\n" << std::endl;
}