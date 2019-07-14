#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstddef> 
#include <iomanip>
#include <vector>
#include <algorithm>
#include "myclock.h"
#include "elements.h"

void SessionMorning::Show(){
    //cout << "MORNING MEETING:" << endl;
    for (vector<class Item>::iterator it = m_items.begin() ; it != m_items.end(); ++it) {
        m_clk.PrintTime();
        cout<< " " << it->m_name << " " << it->m_cost << "min" <<endl;
        m_clk.AddMin(it->m_cost);
    }
    m_clk.PrintTime();
    cout<< " Networking Event" <<endl;
}

void SessionAfternoon::Show(){
    //cout << "AFTERNOON MEETING:" << endl;
    for (vector<class Item>::iterator it = m_items.begin() ; it != m_items.end(); ++it) {
        m_clk.PrintTime();
        cout<< " " << it->m_name << " " << it->m_cost << "min" <<endl;
        m_clk.AddMin(it->m_cost);
    }
    m_clk.PrintTime();
    cout<< " Networking Event" <<endl;
}

void ItemSets::Show(){
  for (vector<class Item>::iterator it = m_items.begin() ; it != m_items.end(); ++it) {
    cout << "[name]" << it->m_name << ",[cost]" << it->m_cost << ",[is_scheduled]" << it->is_scheduled <<endl;
  }
}

void ItemSets::SortItems(){
    //sort items by time costs
    sort(m_items.begin(),m_items.end());
    reverse(m_items.begin(),m_items.end());
}

bool ItemSets::IsFinish(){
    for (vector<class Item>::iterator it = m_items.begin() ; it != m_items.end(); ++it) {
        if(it->is_scheduled == 0) {
            return false;
        } 
    }
    return true;
}

void TrackSets::Show(){
    int i = 0;
    for (vector<class Track>::iterator it = m_tracks.begin() ; it != m_tracks.end(); ++it) {
        if(i > m_valid_days){
            return;
        }
        cout << "TRACK " << i+1 << endl;
        it->m_morning_session.Show();
        it->m_afternoon_session.Show();
        i++;
    }
}