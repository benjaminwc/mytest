#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "myclock.h"

using namespace std;
/*
 *  class Track & Session & Item
 *  public variables need set() get() function TBD
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

class SessionAfternoon {
    public:
        unsigned int m_remains;
        vector<class Item> m_items;
        Clock m_clk;
        void Show();
};

class ItemSets {
    public:
        unsigned int m_totals;
        vector <class Item> m_items;
        void Show();
        void SortItems();
        bool IsFinish();//whether all the items have already been scheduled 
};

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

#endif