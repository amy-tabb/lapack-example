# lapack-example

This repository contains an example for using Eigen matrices with Fortran Lapack within C++.  It was adapted and updated from [the Eigen docs, 2010](https://eigen.tuxfamily.org/index.php?title=Lapack). This is a CMake project.

## Install LAPACK

On Ubuntu 18.04, run 

```bash
sudo apt install liblapack3
```

## Find and check the symbol name

First, you need to find where the liblapack.so file is.  For my Ubuntu 18.04 system, it was at `/usr/lib/x86_64-linux-gnu`, and then grep for the function you want -- 

```bash
readelf -s /usr/lib/x86_64-linux-gnu/liblapack.so | grep dggev
```

The example post showed using the Genererlized Eigen-problem function `dggev`, so that's what I did too.

and the result was 

```
1322: 00000000001eac90 5911 FUNC GLOBAL DEFAULT 12 dggev3_
1709: 00000000001ec3b0 7779 FUNC GLOBAL DEFAULT 12 dggevx_
1929: 00000000001e9750 5425 FUNC GLOBAL DEFAULT 12 dggev_
```

The documentation for dggev is [here](http://www.netlib.org/lapack/explore-html/d9/d8e/group__double_g_eeigen_ga4f59e87e670a755b41cbdd7e97f36bea.html), for [dggev3, here](http://www.netlib.org/lapack/explore-html/d9/d8e/group__double_g_eeigen_ga28d81b96083b18aa21c47e2e6ff59e5a.html).

## C++ code; building 

Is in the `src` folder.  Some notes compared to the original tutorial -- Eigen has changed since then, so a const_cast is not needed for `.data()`, and A and B are altered while running `ddgev`.

This is a cmake project.  First, create a build folder.  From the build folder, 

- `cmake ../src` (basic)
- `cmake -DCMAKE_BUILD_TYPE=Release ../src` (Release configuration)
- `cmake -DCMAKE_BUILD_TYPE=Debug ../src` (Debug configuration)
- `cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../src/` (Create an Eclipse project to import -- it will be in the build folder)

and then `make`, or import the project to your IDE and play around with it.  Run by typing

```bash
$ ./lapack-example 
```

The output on my computer was:

```c++
Before calling the GEP function. 
Matrix A 
  0.680375   0.823295  -0.444451  -0.270431
 -0.211234  -0.604897    0.10794  0.0268018
  0.566198  -0.329554 -0.0452059   0.904459
   0.59688   0.536459   0.257742    0.83239
Matrix B 
 0.271423 -0.967399 -0.686642  0.997849
 0.434594 -0.514226 -0.198111 -0.563486
-0.716795 -0.725537 -0.740419 0.0258648
 0.213938  0.608354 -0.782382  0.678224
A 
  -1.10665  -0.712483 -0.0185513    0.18791
         0   0.537515   -1.29261   0.632071
         0   0.237337  -0.161502  -0.345992
         0          0          0   0.396951
B
  0.739598 -0.0797263  -0.192251 -0.0954299
         0    1.63202          0   -0.52068
         0          0   0.965544  -0.583576
         0          0          0    1.24779
lambda 
 -1.10665         0  0.739598
 0.121661  0.547516   1.50115
0.0850747 -0.382864   1.04972
 0.396951         0   1.24779
v 
 -0.303799   0.474613  -0.478612   0.820211
  0.456401  -0.146975 -0.0902062  -0.773746
        -1   0.924007 -0.0759929   0.884707
 -0.700859  -0.574726 -0.0716408         -1
```

