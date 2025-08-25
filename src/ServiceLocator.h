#pragma once

//
// Created by Mike Mitterer on 2019-01-04.
//

//#ifndef CATDOOR2_SERVICELOCATOR_H
//#define CATDOOR2_SERVICELOCATOR_H

#include "CatDoor.h"

namespace mm {

    class ServiceLocator {
    private:
        static std::shared_ptr<Application> applicationService;
        static std::shared_ptr<CatDoor> catDoorService;

    public:
        static std::shared_ptr<CatDoor> provide(std::shared_ptr<CatDoor> service) {
            catDoorService = service;
            return catDoorService;
        }

        static std::shared_ptr<Application> provide(std::shared_ptr<Application> service) {
            applicationService = service;
            return applicationService;
        }

        static std::shared_ptr<CatDoor> getCatDoor() { return catDoorService; }
        static std::shared_ptr<Application> getApplication() { return applicationService; }
    };

}

//#endif //CATDOOR2_SERVICELOCATOR_H
