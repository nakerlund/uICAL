/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "uICAL/cppstl.h"
#include "uICAL/types.h"
#include "uICAL/error.h"
#include "uICAL/util.h"
#include "uICAL/logging.h"
#include "uICAL/vline.h"
#include "uICAL/vlinestream.h"

namespace uICAL {
    VLineStream::VLineStream(istream& ical)
    : ical(ical)
    , currentLine(nullptr)
    , repeat(false)
    {}

    void VLineStream::repeatLine() {
        this->repeat = true;
    }

    const VLine_ptr VLineStream::next() {
        if (this->repeat && this->currentLine != nullptr) {
            this->repeat = false;
            return this->currentLine;
        }

        string line;
        string completeLine;
        bool hasLine = false;

        if (line.readfrom(this->ical, '\n')) {
            line.rtrim();
            completeLine = line;
            hasLine = true;
        }

        if (hasLine) {
            int peek_val = this->ical.peek();
            int loop_count = 0;
            while (peek_val != std::char_traits<char>::eof()) {
                char nextChar = (char)peek_val;

                if (!std::isspace(static_cast<unsigned char>(nextChar))) {
                    break;
                }

                string nextLine;
                if (nextLine.readfrom(this->ical, '\n')) {
                    nextLine.rtrim();
                    completeLine += nextLine.substr(1);
                }
                loop_count++;
                if (loop_count > 1000) {
                    break;
                }
                peek_val = this->ical.peek();
            }

            this->currentLine = new_ptr<VLine>(completeLine);
        } else {
            this->currentLine = nullptr;
        }

        return this->currentLine;
    }
}
