#include <ItcLogger/Logger.hpp>

#include <truth_42_data_point.hpp>

namespace Nos3
{
    extern ItcLogger::Logger *sim_logger;

    /*************************************************************************
     * Constructors
     *************************************************************************/
    Truth42DataPoint::Truth42DataPoint(int16_t spacecraft, const boost::shared_ptr<Sim42DataPoint> dp) : 
        _sc(spacecraft), _dp(*dp), _not_parsed(true) 
    {
        sim_logger->trace("Truth42DataPoint::Truth42DataPoint:  Created instance using _sc=%d, _dp=%s", 
            _sc, _dp.to_string().c_str());
    }
    
   /*************************************************************************
     * Mutators
     *************************************************************************/

    void Truth42DataPoint::do_parsing(void) const
    {
        std::ostringstream TimeString;
        TimeString << "TIME";
        size_t TSsize = TimeString.str().size();
        size_t position;

        _not_parsed = false;
        
        std::vector<std::string> lines = _dp.get_lines();

        try {
            for (int i = 0; i < lines.size(); i++) {
                if (lines[i].compare(0, TSsize, TimeString.str()) == 0) { // e.g. TIME 2017-181-16:00:16.333600000
                    sim_logger->trace("Truth42DataPoint::do_parsing:  Found a string with the correct prefix = %s.  String:  %s", TimeString.str().c_str(), lines[i].c_str());
                    sim_logger->trace("FOUND TIME STRING: %s",lines[i].c_str());
                    position = lines[i].find_first_of(" ");
                    _year = std::stoi(lines[i].substr(position+1, 4));
                    position = lines[i].find_first_of("-");
                    _doy =std::stoi(lines[i].substr(position+1, 3));
                    year_doy_to_month_day(_year, _doy, _month, _day);
                    position = lines[i].find_last_of("-"); 
                    _utc_hh = std::stoi(lines[i].substr(position+1,2));
                    position = lines[i].find_first_of(":");
                    _utc_mm = std::stoi(lines[i].substr(position+1,2));
                    position = lines[i].find_last_of(":");
                    _utc_ss = std::stoi(lines[i].substr(position+1,2));
                    position = lines[i].find_first_of(".");
                    _utc_frac_secs = std::stoi(lines[i].substr(position+1,4));
                }
            }
        } catch(const std::exception& e) {
            sim_logger->error("GNSS200DataPoint::do_parsing:  Parsing exception:  %s", e.what());
        }

        sim_logger->trace("GNSS200DataPoint::do_parsing:  Parsed data point:\n%s", to_string().c_str());
    }

    /*************************************************************************
     * Private helpers
     *************************************************************************/
    
    void Truth42DataPoint::year_doy_to_month_day(int16_t year, int16_t doy, int16_t &month, int16_t &day) const
    {
        unsigned short int leap; // 0 = normal year, 1 = leap year
        if (year%4 == 0) {
            if (year%100 == 0) {
                if (year%400 == 0) {
                    leap = 1;
                } else {
                    leap = 0;
                }
            } else {
                leap = 1;
            }
        } else {
            leap = 0;
        }

        const unsigned short int mon_yday[2][13] =
        {
            /* Normal years.  */
            { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
            /* Leap years.  */
            { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
        };
        day = 0;
        month = 0;
        for (unsigned short int i = 0; i < 12; i++) {
            if ((doy > mon_yday[leap][i]) && (doy <= mon_yday[leap][i+1])) {
                day = doy - mon_yday[leap][i];
                month = i+1;
                break;
            }
        }
    }


    /*************************************************************************
     * Accessors
     *************************************************************************/

    std::string Truth42DataPoint::to_string(void) const
    {
        parse_data_point();
        
        std::stringstream ss;

        ss << std::fixed << std::setfill(' ');
        ss << "42 Data Point:";
        ss << " year: " << _year;
        ss << " day of year: " << _doy;
        ss << " month: " << _month;
        ss << " day: " << _day;

        return ss.str();
    }
    
}
