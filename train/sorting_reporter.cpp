#include "sorting_reporter.h"
#include "sorting_hill.h"

#include <iostream>

using namespace std::literals;

SortingReporterImpl::~SortingReporterImpl() {}

void SortingReporterImpl::StartShift(const SortingHill& sorting_hill) {
    std::cout << "Начало рабочей смены\n"s;
    std::cout << "Очередь вагонов: "s << sorting_hill.GetNumberOfWagBuffer() << std::endl;
}

void SortingReporterImpl::EndShift(const SortingHill& sorting_hill) {
    SortingHillStats stats = sorting_hill.GetStats();
    std::cout << "Рабочая смена окончена\n\n"s;
    std::cout << "===== ОТЧЁТ О СМЕНЕ =====\n"s;
    std::cout << "Путей:             "s << sorting_hill.GetNumberOfPaths() << "\n";
    std::cout << "Подготовлено путей:                    "s << stats.GetPathSetuped() << "\n";
    std::cout << "Запланировано поездов:                 "s << stats.GetTrinesPlaned() << "\n";
    std::cout << "Прибыло локомотивов:                   "s << stats.GetLocomotivesArrived() << "\n";
    std::cout << "Обработано вагонов (с повторами):      "s << stats.GetWagonStats() << "\n";
    std::cout << "Отправлено поездов:                    "s << stats.GetTrainsSentAmount() << "\n";
    std::cout << "Осталось вагонов в буфере:             "s << sorting_hill.GetWagQueue() << "\n";
    std::cout << "=========================="s << std::endl;
}


void SortingReporterImpl::PreparePath(SortingHill& sorting_hill, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.IncPath();
    }
}

void SortingReporterImpl::AllocatePathForTrain(SortingHill& sorting_hill, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.IncTrainsPlaned();
    }
}

void SortingReporterImpl::HandleLocomotive(SortingHill& sorting_hill, const Locomotive& locomotive, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.IncLoco(locomotive);
    }
}

void SortingReporterImpl::HandleWagon(SortingHill& sorting_hill, const Wagon& wagon, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.IncWagon(wagon);
    }
}

void SortingReporterImpl::SendTrain(SortingHill& sorting_hill, OperationInfo& operation_info) {
    if (sorting_hill.IsChangedCommited()) {
        sorting_hill.IncTrainsSent();
    }
}
