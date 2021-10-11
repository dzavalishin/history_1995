/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	
 *
 *      $Log: MpStat.h $
 *      Revision 1.4  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1996/07/02 01:55:57  dz
 *      writing
 *
 *      Revision 1.2  1996/06/30 22:58:25  dz
 *      update
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef MPSTAT_H
#define MPSTAT_H


// Note that average() method is not a true
// average calculator. Instead it tries to
// calc. average for values, that are not too
// outstanding, in attempt to ignore timings for
// blocks for which acks was lost and resent
// due to Query. Or should we update Frame times
// when sending query too?
// NB - since receiver can generate NAcks on its own,
// Nack timings should never be supplied here.

class MP_Averager
    {
    private:
        int         size_v;
        vector<int> v;
        
    public:
        MP_Averager( int size = 20 ) : size_v(size) {}
        
        void new_value(int);
        int average() const;
    };

// Link statistics. This stuff is not channel dependent.

// ���� �� ���� ����⨪� ����� ����⮢ �⤥�쭮 �� ࠧ�� �� ࠧ��ࠬ

class MP_LinkStat
    {
    private:
        int          ta_time_v;
        MP_Averager  ta_av;
        
        int          pd_rate_v;
        MP_Averager  dp_av;
        
    public:
        
        MP_LinkStat()
            {
            ta_time_v = 0;
            }
            
            
        turnaround_measure( int ); // newly measured turnaround time, sec
        int turnaround_time() const { return ta_time_v;};
    
        packet_drop_measure( int ); // newly measured packet drop rate, %
        int packet_drop_rate() const { return pd_rate_v;};
    };



#endif // MPSTAT_H

