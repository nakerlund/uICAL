/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_calendar_h
#define uical_calendar_h

#include "uICAL/base.h"

namespace uICAL {
    // Initial capacity for event vector to reduce reallocations on large calendars.
    // Can be overridden at compile time: -DUICAL_CALENDAR_INITIAL_CAPACITY=500
    #ifndef UICAL_CALENDAR_INITIAL_CAPACITY
    #define UICAL_CALENDAR_INITIAL_CAPACITY 16000
    #endif

    class Calendar : public Base {
        public:
            Calendar();

            using eventP_t = std::function<bool (const VEvent&)>;

            static Calendar_ptr load(istream& ical);
            static Calendar_ptr load(istream& ical, eventP_t addEvent);
            static Calendar_ptr load(istream& ical, TZMap_ptr& tzmap);
            static Calendar_ptr load(istream& ical, TZMap_ptr& tzmap, eventP_t addEvent);

            void addEvent(const VEvent_ptr &);

            void str(ostream& out) const;

            friend class CalendarIter;
        protected:
            using events_t = std::vector<VEvent_ptr>;
            events_t events;
    };
}
#endif
