/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "uICAL/cppstl.h"
#include "uICAL/types.h"
#include "uICAL/error.h"
#include "uICAL/calendarentry.h"


namespace uICAL {
    CalendarEntry::CalendarEntry(Type type, const string& summary, const string& uid, const DateTime& start) {
        this->_type = type;
        this->_summary = summary;
        this->_start = start;
        this->_uid = uid;
    }

    CalendarEntry::CalendarEntry(Type type, const string& summary, const string& uid, const DateTime& start, const DatePeriod& span) {
        this->_type = type;
        this->_summary = summary;
        this->_uid = uid;
        this->_start = start;
        this->_span = span;
    }

    CalendarEntry::Type CalendarEntry::type() const {
        return this->_type;
    }

    string CalendarEntry::asString(Type type) {
        switch(type) {
            case Type::EVENT:
                return "EVENT";
            case Type::NONE:
            default:
                return "UNKNOWN";
        }
    }

    const string CalendarEntry::summary() const {
        return this->_summary;
    }

    const string CalendarEntry::uid() const {
        return this->_uid;
    }

    DateTime CalendarEntry::start() const {
        return this->_start;
    }

    DateTime CalendarEntry::end() const {
        return this->_start + this->_span;
    }

    void CalendarEntry::str(ostream& out) const {
        out << "Calendar " << this->asString(this->_type) << ": " << this->_summary << uICAL::endl;
        out << " - uid: " << this->_uid << uICAL::endl;
        out << " - start: " << this->_start << uICAL::endl;
        out << " - span: " << this->_span << uICAL::endl;
    }
}
