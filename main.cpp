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

using namespace std;
/*
 *  class Clock,used for time calculation and show
 */
class Clock {
    public:
        void AddMin(unsigned int m_min);
        void ChangeAmPm();
        void PrintTime();
        void Reset(unsigned int h,unsigned int m,unsigned int am);
    private:
        unsigned int m_hour;
        unsigned int m_min;
        unsigned int m_AM_PM;
};

void Clock::Reset(unsigned int h,unsigned int m,unsigned int am) {
  m_hour = h % 12;
  m_min = m % 60;
  m_AM_PM = am % 2;
}

void Clock::ChangeAmPm() {
    if (m_AM_PM == 0) {
        m_AM_PM = 1;
    }
    else {
        m_AM_PM = 0;
    }
    
}

void Clock::AddMin(unsigned int mins) {
    unsigned int add_min = mins + m_min;
    unsigned int add_hour = add_min / 60;
    m_min = add_min % 60;
    unsigned int m_AM_PM_turns = (m_hour + add_hour)/12;
    m_hour = (m_hour + add_hour)%12;
    for(unsigned int i=0;i<m_AM_PM_turns;i++) {
        ChangeAmPm();
    }
}

void Clock::PrintTime() {
    cout << setfill('0') << setw(2) << m_hour << ":" << setfill('0') << setw(2) << m_min << " " << ((m_AM_PM == 1)?"PM":"AM");
}

/*
 *  class Track & Session & Item
 */
class Item {
    public:
        string m_name;
        int    m_cost;
        bool   is_scheduled;
        bool operator <(const Item &other) const
        {
          return m_cost < other.m_cost;
        }
        bool operator >(const Item &other) const
        {   
            return m_cost > other.m_cost;
        }
};

class SessionMorning {
    public:
        unsigned int m_remains;
        vector<class Item> m_items;
        Clock m_clk;
        void Show();
};

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

class SessionAfternoon {
    public:
        unsigned int m_remains;
        vector<class Item> m_items;
        Clock m_clk;
        void Show();
};

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

class ItemSets {
    public:
        unsigned int m_totals;
        vector <class Item> m_items;
        void Show();
        void SortItems();
        bool IsFinish();//whether all the items have already been scheduled 
};

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

class Track {
    public:
        bool m_scheduled;
        class SessionMorning m_morning_session;
        class SessionAfternoon m_afternoon_session;
};

class TrackSets {
    public:
        unsigned int m_valid_days;
        vector <class Track> m_tracks;
        void Show();
};

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


