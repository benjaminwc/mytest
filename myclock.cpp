#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstddef> 
#include <iomanip>
#include "myclock.h"

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
