#include "stage.hpp"
#include <utility>

void Stage::setOnFinished(Stage::FinishedCallback cb){
    onFinished = std::move(cb);
}
