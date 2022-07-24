#include "pins_usage.hpp"

int main(void)
{
    // config lvds enable pin
	PE2::PortType::ClockEnable();
	PE2::ConfigAsOutput(GPIO_traits::func_t::PORT);
	PE2::Reset();
	// config bklt enable pin
	PE1::PortType::ClockEnable();
	PE1::ConfigAsOutput(GPIO_traits::func_t::PORT);
	PE1::Reset();
    
    while(1);
}