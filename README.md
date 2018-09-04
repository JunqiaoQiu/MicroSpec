# MicroSpecRepo

**MicroSpec** is a C++ library which for the first time, exposes fine-grained speculative parallelism to FSM computations. 

**MicroSpec** provides a uniform interface to various DFAs, and implements the original sequential as well as coarse-grained speculative parallel execution methods, and more importantly, the four fine-grained speculative parallelization methods and the data transformation. More details can be found in [1]. 

## Getting Started

### Obtain source code

You can clone this repository by using git:

```sh
git clone https://github.com/JunqiaoQiu/MicroSpecRepo.git
```

### Install and Build

There are some requirements for building this library: `cmake2.8+`, `gcc4.9+`
It will use SIMD instructions, so please check your CPU support:
```sh
cat /proc/cpuinfo | grep flags
```

Build it:

```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make 
```

### Test

There are several test cases under test/.

## Example Usage
Here is an example use of seq-DFA execution, coarse-grained spec-DFA execution and specUnroll-DFA execution. 

```cpp
...

DFA* obj_seq = new Seq_DFA();
obj_seq->Run(table_, inputs_);

DFA* obj_spec = new Spec_DFA_Pthread();
obj_spec->Run(table_, inputs_);

DFA* obj_specunroll = new Spec_DFA_Unroll_Pthread();
obj_specunroll->Run(table_, inputs_);

```

## TODO
* Complete README and TestCases;
* Provide user-define actions;


## References:
[1] Qiu, Junqiao, Zhijia Zhao, and Bin Ren. "Microspec: Speculation-centric fine-grained parallelization for fsm computations." Parallel Architecture and Compilation Techniques (PACT), 2016 International Conference on. IEEE, 2016.


