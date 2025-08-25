#include "ColoredBuffer.hpp"
//demonstration of the tool
int main() {
    ColoredBuffer rb(std::cout.rdbuf());
    wrapper guard(rb);  

    rb.rainbow();
    std::cout<<"This is a long sentence because i need many words\n";
    rb.disable();
    std::cout<<"back to normal text"<<std::endl;
    rb.set_color("yellow");

    std::cout<<"now i am writing yellow"<<std::endl;
    std::cout<<"still yellow."<<std::endl;

    rb.set_color("darkgreen");//doesnt exist

    rb.set_color("cyan");

    std::cout<<"cyan is also quite pretty"<<std::endl;

}

