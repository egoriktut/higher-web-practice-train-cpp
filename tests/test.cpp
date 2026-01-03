#include "test_sorting_hill.h"
#include "test_sorting_hill_stats.h"
#include "test_sorting_reporter.h"
#include "test_sorting_operator.h"

int main() {
    run_tests_sorting_hill();
    run_tests_sorting_hill_stats();
    run_tests_sorting_reporter();
    run_tests_sorting_operator();

    return 0;
}