[![LinkedIn][linkedin-shield]][linkedin-url]
<!--
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]


[![Github][github-shield]][github.com/zoumson?tab=repositories]
[![Stack Overflow][stackoverflow-shield]][stackoverflow.com/users/11175375/adam]
[![Leetcode][leetcode-shield]][eetcode.com/Hard_Code/]
-->
## Curve fitting 

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#file-structure">Files Structure</a>
      <ul>
        <li><a href="#folders">Folders</a></li>
        <li><a href="#entire-files-structure">Entire Files Structure</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->

In regression analysis, curve fitting is the process of specifying the model that provides the best fit to the specific curves in your dataset. 
Curved relationships between variables are not as straightforward to fit and interpret as linear relationships.
For linear relationships, as you increase the independent variable by one unit, the mean of the dependent variable always changes by a specific amount. 
This relationship holds true regardless of where you are in the observation space.

Unfortunately, the real world isn’t always nice and neat like this. Sometimes your data have curved relationships between variables. In a curved relationship, the change in the dependent variable associated with a one unit shift in the independent variable varies based on the location in the observation space. 
In other words, the effect of the independent variable is not a constant value. Most curves used to fit experimental data are non-linear. 
In this project two c++ frameworks, namely, `ceres` and `g2o` are used to estimate the parameter of the model given a set of experimenal data.

The fitting model is as follows:  

<a href="https://www.codecogs.com/eqnedit.php?latex=\large&space;y=\exp&space;(a\cdot&space;x^{2}&plus;b\cdot&space;x&plus;c)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\large&space;y=\exp&space;(a\cdot&space;x^{2}&plus;b\cdot&space;x&plus;c)" title="\large y=\exp (a\cdot x^{2}+b\cdot x+c)" /></a>

<!--Built with -->
### Built With

<br>

* [cmake](https://cmake.org/)
* [gnu](https://www.gnu.org/)
* [eigen](https://eigen.tuxfamily.org/)
* [g2o](https://github.com/RainerKuemmerle/g2o/)
* [ceres](https://github.com/ceres-solver/ceres-solver/)
* [suitesparse](https://people.engr.tamu.edu/davis/suitesparse.html/)
* [qt](https://www.qt.io/)
* [boost](https://www.boost.org/)
* [opencv](https://opencv.org/)
<br>

## File Structure

### Folders

* [include/](include/): c++ header files.
* [src/](src/): c++ definitions.


### Entire Files Structure 


```
.
├── CMakeLists.txt
├── include
│   ├── CeresCost.h
│   └── CommonCurve.h
├── README.md
└── src
    ├── ceres
    │   ├── CMakeLists.txt
    │   ├── cmake_modules
    │   │   └── CeresConfig.cmake.in
    │   └── demoCeres.cpp
    └── g2o
        ├── CMakeLists.txt
        ├── cmake_modules
        │   └── FindG2O.cmake
        └── demoG2O.cpp

6 directories, 10 files


```


<!-- GETTING STARTED -->
## Getting Started

This is a sample code of how you may use  `g2o` and `ceres` to perform curve fitting.
To get a local copy up and running follow these simple steps.

### Prerequisites

This is an example of how to list things you need to use the software and how to install them.
* cmake
  ```sh
  sudo apt-get install cmake
  ```
 * eigen
 ```sh
 sudo apt-get install -y libeigen3-dev
 ```
 * suitesparse
 ```sh
 sudo apt-get install -y libsuitesparse-dev
 ```
  * qt
 ```sh
 sudo apt-get install -y qtdeclarative5-dev qt5-qmake libqglviewer-dev
 ```
  * g2o
 ```sh
 sudo su 
 ```
  ```sh
 cd /opt && mkdir g2oInstall && cd g2oInstall \
 && git clone https://github.com/RainerKuemmerle/g2o.git \
 && cd g2o && mkidr build && cd build && cmake .. \
 && make -j4 && make install
 ```
  * ceres
 ```sh
 sudo apt-get install libgoogle-glog-dev libatlas-base-dev && \
 && cd /opt && mkdir ceresInstall && cd ceresInstall \
 && git clone https://github.com/ceres-solver/ceres-solver.git \
 && cd ceres-solver && mkdir build && cd build && cmake .. \
 && make -j4 && make install
 ```
  * opencv4
 ```sh
 sudo su
 ```
 ```sh
git clone https://github.com/zoumson/OpencvInstall.git     \
&& cd OpencvInstall && chmod +x install.sh && ./install.sh
 ```
  * Boost
  ```sh
    sudo apt-get install libboost-all-dev
  ```
 
 
### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/zoumson/CurveFitting.git
   ```
2. Go to the project directory source
   ```sh
   cd CurveFitting
   ```
3. Create empty directories `build`, and `bin`
   ```sh
   mkdir build &&  mkdir bin 
   ```
5. Generate the exectutables and move them to `bin`
   ```sh
   cd build && cmake .. && make -j4 && cd ..
   ```

<!-- USAGE EXAMPLES -->
### Usage
1. Command line options
```sh
Usage: options_description [options]
Allowed options:
  -h [ --help ]                         produce help message
  -a [ --first ] arg                    first parameter
  -b [ --second ] arg                   second parameter
  -c [ --third ] arg                    third parameter
  -n [ --number ] [=arg(=500)] (=100)   number of data
  -i [ --iteration ] [=arg(=200)] (=100)
                                        number of iterations
  -s [ --noise ] [=arg(=5)] (=1)        noise added

```
2. Run with `ceres`
```sh
./bin/demoCeres -a 1 -b 1 -c 1
```
3. Sample Data generated as `29_06_2021_13_46_37_readDataCeres.txt` in directory `result`
4. Parameter for the model saved in `29_06_2021_13_46_37_parametersCeres.txt`
```sh
Estimated a,b,c =0.660982 1.5863 -0.190824
```
5. Run with `g2o`
```sh
./bin/demoG2O -a 1 -b 1 -c 1
```
6. Sample Data generated as `29_06_2021_13_49_32_readDataG2O.txt` in directory `result`
7. Parameter for the model saved in `29_06_2021_13_46_37_parametersCeres.txt`
```sh
estimated model a, b, c: 0.656489  1.59246 -0.19271
```
8. Back to the initial file structure configuration
   ```sh
   rm -r bin build result
   ```
<!-- ROADMAP -->
## Roadmap

All the headers files are well docummented, read through the comments

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Adama Zouma - <!-- [@your_twitter](https://twitter.com/your_username) -->- stargue49@gmail.com

Project Link: [https://github.com/zoumson/CurveFitting](https://github.com/zoumson/CurveFitting.git)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [Google](https://www.google.com/)
* [Stack Overflow](https://stackoverflow.com/)
* [Github](https://github.com/)




<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: linkedin.com/in/adama-zouma-553bba13a
[product-screenshot]: images/screenshot.png

