//
//  World.h
//  zombiewx-mac
//
//  Created by Elif Naz Aydin on 11/4/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "Singleton.h"
#include "Traits.h"
#include "ZomDrawPanel.h"
#include <map>
#include <utility>


class MachineState;

class World : public Singleton<World> 
{
    DECLARE_SINGLETON(World);

public:

    void addZombie(MachineState* zombie);
    void addHuman(MachineState* human);
    
    std::vector<MachineState*>* getHumans() { return &mHumans;}
    std::vector<MachineState*>* getZombies() { return &mZombies;}
    std::vector<MachineState*>* getToDelete() { return &mToDelete;}

    void setDrawPanel(ZomDrawPanel *panel);
    //calls paintNow on the panel
    void updatePanel();


    std::map<std::pair<int, int>, MachineState*> grid; // maps taken coordinates and states

private:

    World() {    }
    ZomDrawPanel *mPanel;
    
    std::vector<MachineState*> mHumans; // list of all humans
    std::vector<MachineState*> mZombies; // list of all zombies
    std::vector<MachineState*> mToDelete; // humans to delete after the turn

    
};