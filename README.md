# Hacknet QtSE
Hacknet QtSE is a cross-platform save editor for Hacknet, written in C++ and using the Qt framework.

Please note that this project is currently an early WIP; it does not have much functionality as of yet.

## Building
### Prerequisites
- Windows or Linux-based OS (Currently only Linux has been tested so far though)
- cmake >= 3.5
- make >= 4.4.1
- Qt >= 5
- pugixml >= 1.13

### How to build
1.  Clone this repository and `cd` into it:
    ```
    git clone https://github.com/J0w03L/Hacknet-QtSE.git
    cd Hacknet-QtSE
    ```

2.  Run `cmake` on `CMakeLists.txt`:
    ```
    cmake CMakeLists.txt
    ```
    This should create a `Makefile` file which you will need for the next step.
    
3.  Run `make`:
    ```
    make
    ```
