<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->

<a name="readme-top"></a>

<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

<!-- [![Contributors][contributors-shield]][contributors-url] -->
<!-- [![Forks][forks-shield]][forks-url] -->
<!-- [![Stargazers][stars-shield]][stars-url] -->
<!-- [![Issues][issues-shield]][issues-url] -->
<!-- [![MIT License][license-shield]][license-url] -->
<!-- [![LinkedIn][linkedin-shield]][linkedin-url] -->

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/M0sT4fa/FSM">
    <img src="resources/logo 1.jpeg" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">FSM</h3>

  <p align="center">
    Finite state machine library written in C++ for designing compilers. Might be extended in the future.
    <!-- <br /> -->
    <!-- <a href="https://github.com/M0sT4fa/FSM/"><strong>Explore the docs »</strong></a> -->
    <!-- <br /> -->
    <br />
    <a href="https://github.com/M0sT4fa/FSM">View Demo</a>
    ·
    <a href="https://github.com/M0sT4fa/FSM/issues">Report Bug</a>
    ·
    <a href="https://github.com/M0sT4fa/FSM/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <!-- <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul> -->
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#building-documentation">Building Documentation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <!-- <li><a href="#acknowledgments">Acknowledgments</a></li> -->
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->

Finite state machine library written in C++ for designing compilers. Might be extended in the future.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->

## Getting Started

<!-- This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

This project uses CMake. Therefore, to incorporate it into your project, you need to clone this project into some subdirectory within your project. This is typically done using `git submodule` or using CMake's `FetchContent` or `ExternalProject_Add`.

In the first two cases, you need to call `add_subdirectory()` on the subdirectory in which the project was fetched. In the case of `ExternalProject_Add`, the CMake command itself will do everything for you. -->

### Prerequisites

<!-- This is an example of how to list things you need to use the software and how to install them. -->

This project is built with CMake. To include it in one of your projects, your project must be CMake-based or otherwise be able to include CMake projects.

- Install CMake on Windows using `winget`

  ```pwsh
  winget install --id "Kitware.CMake"
  ```

- Install CMake for Windows and Other OSes from the [Official CMake Download Webiste].

### Installation

There are many ways to incorporate this project into your project. The simples is using `git submodule` (which is the one listed here), but you can also use `FetchContent()` and `ExternalProject_Add()` Cmake commands.

I assume that your project puts external libraries in a top level directory named external.

1. Submodule the repo into your project

   ```powershell
   git submodule add https://github.com/M0sT4fa/FSM.git external/FSM --recursive
   ```

   **⚠️Warning**: The `--recursive` option is extremely important. It makes sure dependencies of this project are fetched with it. If you didn't include it, run this command from the root directory of your project:

   ```powershell
   git submodule --remote external/FSM
   ```

2. Within `CMakeLists.txt`, add `add_subdirectory()` command with `external/FSM` directory as its argument.

   ```CMake
   # Add the following command to your top-level CMakeLists.txt
   add_subdirectory("${CMAKE_SOURCE_DIR}/external/FSM")
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->

## Usage

The main target is `fsm`. This target represents the library.

Another target is [`utility`]('https://github.com/M0sT4fa/utility.git'). This target represents a library that is a dependency of this library.

For example, assume you have an executable target named `foo` and you want to link this library to it.

```CMake
target_link_libraries(foo PRIVATE fsm)
```

_For more examples, please refer to the Documentation._

## Building Documentation

The documentation is built automatically if you set the CMake cache variable `BUILD_DOCUMENTATION`. It is built in the directory `${CMAKE_BUILD_DIRECTORY}/docs/sphinx/`. Open it using `index.html` in that directory after it is built.

Conversly, you can run the following pwershell script on the root directory:

```powershell
# Build directory
$BuildDirectory = ".\out\build\x64-Debug\"

# Build the documentation targets
cmake.exe --build $BuildDirectory --target Doxygen && cmake.exe --build $BuildDirectory --target Sphinx

# Run the documentation
& "$BuildDirectory\docs\Sphinx\index.html"
```

This script will automatically build and run the documentation, assuming your build directory is ".\out\build\x64-Debug\". Note that the build directory can be different, and in this case, this script will not be successful.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ROADMAP -->

## Roadmap

Nothing in mind currently, although I want to make the library more general. This library has come out of my own needs and I will likely add features as I need them.

See the [open issues](https://github.com/M0sT4fa/FSM/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->

## Contact

Mostafa Bideer - mostafa2018a3s@gmail.com

<!-- ⚠️**Warning**: I'm very likely to not respond, at least now, as medical school is taking most of my time. -->

Project Link: [https://github.com/M0sT4fa/FSM](https://github.com/M0sT4fa/FSM)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->

<!-- ## Acknowledgments

- []()
- []()
- []()

<p align="right">(<a href="#readme-top">back to top</a>)</p> -->

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[Official CMake Download Webiste]: https://cmake.org/download
[contributors-shield]: https://img.shields.io/github/contributors/M0sT4fa/FSM.svg?style=for-the-badge
[contributors-url]: https://github.com/M0sT4fa/FSM/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/M0sT4fa/FSM.svg?style=for-the-badge
[forks-url]: https://github.com/M0sT4fa/FSM/network/members
[stars-shield]: https://img.shields.io/github/stars/M0sT4fa/FSM.svg?style=for-the-badge
[stars-url]: https://github.com/M0sT4fa/FSM/stargazers
[issues-shield]: https://img.shields.io/github/issues/M0sT4fa/FSM.svg?style=for-the-badge
[issues-url]: https://github.com/M0sT4fa/FSM/issues
[license-shield]: https://img.shields.io/github/license/M0sT4fa/FSM.svg?style=for-the-badge
[license-url]: https://github.com/M0sT4fa/FSM/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
