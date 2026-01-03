#include "sorting_reporter.h"
#include "sorting_hill.h"

#include <iostream>

using namespace std::literals;

SortingReporterImpl::~SortingReporterImpl() {}

void SortingReporterImpl::StartShift(const SortingHill& sorting_hill) {
    std::cout << "Начало рабочей смены"s << std::endl;
    std::cout << "Очередь вагонов: "s << sorting_hill.GetNumberOfWagBuffer() << std::endl;
}

void SortingReporterImpl::EndShift(const SortingHill& sorting_hill) {
    SortingHillStats stats = sorting_hill.GetStats();
    std::cout << "Рабочая смена окончена"s << std::endl;
    std::cout << std::endl;
    std::cout << "===== ОТЧЁТ О СМЕНЕ ====="s << std::endl;
    std::cout << "Путей:             "s << sorting_hill.GetNumberOfPaths() << std::endl;
    std::cout << "Подготовлено путей:                    "s << stats.GetPathSetuped() << std::endl;
    std::cout << "Запланировано поездов:                 "s << stats.GetTrinesPlaned() << std::endl;
    std::cout << "Прибыло локомотивов:                   "s << stats.GetLocomotivesArrived() << std::endl;
    std::cout << "Обработано вагонов (с повторами):      "s << stats.GetWagonStats() << std::endl;
    std::cout << "Отправлено поездов:                    "s << stats.GetTrainsSentAmount() << std::endl;
    std::cout << "Осталось вагонов в буфере:             "s << sorting_hill.GetWagQueue() << std::endl;
    std::cout << "=========================="s << std::endl;
}


void SortingReporterImpl::PreparePath(SortingHill& sorting_hill, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.incPath();
    }
}

void SortingReporterImpl::AllocatePathForTrain(SortingHill& sorting_hill, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.incTrainsPlaned();
    }
}

void SortingReporterImpl::HandleLocomotive(SortingHill& sorting_hill, const Locomotive& locomotive, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.incLoco(locomotive);
    }
}

void SortingReporterImpl::HandleWagon(SortingHill& sorting_hill, const Wagon& wagon, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.incWagon(wagon);
    }
}

void SortingReporterImpl::SendTrain(SortingHill& sorting_hill, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.incTrainsSent();
    }
}
