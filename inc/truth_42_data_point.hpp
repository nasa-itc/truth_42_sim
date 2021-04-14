#ifndef NOS3_TRUTH42DATAPOINT_HPP
#define NOS3_TRUTH42DATAPOINT_HPP

#include <boost/shared_ptr.hpp>

#include <sim_42data_point.hpp>

namespace Nos3
{
    // vvv This is pretty standard for a data point (if one is needed to transfer data between a data provider and a hardware model for your sim)
    class Truth42DataPoint : public SimIDataPoint
    {
    public:
        // Constructor
        Truth42DataPoint(int16_t spacecraft, const boost::shared_ptr<Sim42DataPoint> dp);

        // Accessors
        std::string to_string(void) const;

        int16_t get_year(void) const {parse_data_point(); return _year;}
        int16_t get_doy(void) const {parse_data_point(); return _doy;}
        int16_t get_month(void) const {parse_data_point(); return _month;}
        int16_t get_day(void) const {parse_data_point(); return _day;}
        int16_t get_utc_hh(void) const {parse_data_point(); return _utc_hh;}
        int16_t get_utc_mm(void) const {parse_data_point(); return _utc_mm;}
        int16_t get_utc_ss(void) const {parse_data_point(); return _utc_ss;}
        int16_t get_utc_frac_secs(void) const {parse_data_point(); return _utc_frac_secs;}

    private:
        // Disallow the big 4
        Truth42DataPoint(void) {};
        Truth42DataPoint(const Truth42DataPoint&) {};
        Truth42DataPoint& operator=(const Truth42DataPoint&) {};
        ~Truth42DataPoint(void) {};

        /// @name Private mutators
        //@{
        inline void parse_data_point(void) const {if (_not_parsed) do_parsing();}
        void do_parsing(void) const;
        //@}

        // Private helper
        void year_doy_to_month_day(int16_t year, int16_t doy, int16_t &month, int16_t &day) const;

        // Private data
        Sim42DataPoint _dp;
        int16_t _sc;
        // mutable below so parsing can be on demand:
        mutable bool _not_parsed;
        mutable int16_t _year;
        mutable int16_t _doy; // day of year
        mutable int16_t _month;
        mutable int16_t _day;
        mutable int16_t _utc_hh;
        mutable int16_t _utc_mm;
        mutable int16_t _utc_ss;
        mutable int16_t _utc_frac_secs;
    };
}

#endif
