# aft-hal
Hardware abstraction library (HAL) for Purdue SoCET's system on chip.

# Short Term To Do List
- [x] Add CLINT structure to [pal.h](https://github.com/Purdue-SoCET/aft-hal/blob/main/src/pal/pal.h)
- [x] Add PLIC structure to [pal.h](https://github.com/Purdue-SoCET/aft-hal/blob/main/src/pal/pal.h)
- [x] Local HAL error field
- [x] Create function pointer array for interrupt handling
- [ ] Change Makefile to compile using dependencies
- [ ] Place dependencies in "build" directory (removed when clean is run)
- [ ] Restructure module source directories for peripherals that pull-in other 
      source files
- [x] 3-level handler for external interrupt handling

# Long Term To Do List
- [ ] Improved print function
- [ ] Memory allocator (bare metal malloc)
- [ ] Allow usage of C++ STL
    - [ ] Containers
    - [ ] Smart pointers
- [ ] Proper error handling
- [ ] Register debug/layout function