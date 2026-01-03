#pragma once

#include "enums.h"

#include <array>
#include <cstddef>
#include <iostream>
#include <optional>
#include <vector>
#include <string>
#include <map>

struct Wagon {
    int number;
    WagonType wagon_type;
};

struct Locomotive {
    LocoType loco_type;
};

struct OperationInfo {

};

struct Train {
    std::optional<Locomotive> locomotive;
    std::optional<TrainType> train_type;
    std::optional<int> number;
    std::vector<Wagon> wagons;
};

const static std::map<LocoType, int> locomotiveCapacity = {
    {LocoType::kDiesel24, 24},
    {LocoType::kDiesel64, 64},
    {LocoType::kElectro16, 16},
    {LocoType::kElectro32, 32},
};

const static std::vector<WagonType> wagonPriorityToSend = {
    WagonType::kPass,
    WagonType::kFreight,
    WagonType::kDanger,
};

const static std::map<TrainType, int> trainPriorityToSend = {
    {TrainType::kPass, 1},
    {TrainType::kFreight, 2},
    {TrainType::kDanger, 3},
};

const static std::map<WagonType, TrainType> wagonTrainMapper = {
    {WagonType::kPass, TrainType::kPass},
    {WagonType::kDanger, TrainType::kDanger},
    {WagonType::kFreight, TrainType::kFreight},
};

const static std::map<TrainType, WagonType> trainWagonMapper = {
    {TrainType::kPass, WagonType::kPass},
    {TrainType::kDanger, WagonType::kDanger},
    {TrainType::kFreight, WagonType::kFreight},
};

inline constexpr std::array<EventType, 17> kEventsBalanced = {
    EventType::kWagonArrived, EventType::kWagonArrived, EventType::kWagonArrived, EventType::kWagonArrived,
    EventType::kWagonArrived, EventType::kWagonArrived, EventType::kWagonArrived, EventType::kWagonArrived,
    EventType::kWagonArrived, 
    // След. строку мжно закомментить, чтобы оставались в буфере поезда
    EventType::kLocoArrived, EventType::kLocoArrived, EventType::kLocoArrived,
    EventType::kLocoArrived, EventType::kPreparePath, EventType::kPreparePath, EventType::kTrainPlanned,
    EventType::kTrainReady
};

inline constexpr std::array<TrainType, 3> kTrainType = {
    TrainType::kFreight, TrainType::kPass, TrainType::kDanger
};

inline constexpr std::array<WagonType, 4> kWagonType = {
    WagonType::kEmpty, WagonType::kFreight, WagonType::kPass, WagonType::kDanger
};

inline constexpr std::array<LocoType, 4> kLocoType = {
    LocoType::kElectro16, LocoType::kElectro32, LocoType::kDiesel24, LocoType::kDiesel64
};

inline std::ostream& operator<<(std::ostream& os, EventType event_type) {
    using namespace std::literals;
    switch (event_type) {
        case EventType::kShiftStarted:
            os << "начало работ"s;
            break;
        case EventType::kPreparePath:
            os << "готовьте путь"s;
            break;
        case EventType::kTrainPlanned:
            os << "формируйте состав"s;
            break;
        case EventType::kLocoArrived:
            os << "подача локомотива"s;
            break;
        case EventType::kWagonArrived:
            os << "вагон на сортировку"s;
            break;
        case EventType::kTrainReady:
            os << "отправляйте поезд"s;
            break;
        case EventType::kShiftEnded:
            os << "окончание работ"s;
            break;
        default:
            os << "неизвестно"s;
            break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, TrainType train_type) {
    using namespace std::literals;
    switch (train_type) {
        case TrainType::kFreight:
            os << "О"s;
            break;
        case TrainType::kDanger:
            os << "Г"s;
            break;
        case TrainType::kPass:
            os << "Л"s;
            break;
        default:
            os << ""s;
            break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, WagonType wagon_type) {
    using namespace std::literals;
    switch (wagon_type) {
        case WagonType::kFreight:
            os << "О"s;
            break;
        case WagonType::kDanger:
            os << "Г"s;
            break;
        case WagonType::kPass:
            os << "Л"s;
            break;
        case WagonType::kEmpty:
            os << "П"s;
            break;
        default:
            os << ""s;
            break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, LocoType loco_type) {
    using namespace std::literals;
    switch (loco_type) {
        case LocoType::kDiesel24:
            os << "Дизель 24"s;
            break;
        case LocoType::kDiesel64:
            os << "Дизель 64"s;
            break;
        case LocoType::kElectro16:
            os << "Элка 16"s;
            break;
        case LocoType::kElectro32:
            os << "Элка 32"s;
            break;
        default:
            os << ""s;
            break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::map<WagonType, std::vector<Wagon>> &wagons) {
    size_t sum_size = 0;
    for (const auto &[wagon_type, wagon_vect] : wagons) {
        std::cout << wagon_type << ": " << wagon_vect.size() << "; ";
        sum_size += wagon_vect.size();
    }
    std::cout << " \\ " << sum_size;
    return os;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const std::map<T, size_t> &mapper_amount) {
    size_t sum_size = 0;
    for (const auto [current_type, size_of_current_type] : mapper_amount) {
        std::cout << current_type << ": " << size_of_current_type << "; ";
        sum_size += size_of_current_type;
    }
    std::cout << " \\ " << sum_size;
    return os;
}