// TODO: VALIDATION!!

#include "SentMessage.h"

SentMessage::SentMessage(){
    strcpy (this->recipient, "NOID_NOID");
    memset (this->message, 0, 5000);
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

SentMessage::SentMessage(std::string recipient, std::string message, size_t timeStamp){
    strcpy (this->recipient, recipient.c_str());
    memset (this->message, 0, 5000);
    strcpy (this->message, message.c_str());
    this->timeStamp = timeStamp;
}

SentMessage::~SentMessage() {}

// Add a pretty print overload
std::ostream& operator << (std::ostream& outs, const SentMessage& message){
    SentMessage tempMessage = message; // Temporary variable to circumvent "const" restrictions.
    std::time_t t = static_cast<std::time_t>(tempMessage.timeStamp);
    outs << "--\nTO: " << tempMessage.recipient
         << "\n--\nPAYLOAD:\n" << tempMessage.message
         << "\n--\nTIMESTAMP: " << std::put_time(std::gmtime(&t), "%d.%m.%y|%H:%M:%S"  )
    << std::endl;
    return outs;
}

// std::time_t today_time = std::chrono::system_clock::to_time_t(p1);
//     std::cout << "today: " << std::ctime(&today_time);

// %a
// abbreviated weekday name (Sun, Mon, etc.)
// %A
// full weekday name (Sunday, Monday, etc.)
// %b
// abbreviated month name (Jan, Feb, etc.)
// %B
// full month name (January, February, etc.)
// %c
// full date and time string
// %d
// day of the month as two-digit decimal integer (01-31)
// %D
// date as %m/%d/%y (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %e
// day of month (1-31); single digits are preceded by a blank (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %h
// same as %b (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %H
// hour as two-digit 24-hour clock decimal integer (00-23)
// %I
// hour as two-digit 12-hour clock decimal integer (01-12)
// %j
// day of the year as three-digit decimal integer (001-366)
// %k
// hour (0-23); single digits are preceded by a blank (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %l
// hour (0-12); single digits are preceded by a blank (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %m
// month as a two-digit decimal integer (01-12)
// %M
// minute as a two-digit decimal integer (00-59)
// %n
// new-line character (\n) (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %p
// either "AM" or "PM"
// %r
// time expressed as %I:%M:%S %p (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %R
// time expressed as %H:%M (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %S
// second as a two-digit decimal integer (00-59)
// %t
// horizontal tab (\t) (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %T
// time expressed as %H:%M:%S (this is an extension to the ANSI standard, in accordance with the POSIX.2 standard)
// %U
// number of week in the year as two-digit decimal integer (00-52) with Sunday considered as first day of the week
// %w
// weekday as one-digit decimal integer (0-6) with Sunday as 0
// %W
// number of week in the year as two-digit decimal integer (00-52) with Monday considered as first day of the week
// %x
// full date string (no time); in the C locale, this is equivalent to "%m/%d/%y".
// %X
// full time string (no date); in the C locale, this is equivalent to "%H:%M:%S".
// %y
// year without century as two-digit decimal number (00-99)
// %Y
// year with century as four-digit decimal number
// %Z
// time zone name (e.g. EST); null string if no time zone can be obtained
// %%
// stands for '%' character in output string.