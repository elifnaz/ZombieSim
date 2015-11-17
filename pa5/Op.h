// Defines all the classes for different operations
// which can be performed.
#pragma once
#include "World.h"

struct MachineState;

// Abstract Base Class

class Op
{
public:
    Op(const char* OpName, int parameter)
    : mOpName(OpName)
    , mParam(parameter)
    { }
    
    void DebugOutput(MachineState& state);
    
    virtual void Execute(MachineState& state) = 0;
    
    MachineState* GetTarget(MachineState& state, int n = 1);
    
    int getParam() const {   return mParam;  }
    
    virtual ~Op() { }
protected:
    int mParam;
    const char* mOpName;
    bool canMove(MachineState& state);
    bool frontTile(MachineState &state, MachineState *&frontState, int num);
    void DeleteHelper(MachineState& state);
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
    OpRotate(int parameter)
    : Op("OpRotate", parameter)
    { }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
    OpGoto(int parameter)
    : Op("OpGoto", parameter)
    { }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the attack operation
struct OpAttack : Op
{
    OpAttack()
    : Op("OpAttack", 1)
    { }
    
    virtual void Execute(MachineState& state) override;
};


// Defines the ranged attack operation
struct OpRangedAttack : Op
{
    OpRangedAttack()
    : Op("OpRangedAttack", 2)
    { }
    
    virtual void Execute(MachineState& state) override;
};


// Defines the test wall operation (sets flag to true if facing a wall)
struct OpTestWall : Op
{
    OpTestWall()
    : Op("OpTestWall", 0)
    {
        
    }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the test human operation (sets flag to true if facing a human)
// Parameter determines how many tiles in front (1 or 2)
struct OpTestHuman : Op
{
    OpTestHuman(int parameter)
    : Op("OpTestHuman", parameter)
    {
        
    }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the test zombie operation (sets flag to true if facing a zombie)
// Parameter determines how many tiles in front (1 or 2)
struct OpTestZombie : Op
{
    OpTestZombie(int parameter)
    : Op("OpTestZombie", parameter)
    {
        
    }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the test random operation (sets flag randomly)
struct OpTestRandom : Op
{
    OpTestRandom()
    : Op("OpTestRandom", 0)
    { }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the forward operation
struct OpForward : Op
{
    OpForward()
    : Op("OpForward", 0)
    { }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the jump to line if flag is true operation
// Parameter determines the line number
struct OpJe : Op
{
    OpJe(int parameter)
    : Op("OpJe", parameter)
    { }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the jump to line if flag is false operation
// Parameter determines the line number
struct OpJne : Op
{
    OpJne(int parameter)
    : Op("OpJne", parameter)
    { }
    
    virtual void Execute(MachineState& state) override;
};

struct OpTestPassable : Op
{
    OpTestPassable()
    : Op("OpTestPassable", 0)
    { }
    
    virtual void Execute(MachineState& state) override;
};

// Defines the end turn operation
struct OpEndTurn : Op
{
    OpEndTurn()
    : Op("OpEndTurn", 0)
    { }
    
    virtual void Execute(MachineState& state) override;
};