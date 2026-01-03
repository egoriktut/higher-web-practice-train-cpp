#include "sorting_operator.h"

SortingOperatorImpl::~SortingOperatorImpl() {}

void SortingOperatorImpl::StartShift(const SortingHill&) {}

void SortingOperatorImpl::EndShift(const SortingHill&) {}

void SortingOperatorImpl::PreparePath(SortingHill& sorting_hill, OperationInfo& operation_info) {
    sorting_hill.SetupPath();
}

void SortingOperatorImpl::AllocatePathForTrain(SortingHill& sorting_hill, OperationInfo& operation_info) {
    sorting_hill.SetupLocomotiveToPath();
}

void SortingOperatorImpl::HandleLocomotive(SortingHill& sorting_hill, const Locomotive& locomotive, OperationInfo& operation_info) {
    sorting_hill.AddLocomotiveToQueue(locomotive);
}

void SortingOperatorImpl::HandleWagon(SortingHill& sorting_hill, const Wagon& wagon, OperationInfo& operation_info) {
    sorting_hill.AddWagonToQueue(wagon);
}

void SortingOperatorImpl::SendTrain(SortingHill& sorting_hill, OperationInfo& operation_info) {
    sorting_hill.SendTrain();
}
