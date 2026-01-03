// test_sorting_operator_simple.cpp
#include "../train/sorting_operator.h"
#include "../train/sorting_hill.h"
#include <cassert>
#include <memory>
#include <iostream>

inline void test_sorting_operator_actions() {
    std::vector<std::unique_ptr<SortingHandler>> handlers;
    handlers.push_back(std::make_unique<SortingOperatorImpl>());
    
    SortingHill hill(2, std::move(handlers));
    SortingOperatorImpl op;
    OperationInfo info;
    op.StartShift(hill);
    
    op.PreparePath(hill, info);
    
    Locomotive loco{LocoType::kDiesel24};
    op.HandleLocomotive(hill, loco, info);
    
    
    Wagon wagon{789, WagonType::kFreight};
    op.HandleWagon(hill, wagon, info);
    
    hill.AddLocomotiveToQueue(Locomotive{LocoType::kElectro16});
    op.AllocatePathForTrain(hill, info);

    
    op.EndShift(hill);
    
    std::cout << "All SortingOperator tests passed!" << std::endl;
}


inline int run_tests_sorting_operator() {
    std::cout << "TEST SortingOperator" << std::endl;
    std::cout << "-----------" << std::endl;
    test_sorting_operator_actions();
    std::cout << "=== ALL SortingOperator TESTS PASSED ===\n" << std::endl;
    return 0;
}