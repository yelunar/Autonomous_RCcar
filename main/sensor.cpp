#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <pigpio.h>

using namespace std::chrono_literals;

int main()
{
    // initialize GPIO pins
    if (gpioInitialise() < 0) {
        std::cout << "Failed to initialize GPIO.\n";
        return 1;
    }
    gpioSetMode(18, PI_OUTPUT);
    gpioSetMode(19, PI_OUTPUT);
    gpioSetPWMrange(18, 100);
    gpioSetPWMrange(19, 100);

    // set initial PWM duty cycles
    gpioPWM(18, 0);
    gpioPWM(19, 0);

    // loop until Ctrl-C is pressed
    while (true) {
        // read PID control output from file
        std::ifstream infile("output.txt");
        double output;
        infile >> output;
        infile.close();

        // update PWM duty cycles
        int left_duty_cycle = base_duty_cycle - output;
        int right_duty_cycle = base_duty_cycle + output;
        gpioPWM(18, left_duty_cycle);
        gpioPWM(19, right_duty_cycle);

        // wait for some time
        std::this_thread::sleep_for(100ms);
    }

    // cleanup GPIO
    gpioTerminate();
    return 0;
}
