//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include "World.h"
#include <map>
#include <vector>
#include <utility>

enum
{
    ID_SImSTART=1000,
    ID_TURN_TIMER,
    ID_LOAD_ZOMBIE,
    ID_LOAD_SURVIVOR,
    ID_RANDOMIZE,
    ID_RESET
    
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
EVT_MENU(wxID_NEW, ZomFrame::OnNew)
EVT_MENU(ID_SImSTART, ZomFrame::OnSimStart)
EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
EVT_MENU(ID_RESET, ZomFrame::OnReset)

wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, mIsActive(false)
{
    // File menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_NEW);
    menuFile->Append(wxID_EXIT);
    
    // Simulation menu
    mSimMenu = new wxMenu;
    mSimMenu->Append(ID_SImSTART, "Start/stop\tSpace", "Start or stop the simulation");
    mSimMenu->Append(ID_LOAD_ZOMBIE, "Load Zombie...", "Load zombies to the world");
    mSimMenu->Append(ID_LOAD_SURVIVOR, "Load Survivor...", "Load survivors to the world");
    mSimMenu->Append(ID_RANDOMIZE, "Randomize", "Generate random zombies");
    mSimMenu->Append(ID_RESET, "Reset", "Reset the simulation");
    
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(mSimMenu, "&Simulation");
    SetMenuBar(menuBar);
    CreateStatusBar();
    
    // Prepare the draw panel and show this frame
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    mPanel = new ZomDrawPanel(this);
    sizer->Add(mPanel, 1, wxEXPAND);
    
    SetSizer(sizer);
    SetAutoLayout(true);
    Show(true);
    
    mSimMenu->Enable(ID_SImSTART, false);
    mSimMenu->Enable(ID_RESET, false);
    mSimMenu->Enable(ID_RANDOMIZE, false);
    mZombieLoaded = false;
    mHumanLoaded = false;
    
    mTurnTimer = new wxTimer(this, ID_TURN_TIMER);
    
}

ZomFrame::~ZomFrame()
{
    delete mTurnTimer;
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
    mTurnTimer->Stop();
    mPanel->mMonth = 0;
    World::get().getHumans()->clear();
    World::get().getZombies()->clear();
    
    mSimMenu->Enable(ID_SImSTART, false);
    mSimMenu->Enable(ID_RESET, false);
    mSimMenu->Enable(ID_RANDOMIZE, false);
    mHumanLoaded = false;
    mZombieLoaded = false;
    
    mPanel->PaintNow();
    
}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
    if (!mIsActive)
    {
        // Add timer to run once per second
        mTurnTimer->Start(1000);
        mIsActive = true;
    }
    else
    {
        mTurnTimer->Stop();
        mIsActive = false;
    }
}
void ZomFrame::OnLoadZombie(wxCommandEvent& event)
{
    
    try
    {
        wxFileDialog openFileDialog(this, _("Open a ZOM file"), "", "./zom",
                                    "ZOM Files|*.zom", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
        if (openFileDialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }
        std::string fileName = openFileDialog.GetPath().ToStdString();
        zombieMachine.LoadMachine(fileName);
        mPanel->mZombieProgram = fileName.substr(fileName.find_last_of('/') + 1);
        World::get().setDrawPanel(mPanel);
        
        if (mHumanLoaded)
        {
            mSimMenu->Enable(ID_SImSTART, true);
            mSimMenu->Enable(ID_RESET, true);
            mSimMenu->Enable(ID_RANDOMIZE, true);
        }
        mZombieLoaded = true;
        mPanel->PaintNow();
        
    }
    catch (FileLoadExcept exception)
    {
        wxMessageBox("ZOM file is invalid", "Error", wxOK | wxICON_ERROR);
    }
    
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{
    
    try
    {
        wxFileDialog openFileDialog(this, _("Open a ZOM file"), "", "./zom",
                                    "ZOM Files|*.zom", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
        if (openFileDialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }
        std::string fileName = openFileDialog.GetPath().ToStdString();
        humanMachine.LoadMachine(fileName);
        mPanel->mHumanProgram = fileName.substr(fileName.find_last_of('/') + 1);
        World::get().setDrawPanel(mPanel);
        
        if (mZombieLoaded)
        {
            mSimMenu->Enable(ID_SImSTART, true);
            mSimMenu->Enable(ID_RESET, true);
            mSimMenu->Enable(ID_RANDOMIZE, true);
            
        }
        mHumanLoaded = true;
        mPanel->PaintNow();
        
        
    }
    catch (FileLoadExcept exception)
    {
        wxMessageBox("ZOM file is invalid", "Error", wxOK | wxICON_ERROR);
    }
    
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
    
    wxString winner;
    if (World::get().getZombies()->empty())
    {
        winner = "Humans won!!!";
        
    }
    else if (World::get().getHumans()->empty())
    {
        winner = "Zombies won!!!";
    }
    
    else{
        auto iter = World::get().getZombies()->begin();
        for (; iter != World::get().getZombies()->end(); iter++)
        {
            
            zombieMachine.TakeTurn(**iter);
            
        }
        
        
        iter = World::get().getHumans()->begin();
        for (; iter != World::get().getHumans()->end(); iter++)
        {
            humanMachine.TakeTurn(**iter);
            
        }
        
        iter = World::get().getToDelete()->begin();
        for (; iter != World::get().getToDelete()->end(); iter++)
        {
            World::get().getHumans()->erase(std::find(World::get().getHumans()->begin(), World::get().getHumans()->end(), *iter));
            
        }
        World::get().getToDelete()->clear();
        
        mPanel->mMonth++;
        World::get().updatePanel();
        return;
        
    }
    
    mTurnTimer->Stop();
    wxMessageBox(winner, "GG");
    
    
    
}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
    
    World::get().getZombies()->clear();
    World::get().getHumans()->clear();
    std::map<int, int> coords;
    World::get().grid.clear();
    std::vector<std::pair<int, int>> coordinates;
    
    srand(static_cast<unsigned int>(time(NULL)));
    
    MachineState *zombie;
    for (int i = 0; i < 20; i++) {
        zombie = new MachineState();
        
        zombie->mX = rand() % 20;
        zombie->mY = rand() % 20;
        
        
        auto coord = std::make_pair(zombie->mX, zombie->mY);
        while (std::find(coordinates.begin(), coordinates.end(), coord) != coordinates.end())
        {
            zombie->mX = rand() % 20;
            zombie->mY = rand() % 20;
            coord = std::make_pair(zombie->mX, zombie->mY);
            
        }
        
        coordinates.push_back(coord);
        
        int facing = rand() % 4;
        
        switch (facing) {
            case 0:
                
                
                zombie->mFacing = MachineState::UP;
                break;
            case 1:
                
                
                zombie->mFacing = MachineState::DOWN;
                break;
            case 2:
                
                
                zombie->mFacing = MachineState::LEFT;
                break;
            case 3:
                
                
                zombie->mFacing = MachineState::RIGHT;
                break;
        }
        zombieMachine.BindState(*zombie);
        World::get().addZombie(zombie);
        World::get().grid[coord] = zombie;
        
    }
    
    MachineState *human;
    for (int i = 0; i < 10; i++) {
        human = new MachineState();
        
        human->mX = rand() % 20;
        human->mY = rand() % 20;
        
        auto coord = std::make_pair(human->mX, human->mY);
        while (std::find(coordinates.begin(), coordinates.end(), coord) != coordinates.end())
        {
            human->mX = rand() % 20;
            human->mY = rand() % 20;
            coord = std::make_pair(human->mX, human->mY);
            
        }
        
        coordinates.push_back(coord);
        
        int facing = rand() % 4;
        
        switch (facing) {
            case 0:
                
                
                human->mFacing = MachineState::UP;
                break;
            case 1:
                
                human->mFacing = MachineState::DOWN;
                break;
            case 2:
                
                human->mFacing = MachineState::LEFT;
                break;
            case 3:
                
                
                human->mFacing = MachineState::RIGHT;
                break;
        }
        humanMachine.BindState(*human);
        World::get().addHuman(human);
        World::get().grid[coord] = human;
        
    }
    
    mPanel->PaintNow();
    
}
void ZomFrame::OnReset(wxCommandEvent& event)
{
    mTurnTimer->Stop();
    mPanel->mMonth = 0;
    World::get().getHumans()->clear();
    World::get().getZombies()->clear();
    mPanel->PaintNow();
    
}