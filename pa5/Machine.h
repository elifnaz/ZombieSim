#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Op.h"
#include "Exceptions.h"
#include <wx/msgdlg.h>
#include <fstream>

// Defines state data accessible by the machine and ops


struct MachineState
{
    // Friend Machine so BindState works
    template <typename MachineTraits>
    friend class Machine;
    
    enum Facing { UP, RIGHT, DOWN, LEFT };
    MachineState()
    : mProgramCounter(1)
    , mActionsTaken(0)
    , mFacing(UP)
    , mTest(false)
    , mX(0)
    , mY(0)
    , mTotalOps(0)
    { }
    
    ~MachineState()
    {
    }
    
    // Active line number in behavior program
    int mProgramCounter;
    // Number of actions taken this turn
    int mActionsTaken;
    // Current facing of this character
    Facing mFacing;
    // Test flag for branches
    bool mTest;
    //coordinate info
    int mX;
    int mY;
    
    // setter for  number of ops (used to check if goto is out of range)
    void setTotalOps(int num) noexcept {  mTotalOps = num;}
    // getter for number of ops
    int getTotalOps() const noexcept {  return mTotalOps;}
    
    int GetActionsPerTurn() const noexcept { return mActionsPerTurn; }
    bool GetInfect() const noexcept { return mInfectOnAttack; }
    
private:
    // Data which is set by the traits
    int mActionsPerTurn;
    bool mInfectOnAttack;
    //size of mOps of machine
    int mTotalOps;
    
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
    // Load in all the ops for this machine from the specified file
    void LoadMachine(const std::string& filename);
    
    // Given the state, binds the trait parameters to it
    void BindState(MachineState& state);
    
    // Take a turn using this logic for the passed in state
    void TakeTurn(MachineState& state);
    
    // Destructor
    ~Machine();
private:
    std::vector<Op*> mOps;
};

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
    std::ifstream file (filename);
    if (file.is_open())
    {
        std::string s; // store the comment
        std::string op; //store the op
        int param = 0;
        std::size_t paramDelimIndex; // index of ','
        std::size_t commentDelimIndex; // index of ';'
        
        mOps.clear();
        
        while (!file.eof())
        {
            
            
            std::getline(file, op);
            commentDelimIndex = op.find(";");
            paramDelimIndex = op.find(",");
            
            if (paramDelimIndex != std::string::npos)
            {
                
                param = std::stoi(op.substr(paramDelimIndex + 1));
                op.erase(paramDelimIndex, op.length() - paramDelimIndex);
                
            }
            
            else if (commentDelimIndex != std::string::npos)
            {
                
                op.erase(commentDelimIndex, op.length() - commentDelimIndex);
                
            }
            
            op.erase(remove_if(op.begin(), op.end(), isspace), op.end());
            
            
            if (op == "test_wall")
            {
                mOps.push_back(new OpTestWall());
                
                
            }
            else if (op == "test_human")
            {
                mOps.push_back(new OpTestHuman(param));
            }
            else if (op == "test_zombie")
            {
                mOps.push_back(new OpTestZombie(param));
            }
            else if (op == "test_random")
            {
                mOps.push_back(new OpTestRandom());
            }
            else if (op == "test_passable")
            {
                mOps.push_back(new OpTestPassable());
            }
            else if (op == "je")
            {
                mOps.push_back(new OpJe(param));
            }
            else if (op == "jne")
            {
                mOps.push_back(new OpJne(param));
            }
            else if (op == "forward")
            {
                mOps.push_back(new OpForward());
            }
            else if (op == "goto")
            {
                mOps.push_back(new OpGoto(param));
            }
            else if (op == "rotate")
            {
                mOps.push_back(new OpRotate(param));
            }
            else if (op == "attack")
            {
                mOps.push_back(new OpAttack());
            }
            else if (op == "ranged_attack")
            {
                mOps.push_back(new OpRangedAttack());
            }
            else if (op == "end_turn")
            {
                mOps.push_back(new OpEndTurn());
            }
            
        }
        
        file.close();
        
    }
    
    
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
    state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
    state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
    state.setTotalOps(static_cast<int>(mOps.size()));
    
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
    state.mActionsTaken = 0;
    std::cout<<std::endl;
    while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
    {
        if(!state.GetInfect())
        std::cout << "TAKING TURN" << std::endl;

        try
        {
            mOps.at(state.mProgramCounter - 1)->Execute(state);
        }
        catch(RangeAttackExcept& e)
        {
            wxMessageBox("Zombies can't range attack!", "Error", wxOK | wxICON_ERROR);
        }
        catch(GotoExcept& e)
        {
            std::string error = "Line #";
            error += mOps.at(state.mProgramCounter - 1)->getParam();
            error += " is invalid!";
            wxMessageBox(error, "Error", wxOK | wxICON_ERROR);
        }
        catch(InvalidOp& e)
        {
            wxMessageBox("Invalid OP code", "Error", wxOK | wxICON_ERROR);
        }
        
    }
    
}

template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
    for (auto op : mOps)
    {
        delete op;
    }
    
    mOps.clear();
}