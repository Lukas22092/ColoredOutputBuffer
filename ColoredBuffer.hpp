#include <iostream>
#include <vector>


struct Color{
    std::string name;
    std::string id;
};


class ColoredBuffer : public std::streambuf {
private:
    std::streambuf* original_buf;
    bool allow_looping;
    std::vector<Color> colors = {
        {"red", "\033[31m"},
        {"green", "\033[32m"},
        {"yellow", "\033[33m"},
        {"blue", "\033[34m"},
        {"magenta", "\033[35m"},
        {"cyan", "\033[36m"},
        };

    size_t counter = 0;



public:

    ColoredBuffer(std::streambuf* original) 
        : original_buf(original), allow_looping(false) {}

    void rainbow() { 
        allow_looping = true;
        original_buf->sputn(colors[counter].id.c_str(), colors[counter].id.size());
    }

    void disable() { 
        allow_looping = false;
        original_buf->sputn("\033[0m", 4); // reset to white text
            }

    void next_color() {
        counter = (counter + 1) % colors.size();
        original_buf->sputn(colors[counter].id.c_str(), colors[counter].id.size());
    }

    void set_color(std::string input_color){
        allow_looping = false;
        bool in_colors =false;
        for (Color color : colors){
            if (color.name == input_color){
                in_colors=true;
                original_buf->sputn(color.id.c_str(), color.id.size());
                break;
            }
        };
        if(!in_colors){disable();

            std::ostream original_out(original_buf); 
        original_out << "------------------------\n";
        original_out << "color " << input_color << " not found\n";
        check_avaiable_colors();
}
    }


    void check_avaiable_colors(){
        std::ostream original_out(original_buf);
        original_out<<"supported colors : \n";
        original_out << "------------------------\n";
        for(Color color : colors){
            original_buf->sputn(color.id.c_str(), color.id.size());
            original_out<<color.name<<"\n";
        }        
        disable();

        original_out << "------------------------\n";

    };
    ~ColoredBuffer() {
    original_buf->sputn("\033[0m", 4);
    }


protected:
    int overflow(int ch) override {
        if (ch == EOF) return EOF;

        if (allow_looping) {
            if (static_cast<char>(ch) == ' ') {
                // print the space and then change color
                original_buf->sputc(static_cast<char>(ch));
                next_color();
                return ch;
            } else {
                return original_buf->sputc(static_cast<char>(ch));
            }
        }
        // coloring disabled means normal output
        return original_buf->sputc(static_cast<char>(ch));
    }

};

//not using a wrapper to destroy the object after usage might result in a segm. fault.
class wrapper {
private:
    std::streambuf* old_buf;
public:
    wrapper(ColoredBuffer& rb) {
        old_buf = std::cout.rdbuf(&rb); 
    }
    ~wrapper() {
        std::cout.rdbuf(old_buf);        
    }
};


/*  usage - type this on top of your file : 

    ColoredBuffer rb(std::cout.rdbuf());
    wrapper guard(rb);  

*/
