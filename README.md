# MicroSpecRepo

**MicroSpec** is a C++ library which for the first time, exposes fine-grained speculative parallelism to FSM computations. 

**MicroSpec** provides a uniform interface to various DFAs, and implements the original sequential as well as coarse-grained speculative parallel execution methods, and more importantly, the four fine-grained speculative parallelization methods and the data transformation. More details can be found in our paper [1]. 

## Getting Started

### Obtain source code

You can clone this repository by using git:

```sh
git clone https://github.com/JunqiaoQiu/MicroSpecRepo.git
```

### Install and Build

There are some requirements for building this library: `cmake2.8+` and `gcc4.9+`. 

Since some of the fine-grained speculative FSM parallelization use SIMD instructions, so please check whether your CPU support AVX2 if you want to use all of the provided techniques:
```sh
cat /proc/cpuinfo | grep flags
```

Build it:

```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. 
 # If not AVX2 supported, use: cmake -DCMAKE_BUILD_TYPE=Release -DAVX2_SUPPORT:BOOL=OFF  ..
make 
```
### Test

There are several test cases under test/, which are used to test the following class / functions.

- **MappingRule**: defines how to transform the char-type character to an int (e.g., extended ASCII codes)

- **Predictor**: provides a light weight look back prediction (support both sequential and parallel predictions)

- **Input**: represents the given input streams

- **Table**: represents basic information of the give DFA, i.e., the transition table and start state.

- **DFA**: provides DFA executions/computations, and the most important function in it is *run*(*Table*\*, *Input*\*). 

## Example Usage
Here is an example use of different kinds of techniques in **MicroSpec**. 

```cpp
...

DFA* obj_seq = new SeqDFA();
obj_seq->run(table_, inputs_);

DFA* obj_specPthread = new SpecDFA_Pthread(numberOfThreads);
obj_specPthread->run(table_, inputs_);

DFA* obj_specUnrollPthread = new SpecDFA_Unroll_Pthread(numberOfThreads);
obj_specUnrollPthread->run(table_, inputs_);

DFA* obj_specGatherPthread = new SpecDFA_Gather_Pthread(numberOfThreads);
obj_specGatherPthread->run(table_, inputs_);

DFA* obj_specGatherUnrollPthread = new SpecDFA_GatherUnroll_Pthread(numberOfThreads);
obj_specGatherUnrollPthread->run(table_, inputs_);

DFA* obj_specUnrollGatherPthread = new SpecDFA_UnrollGather_Pthread(numberOfThreads);
obj_specUnrollGatherPthread->run(table_, inputs_);

...

```

## References
[1] Junqiao Qiu, Zhijia Zhao, and Bin Ren. "Microspec: Speculation-centric fine-grained parallelization for fsm computations." Proceedings of the 2016 International Conference on Parallel Architectures and Compilation. ACM, 2016.

