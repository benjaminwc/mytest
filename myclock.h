#ifndef MYCLOCK_H
#define MYCLOCK_H

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

#endif