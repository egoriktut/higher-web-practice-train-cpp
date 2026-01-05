#include "sorting_hill.h"
#include "common.h"
#include "enums.h"
#include "random.h"

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <utility>
#include <iomanip>

/*
****** SortingHillStats
*/
size_t SortingHillStats::GetPathSetuped() const {
    return path_setuped_;
}

size_t SortingHillStats::GetTrinesPlaned() const {
    return trians_planed_;
}

std::unordered_map<LocoType, size_t> SortingHillStats::GetLocomotivesArrived() const {
    return locomotives_arrived_;
}

std::unordered_map<WagonType, size_t> SortingHillStats::GetWagonStats() const {
    return wagons_stats_;
}

std::unordered_map<TrainType, size_t> SortingHillStats::GetTrainsSentAmount() const {
    return trains_sent_;
}

void SortingHillStats::IncPath() {
    path_setuped_++;
}

void SortingHillStats::IncTrainsPlaned() {
    trians_planed_++;
}

void SortingHillStats::IncLoco(const Locomotive &locomotive) {
    locomotives_arrived_[locomotive.loco_type]++;
}

void SortingHillStats::IncWagon(const Wagon &wagon) {
    wagons_stats_[wagon.wagon_type]++;
}

void SortingHillStats::IncTrainsSent(const TrainType train_type) {
    trains_sent_[train_type]++;
}


/*
****** SortingHill
*/
SortingHill::SortingHill(size_t number_of_paths, std::vector<std::unique_ptr<SortingHandler>> handlers)
    : handlers_(std::move(handlers)),
      number_of_paths_(number_of_paths) {

    for (int i = 1; i <= number_of_paths; ++i) {
        paths_[i] = std::nullopt;
    }
}

size_t SortingHill::GetNumberOfPaths() const {
    return number_of_paths_;
}

size_t SortingHill::GetNumberOfWagBuffer() const {
    return wagon_buffer_.size();
}

std::unordered_map<WagonType, std::vector<Wagon>> SortingHill::GetWagQueue() const {
    return free_wagons_;
}

void SortingHill::AddWagon(const Wagon &wagon) {
    wagon_buffer_.push(wagon);
}

void SortingHill::PopWagon() {
    wagon_buffer_.pop();
}

bool SortingHill::IsWagonBuffer() const {
    return !wagon_buffer_.empty();
}

bool SortingHill::IsChangedCommited() const {
    return changed_commited_;
}

const SortingHillStats & SortingHill::GetStats() const {
    return stats_;
}

/* 
* Prints
*/
void SortingHill::PrintSendTrain(const int path_index) const {
    const auto path_it = paths_.find(path_index);
    if (path_it == paths_.end()) {
        return;
    }
    const Train & train = path_it->second.value();
    const std::vector<Wagon> & wagons = path_it->second.value().wagons;
    int empty_wagons_in_train = std::count_if(
        wagons.begin(), wagons.end(), [](const Wagon &wagon) {
                return wagon.wagon_type == WagonType::kEmpty;
        }
    );
    std::cout << "Отправлен поезд: " 
        << std::setfill('0') 
        << std::setw(4)
        << train.number.value()
        << train.train_type.value() << " \\ "
        << "локомотив: " << train.locomotive.value().loco_type << " \\ "
        << "вагонов в составе всего: " << wagons.size() << " -> "
        << "специальных: " << wagons.size() - empty_wagons_in_train 
        << ", пустых: " << empty_wagons_in_train
    << std::endl;
}

/* 
* Stats
*/
void SortingHill::IncPath() { 
    stats_.IncPath();
 }
void SortingHill::IncTrainsPlaned() { 
    stats_.IncTrainsPlaned();
 }
void SortingHill::IncLoco(const Locomotive &locomotive) { 
    stats_.IncLoco(locomotive);
 }
void SortingHill::IncWagon(const Wagon& wagon) { 
    stats_.IncWagon(wagon);
 }
void SortingHill::IncTrainsSent() {
    stats_.IncTrainsSent(last_sent_train_.value().train_type.value()); 
    last_sent_train_.reset();
}


/* 
* Вспомогательные методы
*/
int SortingHill::GetFirstFreePath() const {
    int first_free_path_num = -1;
    for (const auto &[path_num, train] : paths_) {
        if (!train.has_value()) {
            first_free_path_num = path_num;
            break;
        }
    }
    return first_free_path_num;
}

int SortingHill::GetFirstSetupedPathWithoutTrain() const {
    int first_setup_whitout_train_path_num = -1;
    for (const auto &[path_num, train] : paths_) {
        if (train.has_value() && !train.value().locomotive.has_value()) {
            first_setup_whitout_train_path_num = path_num;
            break;
        }
    }
    return first_setup_whitout_train_path_num;
}

void SortingHill::LoadTrainTypeWagons(std::optional<Train> &train) {
    if (
        !train.has_value() 
        || !train.value().train_type.has_value()
         || !train.value().locomotive.has_value()
    ) {
        return;
    }
    // Не считаю что тут надо менять на find, это статический мапы с типами.
    const WagonType loading_wagon_type = trainWagonMapper.at(train.value().train_type.value());
    const int current_locomotive_capacity = locomotiveCapacity.at(train.value().locomotive.value().loco_type);
    while (
        free_wagons_.at(loading_wagon_type).size() > 0 
        &&
        current_locomotive_capacity > train.value().wagons.size() 
    ) {
        train.value().wagons.push_back(free_wagons_.at(loading_wagon_type).back());
        free_wagons_[loading_wagon_type].pop_back();
    }
}

void SortingHill::ManageWagons() {
    for (auto &[path_num, train] : paths_) {
        if (!train.has_value() || !train.value().locomotive.has_value()) {
            continue;
        }
        Train &_train = train.value();
        if (
            locomotiveCapacity.at(_train.locomotive.value().loco_type) > _train.wagons.size()
        ) {
            if (!_train.train_type.has_value()) {
                // Определения типа поезда, берем самое большое кол-во вагонов, без учета пустых
                std::pair<WagonType, int> huge_wagon_type{
                    wagonPriorityToSend[0], free_wagons_[wagonPriorityToSend[0]].size()
                };
                for (const WagonType wagon_type : wagonPriorityToSend) {
                    if (free_wagons_[wagon_type].size() > huge_wagon_type.second) {
                        huge_wagon_type.first = wagon_type;
                        huge_wagon_type.second = free_wagons_[wagon_type].size();
                    }
                }
                _train.train_type = wagonTrainMapper.at(huge_wagon_type.first);
                _train.number = train_amount_;
                train_amount_++;
            }
            // Загрузка вагонов
            LoadTrainTypeWagons(train);
        }
    }
}

std::pair<int, int> SortingHill::GetMostValuablePathWihitTrainToSend() const {
    std::pair<int, int> most_valuable_path{-1,-1};
    for (const auto &[path_num, train] : paths_) {
        if (!train.has_value() || !train.value().locomotive.has_value()) {
            continue;
        }
        const Train &_train = train.value();
        // Ищем минимальную разницу между максимальной и текущейзагрузкой поезда
        // Не считаю что тут надо менять на find, это статический мапы с типами.
        int diff_load = 
            locomotiveCapacity.at(_train.locomotive.value().loco_type) - _train.wagons.size();
        if (diff_load == 0) {
            return {path_num, 0};
        }
        if (diff_load < most_valuable_path.second || most_valuable_path.second == -1) {
            most_valuable_path.second = diff_load;
            most_valuable_path.first = path_num;
        }
        else if (diff_load == most_valuable_path.second) {
            // Сравнения по приоритетам
            if (
                _train.train_type.has_value()
                &&
                // Не считаю что тут надо менять на find, это статический мапы с типами.
                trainPriorityToSend.at(_train.train_type.value()) 
                < trainPriorityToSend.at(
                    paths_.at(most_valuable_path.first).value().train_type.value()
                )
            ) {
                most_valuable_path.first = path_num;
            }
        }
    }
    return most_valuable_path;
}

/* 
* Обработчики от Handler-ов
*/
void SortingHill::SetupPath() {
    int first_free_path_num = GetFirstFreePath();
    if (first_free_path_num == -1) {
        std::cout << "Нет свободного пути" << std::endl;
        return;
    }
    train_index_ = first_free_path_num;
    paths_[train_index_] = Train{};
    changed_commited_ = true;
    std::cout << "Путь " << train_index_ << " подготовлен" << std::endl;
}

void SortingHill::SetupLocomotiveToPath() {
    if (free_locomotives_.empty()) {
        std::cout << "Нет свободного локоматива" << std::endl;
        return;
    }
    int first_setup_whitout_train_path_num = GetFirstSetupedPathWithoutTrain();
    if (first_setup_whitout_train_path_num == -1) {
        std::cout << "Нет подходящшего пути" << std::endl;
        return;
    }
    paths_[first_setup_whitout_train_path_num].value().locomotive = free_locomotives_.front();
    free_locomotives_.pop();
    ManageWagons();
    changed_commited_ = true;
    std::cout << "Локомотив " << 
        paths_[first_setup_whitout_train_path_num].value().locomotive.value().loco_type 
        << " установлен на путь " << first_setup_whitout_train_path_num 
    << std::endl;
}

void SortingHill::AddLocomotiveToQueue(const Locomotive &locomotive) {
    free_locomotives_.push(locomotive);
    std::cout 
        << "В отстойник добавлен локомотив " 
        << locomotive.loco_type 
        << " \\ всего локомотивов -> "
        << free_locomotives_.size() 
    << std::endl;
    changed_commited_ = true;
}

void SortingHill::AddWagonToQueue(const Wagon &wagon) {
    free_wagons_[wagon.wagon_type].push_back(wagon);
    std::cout << "В отстойник добавлен вагон " << wagon.number << " | " << wagon.wagon_type
        << " \\ всего выгонов -> "  << free_wagons_
    << std::endl;
    ManageWagons();
    changed_commited_ = true;
}

void SortingHill::SendTrain() {
    std::pair<int, int> train_send_info = GetMostValuablePathWihitTrainToSend();
    if (train_send_info.first == -1) {
        return;
    }
    if (train_send_info.second > 0) {
        // Догрузка состава
        std::optional<Train> &train = paths_.at(train_send_info.first);
        LoadTrainTypeWagons(train);
        // Догрузка пустыми вагонами
        while (
            train.value().wagons.size() < locomotiveCapacity.at(train.value().locomotive.value().loco_type)
            &&
            free_wagons_.at(WagonType::kEmpty).size() > 0
        ) {
            train.value().wagons.push_back(free_wagons_.at(WagonType::kEmpty).back());
            free_wagons_[WagonType::kEmpty].pop_back();
        }
    }
    PrintSendTrain(train_send_info.first);
    last_sent_train_ = paths_.at(train_send_info.first);
    paths_[train_send_info.first].reset();
    changed_commited_ = true;
}

void SortingHill::EndShiftAndSendAllLocomotives() {
    while (
        !free_locomotives_.empty()
        ||
        GetMostValuablePathWihitTrainToSend().first != -1
    ) {
        if (GetMostValuablePathWihitTrainToSend().first != -1) {
            // Отправляем все поезда
            HandleEvent(EventType::kTrainReady);
        } else if (GetFirstSetupedPathWithoutTrain() != -1) {
            // Подготавливаем пути
            HandleEvent(EventType::kTrainPlanned);
        } else if (GetFirstFreePath() != -1) {
            // Загружаем все пути
            HandleEvent(EventType::kPreparePath);
        }
    }
    for (auto &[path_num, train] : paths_) {
        train.reset();
    }
}

/* 
* Helper-ы CheckEvent
*/
bool SortingHill::CheckHasFreePaths() const {
    for (auto [path_num, train] : paths_) {
        if (!train.has_value()) {
            return true;
        }
    }
    return false;
}

bool SortingHill::CheckHasntCurrentPathTrain() const {
    if (paths_.at(train_index_).has_value()) {
        // Если есть поезд, но нет вагонов
        return paths_.at(train_index_).value().wagons.empty();
    }
    // Полностью пустой путь
    return true;
}

bool SortingHill::CheckHasntTrainALocomotive() const {
    if (paths_.at(train_index_).has_value()) {
        // Путь приготовлен, но нет локомотива и вагонов
        return !paths_.at(train_index_).value().locomotive.has_value();
    }
    return true;
}

bool SortingHill::CheckTrainReadyState() const {
    if (!paths_.at(train_index_).has_value()) {
        return false;
    }
    Train current_train = paths_.at(train_index_).value(); 
    if (
        current_train.locomotive.has_value() 
        &&
        locomotiveCapacity.at(current_train.locomotive.value().loco_type)  == current_train.wagons.size()
    ) {
        return true;
    }
    return false;
}

/* Проверка возможности события. */
bool SortingHill::CheckEvent(EventType event) const {
    switch (event) {
        case EventType::kPreparePath: {
            /* Проверить, что есть свободные пути. */
            return CheckHasFreePaths();
        }
        case EventType::kTrainPlanned: {
            /* Проверить, что на назначенных путях нет поездов. */
            return CheckHasntCurrentPathTrain();
        }
        case EventType::kLocoArrived: {
            /* Проверить, что у поезда нет локомотива. */
            return CheckHasntTrainALocomotive();
        }
        case EventType::kTrainReady: {
            /* Проверить, что количество вагонов соответствует локомотиву или буфер вагонов пустой. */
            return CheckTrainReadyState();
        }
        default:
            return true;
    }
}

void SortingHill::HandleEvent(EventType event) {
    changed_commited_ = false;
    switch (event) {
        case EventType::kShiftStarted: {
            for (const auto& handler : handlers_) {
                handler->StartShift(*this);
            }
            break;
        }
        case EventType::kShiftEnded: {
            /* Здесь нужно освободить все пути. Можно отправлять локомотивы, даже без вагонов. */
            EndShiftAndSendAllLocomotives();
            for (const auto& handler : handlers_) {
                handler->EndShift(*this);
            }
            break;
        }
        case EventType::kWagonArrived: {
            if (!IsWagonBuffer()) {
                break;
            }
            const auto& wagon = wagon_buffer_.front();
            OperationInfo operation_info;
            for (const auto& handler : handlers_) {
                handler->HandleWagon(*this, wagon, operation_info);
            }
            PopWagon();
            break;
        }
        case EventType::kLocoArrived: {
            auto loco_type = RandomGen::GetRandomElem<LocoType>(kLocoType);
            Locomotive locomotive{loco_type};
            OperationInfo operation_info;
            for (const auto& handler : handlers_) {
                handler->HandleLocomotive(*this, locomotive, operation_info);
            }
            break;
        }
        case EventType::kTrainPlanned: {
            OperationInfo operation_info;
            for (const auto& handler : handlers_) {
                handler->AllocatePathForTrain(*this, operation_info);
            }
            break;
        }
        case EventType::kTrainReady: {
            OperationInfo operation_info;
            for (const auto& handler : handlers_) {
                handler->SendTrain(*this, operation_info);
            }
            break;
        }
        case EventType::kPreparePath: {
            OperationInfo operation_info;
            for (const auto& handler : handlers_) {
                handler->PreparePath(*this, operation_info);
            }
            break;
        }
        default: {
            using namespace std::literals;
            throw std::out_of_range("Неожиданное событие"s);
        }
    }
}
