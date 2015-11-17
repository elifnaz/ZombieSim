//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"
#include <array>
#include "World.h"


BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
    mMonth = 0;
    SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);

        DrawScore(dc);        
    
}

void ZomDrawPanel::DrawScore(wxDC& dc)
{
    dc.SetTextForeground(*wxRED);
    wxString zombie = "Zombie\nProgram: ";
    zombie.Append(mZombieProgram);
    zombie.Append("\nAlive: ");
    zombie.Append(std::to_string(World::get().getZombies()->size()));
    dc.DrawText(zombie, 620, 10);
    
    dc.SetTextForeground(*wxGREEN);
    wxString human = "Human\nProgram: ";
    human.Append(mHumanProgram);
    human.Append("\nAlive: ");
    human.Append(std::to_string(World::get().getHumans()->size()));
    dc.DrawText(human, 620, 110);

    dc.SetTextForeground(*wxBLACK);
    wxString month = "Month: ";
    month.Append(std::to_string(mMonth));
    dc.DrawText(month, 620, 210);
  
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
    
    for (int i = 40; i < 610; i+= 30)
    {
        dc.DrawLine(i, 10, i, 610);
        dc.DrawLine(10, i, 610, i);

    }
    
    auto iter = World::get().getZombies()->begin();
    
    for (; iter != World::get().getZombies()->end(); iter++)
    {
        wxPoint ptOne(0, 0);
        wxPoint ptTwo(0, 0);
        wxPoint ptThree(0, 0);
        int x = ((*(iter))->mX) * 30; // since each square is 30 x 30
        int y = ((*(iter))->mY) * 30;

        switch ((*(iter))->mFacing)
            {
                case (MachineState::UP) :
                    ptOne = wxPoint(x + 10, y + 40);
                    ptTwo = wxPoint(x + 40, y + 40);
                    ptThree = wxPoint(x + 25, y + 10);
                    break;
                case (MachineState::RIGHT) :
                    ptOne = wxPoint(x + 10, y + 40);
                    ptTwo = wxPoint(x + 10, y + 10);
                    ptThree = wxPoint(x + 40, y + 25);
                    break;
                case (MachineState::DOWN) :
                    ptOne = wxPoint(x + 10, y + 10);
                    ptTwo = wxPoint(x + 40, y + 10);
                    ptThree = wxPoint(x + 25, y + 40);
                    break;
                default:
                case (MachineState::LEFT) :
                    ptOne = wxPoint(x + 10, y + 25);
                    ptTwo = wxPoint(x + 40, y + 10);
                    ptThree = wxPoint(x + 40, y + 40);
                    break;
            }
        
        
        wxPoint *list = new wxPoint[3];
        list[0] = ptOne;
        list[1] = ptTwo;
        list[2] = ptThree;
        
        dc.SetBrush(*wxRED_BRUSH);
        dc.DrawPolygon(3, list);
    }
    
    iter = World::get().getHumans()->begin();
    for (; iter != World::get().getHumans()->end(); iter++)
    {
        wxPoint ptOne(0, 0);
        wxPoint ptTwo(0, 0);
        wxPoint ptThree(0, 0);
        int x = ((*(iter))->mX) * 30; // since each square is 30 x 30
        int y = ((*(iter))->mY) * 30;
        
        switch ((*(iter))->mFacing)
        {
            case (MachineState::UP) :
                ptOne = wxPoint(x + 10, y + 40);
                ptTwo = wxPoint(x + 40, y + 40);
                ptThree = wxPoint(x + 25, y + 10);
                break;
            case (MachineState::RIGHT) :
                ptOne = wxPoint(x + 10, y + 40);
                ptTwo = wxPoint(x + 10, y + 10);
                ptThree = wxPoint(x + 40, y + 25);
                break;
            case (MachineState::DOWN) :
                ptOne = wxPoint(x + 10, y + 10);
                ptTwo = wxPoint(x + 40, y + 10);
                ptThree = wxPoint(x + 25, y + 40);
                break;
            default:
            case (MachineState::LEFT) :
                ptOne = wxPoint(x + 10, y + 25);
                ptTwo = wxPoint(x + 40, y + 10);
                ptThree = wxPoint(x + 40, y + 40);
                break;
        }
        
        
        wxPoint *list = new wxPoint[3];
        list[0] = ptOne;
        list[1] = ptTwo;
        list[2] = ptThree;
        
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.DrawPolygon(3, list);
    }
}