#pragma once

#include "handler_interface.h"
#include "enums.h"
#include "common.h"

#include <memory>
#include <optional>
#include <queue>
#include <vector>
#include <map>

class SortingHillStats {
public:
    size_t GetPathSetuped() const;
    size_t GetTrinesPlaned() const;
    std::map<LocoType, size_t> GetLocomotivesArrived() const;
    std::map<WagonType, size_t> GetWagonStats() const;
    std::map<TrainType, size_t> GetTrainsSentAmount() const;

    void incPath();
    void incTrainsPlaned();
    void incLoco(const Locomotive &locomotive);
    void incWagon(const Wagon& wagon);
    void incTrainsSent(const TrainType train_type);

private:
    size_t path_setuped_ = 0;
    size_t trians_planed_ = 0;
    std::map<WagonType, size_t> wagons_stats_{
        {WagonType::kDanger, 0},
        {WagonType::kFreight, 0},
        {WagonType::kPass, 0},
        {WagonType::kEmpty, 0},
    };
    std::map<TrainType, size_t> trains_sent_{
        {TrainType::kDanger, 0},
        {TrainType::kFreight, 0},
        {TrainType::kPass, 0},
    };
    std::map<LocoType, size_t> locomotives_arrived_{
        {LocoType::kDiesel24, 0},
        {LocoType::kDiesel64, 0},
        {LocoType::kElectro16, 0},
        {LocoType::kElectro32, 0},
    };
};

class SortingHill {
public:
    explicit SortingHill(size_t number_of_paths,
                         std::vector<std::unique_ptr<SortingHandler>> handlers);

    void AddWagon(Wagon wagon);
    bool IsWagonBuffer() const;
    size_t GetNumberOfPaths() const;
    size_t GetNumberOfWagBuffer() const;
    std::map<WagonType, std::vector<Wagon>> GetWagQueue() const;
    SortingHillStats GetStats() const;
    bool IsChangedCommited() const;
    bool CheckEvent(EventType event) const;
    void HandleEvent(EventType event);

    // Helper-ы CheckEvent
    bool CheckHasFreePaths() const;
    bool CheckHasntCurrentPathTrain() const;
    bool CheckHasntTrainALocomotive() const;
    bool CheckTrainReadyState() const;

    // Обработчики от Handler-ов
    void SetupPath();
    void SetupLocomotiveToPath();
    void AddLocomotiveToQueue(const Locomotive locomotive);
    void AddWagonToQueue(const Wagon wagon);
    void SendTrain();
    void EndShiftAndSendAllLocomotives();

    // stats
    void incPath();
    void incTrainsPlaned();
    void incLoco(const Locomotive &locomotive);
    void incWagon(const Wagon& wagon);
    void incTrainsSent();

private:
    std::vector<std::unique_ptr<SortingHandler>> handlers_;
    const size_t number_of_paths_;
    std::queue<Wagon> wagon_buffer_;
    int train_index_ = 1;
    int train_amount_ = 1;

    // Стоящие в очереди после выдачи <вместимость, ждущие локомотивы>
    std::queue<Locomotive> free_locomotives_;
    // Стоящие в очереди после выдачи <тип вагона, ждущие вагоны>
    std::map<WagonType, std::vector<Wagon>> free_wagons_;
    // Пути <номер, поезд (локомотив и вагоны)>
    std::map<int, std::optional<Train>> paths_;

    SortingHillStats stats_{};
    // Флаг для обновления статистики, после реальных изменений
    bool changed_commited_ = false;
    // Последний отправленный поезд, для подсчета статистики
    std::optional<Train> last_sent_train_;

private:
    void PopWagon();

    // Prints
    void printSendTrain(const int path_index) const;

    // Вспомогательные методы
    int GetFirstFreePath() const;
    int GetFirstSetupedPathWithoutTrain() const;
    void ManageWagons();
    std::pair<int, int> GetMostValuablePathWihitTrainToSend() const;
    void LoadTrainTypeWagons(std::optional<Train> &train);
};
