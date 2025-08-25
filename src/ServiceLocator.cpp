//
// Created by Mike Mitterer on 2019-01-04.
//

#include "ServiceLocator.h"

namespace mm {

    std::shared_ptr<Application> ServiceLocator::applicationService = nullptr;
    std::shared_ptr<CatDoor> ServiceLocator::catDoorService = nullptr;

}
