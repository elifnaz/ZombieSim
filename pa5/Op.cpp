#include "Op.h"
#include "Machine.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <utility>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
    //std::cout << state.mProgramCounter << ":" << mOpName << "," << mParam <<" X: "<<state.mX<< " Y: "<<state.mY<<" "<<mOpName<<std::endl<<std::endl;
}

void OpRotate::Execute(MachineState& state)
{
    DebugOutput(state);
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if (mParam == 0)
            {
                state.mFacing = MachineState::RIGHT;
            }
            else
            {
                state.mFacing = MachineState::LEFT;
            }
            break;
        case (MachineState::RIGHT) :
            if (mParam == 0)
            {
                state.mFacing = MachineState::DOWN;
            }
            else
            {
                state.mFacing = MachineState::UP;
            }
            break;
        case (MachineState::DOWN) :
            if (mParam == 0)
            {
                state.mFacing = MachineState::LEFT;
            }
            else
            {
                state.mFacing = MachineState::RIGHT;
            }
            break;
        default:
        case (MachineState::LEFT) :
            if (mParam == 0)
            {
                state.mFacing = MachineState::UP;
            }
            else
            {
                state.mFacing = MachineState::DOWN;
            }
            break;
    }
    
    /*std::cout << "Now facing ";
     switch (state.mFacing)
     {
     case (MachineState::UP):
     std::cout << "UP";
     break;
     case (MachineState::RIGHT):
     std::cout << "RIGHT";
     break;
     case (MachineState::DOWN):
     std::cout << "DOWN";
     break;
     case (MachineState::LEFT):
     std::cout << "LEFT";
     break;
     }
     std::cout << std::endl;*/
    
    state.mProgramCounter++;
    state.mActionsTaken++;
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
    
}

void OpGoto::Execute(MachineState& state)
{
    DebugOutput(state);
    
    if (state.getTotalOps() < mParam)
    {
        throw GotoExcept();
    }
    else
    {
        state.mProgramCounter = mParam;
        
    }
}
void OpTestWall::Execute(MachineState& state)
{
    
    
    DebugOutput(state);
    
    
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if (state.mY == 0)
            {
                state.mTest = true;
            }
            else
            {
                state.mTest = false;
            }
            break;
        case (MachineState::RIGHT) :
            if (state.mX == 19)
            {
                state.mTest = true;
            }
            else
            {
                state.mTest = false;
            }
            break;
        case (MachineState::DOWN) :
            if (state.mY == 19)
            {
                state.mTest = true;
            }
            else
            {
                state.mTest = false;
            }
            break;
        default:
        case (MachineState::LEFT) :
            if (state.mX == 0)
            {
                state.mTest = true;
            }
            else
            {
                state.mTest = false;
            }
            break;
    }
    
    state.mProgramCounter++;
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
}

void OpTestHuman::Execute(MachineState& state)
{
    
    
    DebugOutput(state);
    
    MachineState *testState;
    if (frontTile(state, testState, mParam))
    {
        if (testState->GetInfect())
        {
            state.mTest = false; // facing a zombie, not a human
        }
        else
        {
            state.mTest = true;
            
        }
        
    }
    else
    {
        state.mTest = false; // not facing human nor zombie
    }
    
    
    state.mProgramCounter++;
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
}


void OpTestZombie::Execute(MachineState& state)
{
    
    
    DebugOutput(state);
    
    MachineState *testState;
    
    
    if (frontTile(state, testState, mParam))
    {
        
        if (testState->GetInfect())
        {
            
            
            state.mTest = true; // facing a zombie, not a human
        }
        else
        {
            state.mTest = false; // facing a human, not a zombie
            
        }
    }
    else
    {
        state.mTest = false; // not facing human nor zombie
    }
    
    
    state.mProgramCounter++;
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }}


bool Op::canMove(MachineState& state)
{
    
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if (state.mY == 0 || World::get().grid.find(std::pair<int, int>(state.mX, state.mY - 1)) != World::get().grid.end())
            {
                return false;
            }
            else
            {
                return true;
            }
            break;
        case (MachineState::RIGHT) :
            if (state.mX == 19 || World::get().grid.find(std::pair<int, int>(state.mX + 1, state.mY)) != World::get().grid.end())
            {
                return false;
            }
            else
            {
                return true;
            }
            break;
        case (MachineState::DOWN) :
            if (state.mY == 19 || World::get().grid.find(std::pair<int, int>(state.mX, state.mY + 1)) != World::get().grid.end())
            {
                return false;
            }
            else
            {
                return true;
            }
            break;
        default:
        case (MachineState::LEFT) :
            if (state.mX == 0 || World::get().grid.find(std::pair<int, int>(state.mX - 1, state.mY)) != World::get().grid.end())
            {
                return false;
            }
            else
            {
                return true;
            }
            break;
    }
    
}


void OpTestPassable::Execute(MachineState& state)
{
    DebugOutput(state);
    
    state.mTest = canMove(state);
    state.mProgramCounter++;
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
}


void OpTestRandom::Execute(MachineState& state)
{
    DebugOutput(state);
    srand(static_cast<unsigned int>(time(0)));
    
    state.mTest = rand() % 2 == 1;
    state.mProgramCounter++;
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
}

bool Op::frontTile(MachineState &state, MachineState *&frontState, int num)
{
    
    std::map<std::pair<int, int>, MachineState*>::iterator iter;
    
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if (num == 1)
            {
                iter = World::get().grid.find(std::pair<int, int>(state.mX, state.mY - 1));
                
            }
            else // num == 2
            {
                
                iter = World::get().grid.find(std::pair<int, int>(state.mX, state.mY - 2));
                
            }
            
            if (iter == World::get().grid.end())
            {
                return false;
            }
            else
            {
                frontState = iter->second;
                
                return true;
            }
            break;
        case (MachineState::RIGHT) :
            if (num == 1)
            {
                iter = World::get().grid.find(std::pair<int, int>(state.mX + 1, state.mY));
                
            }
            else // num == 2
            {
                
                iter = World::get().grid.find(std::pair<int, int>(state.mX + 2, state.mY));
                
            }
            
            if (iter == World::get().grid.end())
            {
                return false;
            }
            else
            {
                frontState = iter->second;
                
                return true;
            }
            break;
        case (MachineState::DOWN) :
            if (num == 1)
            {
                iter = World::get().grid.find(std::pair<int, int>(state.mX, state.mY + 1));
                
            }
            else // num == 2
            {
                
                iter = World::get().grid.find(std::pair<int, int>(state.mX, state.mY + 2));
                
            }
            
            if (iter == World::get().grid.end())
            {
                return false;
            }
            else
            {
                frontState = iter->second;
                return true;
            }
            
            break;
        default:
        case (MachineState::LEFT) :
            if (num == 1)
            {
                iter = World::get().grid.find(std::pair<int, int>(state.mX - 1, state.mY));
                
            }
            else // num == 2
            {
                
                iter = World::get().grid.find(std::pair<int, int>(state.mX - 2, state.mY));
                
            }
            
            if (iter == World::get().grid.end())
            {
                return false;
            }
            else
            {
                frontState = iter->second;
                
                return true;
            }
            break;
    }
    
    
    
}

void OpAttack::Execute(MachineState& state)
{
    DebugOutput(state);
    
    if (state.mTest )
    {
        MachineState * stateToAttack;
        if (frontTile(state, stateToAttack, mParam))
        {
            
            if (state.GetInfect() && !stateToAttack->GetInfect())
            {
                
                Machine<ZombieTraits> zombieMachine;
                zombieMachine.BindState(*stateToAttack);
                
                auto iter = World::get().getHumans()->begin();
                for (; iter != World::get().getHumans()->end(); iter++)
                {
                    
                    if (*iter == stateToAttack)
                    {
                        
                        World::get().getHumans()->erase(iter);
                        break;
                    }
                }
                
                World::get().getZombies()->push_back(stateToAttack);
                
                
            }
            else if (!state.GetInfect())
            {
                
                if (stateToAttack->GetInfect()) // attacking a zombie
                {
                    
                    auto iter = World::get().getZombies()->begin();
                    for (; iter != World::get().getZombies()->end(); iter++)
                    {
                        
                        if (*iter == stateToAttack)
                        {
                            
                            World::get().getZombies()->erase(iter);
                            break;
                        }
                    }
                }
                else
                {
                    
                    auto iter = World::get().getHumans()->begin();
                    for (; iter != World::get().getHumans()->end(); iter++)
                    {
                        
                        if (*iter == stateToAttack)
                        {
                            
                            World::get().getToDelete()->push_back(stateToAttack);
                            break;
                        }
                    }
                }
                
                World::get().grid.erase(World::get().grid.find(std::pair<int, int>(stateToAttack->mX, stateToAttack->mY)));
                
            }
        }
        
    }
    
    state.mProgramCounter++;
    state.mActionsTaken++;
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
}


void OpRangedAttack::Execute(MachineState& state)
{
    DebugOutput(state);
    
    if (state.GetInfect())
    {
        // throws exception if called by a zombie
        throw RangeAttackExcept();
    }
    
    if (state.mTest)
    {
        MachineState * stateToAttack;
        if (frontTile(state, stateToAttack, mParam))
        {
            
            if (!stateToAttack->GetInfect()) // attacking a human
            {
                
                auto iter = World::get().getHumans()->begin();
                for (; iter != World::get().getHumans()->end(); iter++)
                {
                    
                    if (*iter == stateToAttack)
                    {
                        
                        World::get().getToDelete()->push_back(stateToAttack);
                        break;
                    }
                }
                World::get().grid.erase(World::get().grid.find(std::pair<int, int>(stateToAttack->mX, stateToAttack->mY)));
                
            }
            else if (stateToAttack->GetInfect()) // attacking a zombie
            {
                
                auto iter = World::get().getZombies()->begin();
                for (; iter != World::get().getZombies()->end(); iter++)
                {
                    
                    if (*iter == stateToAttack)
                    {
                        
                        World::get().getZombies()->erase(iter);
                        break;
                    }
                }
                World::get().grid.erase(World::get().grid.find(std::pair<int, int>(stateToAttack->mX, stateToAttack->mY)));
                
            }
        }
        
    }
    
    state.mProgramCounter++;
    state.mActionsTaken++;
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
    
}


void OpForward::Execute(MachineState& state)
{
    DebugOutput(state);
    
    if (canMove(state))
    {
        auto  it = World::get().grid.find(std::pair<int, int>(state.mX, state.mY));
        if (it != World::get().grid.end())
        {
            World::get().grid.erase(it);
        }
        
        switch (state.mFacing)
        {
            case (MachineState::UP) :
                state.mY--;
                break;
            case (MachineState::RIGHT) :
                state.mX++;
                break;
            case (MachineState::DOWN) :
                state.mY++;
                break;
            default:
            case (MachineState::LEFT) :
                state.mX--;
                break;
        }
        
        World::get().grid[std::pair<int, int>(state.mX, state.mY)] = &state;
        
    }
    
    state.mProgramCounter++;
    state.mActionsTaken++;
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
}

void OpJe::Execute(MachineState& state)
{
    DebugOutput(state);
    if (state.mTest)
    {
        state.mProgramCounter = mParam; // hey throw exception
        
    }
    else
    {
        state.mProgramCounter++;
        
    }
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
    
}

void OpJne::Execute(MachineState& state)
{
    DebugOutput(state);
    if (state.mTest)
    {
        state.mProgramCounter++;
        
    }
    else
    {
        state.mProgramCounter = mParam; // hey throw exception
        
    }
    
    if (state.mProgramCounter > state.getTotalOps())
    {
        DeleteHelper(state);
        throw InvalidOp();
    }
}


void Op::DeleteHelper(MachineState& state)
{
    auto  it = World::get().grid.find(std::pair<int, int>(state.mX, state.mY));
    if (it != World::get().grid.end())
    {
        World::get().grid.erase(it);
        
        if (state.GetInfect())
        {
            World::get().getZombies()->erase(std::find(World::get().getZombies()->begin(), World::get().getZombies()->end(), it->second));
            
        }
        else
        {
            World::get().getHumans()->erase(std::find(World::get().getHumans()->begin(), World::get().getHumans()->end(), it->second));
        }
        
    }
    
    
}


void OpEndTurn::Execute(MachineState& state)
{
    DebugOutput(state);
    if (state.GetInfect()) // zombie
    {
        state.mActionsTaken = 1;
    }
    else // human
    {
        state.mActionsTaken = 2;
        
    }
    
    state.mProgramCounter++;
    if (state.mProgramCounter > state.getTotalOps())
    {
        throw InvalidOp();
    }
}