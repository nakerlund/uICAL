/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "uICAL/cppstl.h"
#include "uICAL/types.h"
#include "uICAL/error.h"
#include "uICAL/util.h"
#include "uICAL/logging.h"
#include "uICAL/calendar.h"
#include "uICAL/calendarentry.h"
#include "uICAL/tzmap.h"
#include "uICAL/vevent.h"
#include "uICAL/vobject.h"
#include "uICAL/vobjectstream.h"
#include "uICAL/vline.h"
#include "uICAL/vlinestream.h"

namespace uICAL {
    Calendar_ptr Calendar::load(istream& ical) {
        TZMap_ptr tzmap = new_ptr<TZMap>();
        return Calendar::load(ical, tzmap, [](const VEvent&) { return true; });
    }

    Calendar_ptr Calendar::load(istream& ical, eventP_t addEvent) {
        TZMap_ptr tzmap = new_ptr<TZMap>();
        return Calendar::load(ical, tzmap, addEvent);
    }

    Calendar_ptr Calendar::load(istream& ical, TZMap_ptr& tzmap) {
        return Calendar::load(ical, tzmap, [](const VEvent&) { return true; });
    }

    Calendar_ptr Calendar::load(istream& ical, TZMap_ptr& tzmap, eventP_t addEvent) {
        log_debug("Calendar::load start");
        VLineStream lines(ical);

        VObjectStream::lineP_t useLine = [](const string parent, const string line) {
            if (parent == "VCALENDAR") {
                if (line.empty()) return true;
            }
            else
            if (parent == "VTIMEZONE") {
                if (line.empty()) return true;
                if (line == "TZID") return true;
            }
            else if (parent == "STANDARD") {
                if (line.empty()) return true;
                if (line == "TZOFFSETFROM") return true;
                if (line == "TZNAME") return true;
            }
            else if (parent == "VEVENT") {
                if (line.empty()) return true;
                if (line == "SUMMARY") return true;
                if (line == "DTSTAMP") return true;
                if (line == "DTSTART") return true;
                if (line == "DTEND") return true;
                if (line == "RRULE") return true;
                if (line == "UID") return true;
            }
            return false;
        };

        VObjectStream stm(lines, useLine);

        {
            log_debug("Calendar::load first object");
            VObject_ptr obj = stm.nextObject(false);
            if (!obj) {
                log_error("Calendar::load first object is null");
                throw ParseError("Parse error, empty stream");
            }
            log_debug("Calendar::load first object name: %s", obj->getName().c_str());

            if (obj->getName() != "VCALENDAR") {
                log_error("Parse error, did not expect: %s", obj->getName().c_str());
                throw ParseError(string("Parse error, did not expect: ") + obj->getName().c_str());
            }
        }

        Calendar_ptr cal = new_ptr<Calendar>();
        size_t event_count = 0;
        size_t tz_count = 0;

        for (;;) {
            auto child = stm.nextObject(true);
            if (child == nullptr) {
                log_debug("Calendar::load loop end (no child)");
                break;
            }

            if (child->getName() == "VTIMEZONE") {
                log_trace("Calendar::load VTIMEZONE");
                tzmap->add(child);
                ++tz_count;
            }
            else if (child->getName() == "VEVENT") {
                log_trace("Calendar::load VEVENT");
                VEvent_ptr event = new_ptr<VEvent>(child, tzmap);
                if (addEvent(*event)) {
                    cal->addEvent(event);
                    ++event_count;
                } else {
                    log_debug("Event ignored: %s @ %s", event->summary.c_str(), event->start.as_str().c_str());
                }
            }
        }
        log_debug("Calendar::load done (events=%u, timezones=%u)",
                  (unsigned)event_count,
                  (unsigned)tz_count);
        log_debug("Calendar::load return");
        return cal;
    }

    Calendar::Calendar() {}

    void Calendar::addEvent(const VEvent_ptr& event) {
        this->events.push_back(event);
    }

    void Calendar::str(ostream& out) const {
        out << "CALENDAR" << uICAL::endl;
    }
}
