//
// Created by nwmqpa on 28/11/19
//

#include "ISystem.hpp"

class DummySystem : public ecs::ISystem<ecs::StateData<string>> {
public:
    [[nodiscard]] ecs::EntityRequest getDependencies() const;
    void operator()(any entities, shared_ptr<ecs::StateData<string>> data);

};