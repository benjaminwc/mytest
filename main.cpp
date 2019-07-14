// Example program
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

using namespace std;

/*
 *  extract topic name & time cost from input txt file
 */
void LoadTxtFile(ItemSets &items)
{
    string str;
    string name;
    string cost;
    int cost_min = 0;
    ifstream in("test_input.txt");
    if (in.is_open()) {
      while (!in.eof()) {
        getline(in,str);
        //cout << str << endl;
        size_t found = str.find_last_of(" ");
        name = str.substr(0,found);
        cost = str.substr(found+1);
        //change "lightning" to 5min
        if (cost == "lightning") {
          cost = "5min";
        }
        string tmp = cost.substr(0,cost.find_last_of("m"));
        //cout << " tmp: " << tmp << endl;
        cost_min = atoi(tmp.c_str());
        //cout << " name: " << name << endl;
        //cout << " cost: " << cost_min << endl;
        class Item tmpItem;
        tmpItem.m_name = name;
        tmpItem.m_cost = cost_min;
        tmpItem.is_scheduled = false;
        items.m_totals++;
        items.m_items.push_back(tmpItem);
      }
    }
    else {
      cout << "can't open txt file!" << endl;
    }
    return;
}

/*
 *  schedule items to tracks
 */
#define SCHEDULE_SECCESS 0
#define SCHEDULE_FAILED  1
#define MAX_DAYS         5


bool InsertItemEnough(class Track &track,int cost){
    if(track.m_morning_session.m_remains >= cost || track.m_afternoon_session.m_remains >= cost) {
        return true;
    }
    return false;
}

void InsertItem(class TrackSets &tracks, int day ,class Item item){
    if(tracks.m_tracks[day].m_morning_session.m_remains >= tracks.m_tracks[day].m_afternoon_session.m_remains) { 
        //INSERT TO MORNING 
        tracks.m_tracks[day].m_morning_session.m_items.push_back(item);
        tracks.m_tracks[day].m_morning_session.m_remains -= item.m_cost;
    }
    else {
        //INSERT TO AFTERNOON
        tracks.m_tracks[day].m_afternoon_session.m_items.push_back(item);
        tracks.m_tracks[day].m_afternoon_session.m_remains -= item.m_cost;
    }
}

int ScheduleItems(class ItemSets &items ,class TrackSets &tracks){
    int scheduled_days = 0;
    while((scheduled_days < MAX_DAYS) && (!items.IsFinish())) {
        for (vector<class Item>::iterator it = items.m_items.begin() ; it != items.m_items.end(); ++it) {
            if(it->is_scheduled == 1){
                continue;
            }
            if(InsertItemEnough(tracks.m_tracks[scheduled_days],it->m_cost)) {
                InsertItem(tracks,scheduled_days,(*it));
                it->is_scheduled = 1;
            }
        }
        if(items.IsFinish()) {
            return SCHEDULE_SECCESS;
        }
        else {
            tracks.m_valid_days ++;//need add new day 
            scheduled_days ++;
        }
    }
    return SCHEDULE_FAILED;
}

/*
 *  basic elements  
 * 
 * 
       TrackSets                                                     +----------------+
                                                                     |  topic_name    |
        +----------------+                         +---------+       +----------------+
      +----------------+ |                         |  item   +<------+  time_cost     |
    +----------------+ | |                         +---------+       +----------------+
  +----------------+ | | |                         |  item   |       |  is_scheduled  |
+----------------| | | | |                         +---------+       |----------------+
|----------------| | | | |                         |  ...    |
||    remains   || | | | |                         +---------+
|| +----------+ || | | | |         +---------+     |         |
|| |   item   | || | | | | <-------+  item   |     +---------+
|| +----------+ || | | | |         |---------+     |         |
|| |          | || | | | |                         +---------+
|| +------Items || | | | |                         |         |
|------S_Morning|| | | | |                         |         |
|----------------| | | | |                         |         |
||    remains   || | | | |         +---------+     |         |
|| +----------+ || | | | | <-------+  item   |     |         |
|| |          | || | | | |         |---------+     |         |
|| +----------+ || | | | 4                         |         |
|| |          | || | | 3                           |         |
|| +------Items || | 2                             |         |
||              || 1                               |         |
|-----S_Afternoon|                                 +----ItemSets
+-----------Track0
 *
 *  function main
 */

int main()
{
    class TrackSets tracks;
    class ItemSets items;
    int ret;
    LoadTxtFile(items);
    items.SortItems();
    //items.Show();

    for(int i = 0;i < MAX_DAYS; ++i) {
        class Track tck;
        tck.m_scheduled = false;
        tck.m_morning_session.m_remains = 180;//from 9:00 ~ 12:00
        tck.m_morning_session.m_clk.Reset(9,00,0);
        tck.m_afternoon_session.m_remains = 240;//from 13:00 ~ 17:00
        tck.m_afternoon_session.m_clk.Reset(1,00,1);
        tracks.m_tracks.push_back(tck);
    }
    tracks.m_valid_days = 0;

    ret = ScheduleItems(items,tracks);
    if(ret != 0){
        cout << "SCHEDULE FAILED!" << endl;
        return ret;
    }
    tracks.Show();
    return ret;
}


