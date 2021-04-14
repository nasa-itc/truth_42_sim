#ifndef NOS3_TRUTH42HARDWAREMODEL_HPP
#define NOS3_TRUTH42HARDWAREMODEL_HPP

#include <map>

#include <boost/tuple/tuple.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/asio.hpp>

#include <sim_i_data_provider.hpp>
#include <truth_42_data_point.hpp>
#include <sim_i_hardware_model.hpp>

namespace Nos3
{
    // vvv This is pretty standard for a hardware model
    class Truth42HardwareModel : public SimIHardwareModel
    {
    public:
        // Constructors / destructor
        Truth42HardwareModel(const boost::property_tree::ptree& config);
        ~Truth42HardwareModel(void);

    private:
        // Private helper methods
        void send_streaming_data(NosEngine::Common::SimTime time);
        void create_data(const Truth42DataPoint& data_point, std::vector<uint8_t>& out_data);
        std::unique_ptr<NosEngine::Client::Bus>             _time_bus; // Very standard
        SimIDataProvider*                                   _truth_42_dp; // I'm only needed if the sim actually has/needs a data provider
        boost::asio::ip::udp::socket                       *_socket;
        boost::asio::ip::udp::endpoint                      _remote;
        double                                              _prev_time;
        std::uint32_t                                       _stream_period_ms;

    };
}

#endif
