//
//  World.cpp
//  zombiewx-mac
//
//  Created by Elif Naz Aydin on 11/4/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "World.h"

void World::addZombie(MachineState* zombie)
{
    
    mZombies.push_back(zombie);

}
void World::addHuman(MachineState* human)
{
    
    mHumans.push_back(human);
}


void World::setDrawPanel(ZomDrawPanel *panel)
{
    mPanel = panel;
    mPanel->PaintNow();
}

void World::updatePanel()
{
    mPanel->PaintNow();
}