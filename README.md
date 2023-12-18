This is a revised and redesigned updated version of the original Virtual Worlds prototype. 

Same concepts applied for generating and displying HCP Voxels but using the OpenFrameWork as a basis for the application, but with some modifications where neccesary.

The OpenFramWorks files that have had modifications and have been added to the Openframeworks directory structure are included in the directory libs/Openframworks

OpenFrameworks was chosen as it aligns very well with the original design concepts and coding that the application was developed under.

Note : This is a work in progress. 

## Introduction

Procedural hexagonal close packing (HCP) point cloud data and voxel generation

This is a working project to demonstrate the concept of a different form of procedural point cloud and voxel geometry to use in 3D space. Voxels are conventionally defined as a cube in 3D space, but for the purposes of simulation and cellular automata, cubic voxels would not be a particularly good choice as the distaces between each cubic voxel are not all equal. An arrangement of hexagonal close packing (HCP) spheres of equal radius can produce such a geometry that every data point in space is surrounded by neighbours all of equal distance from it. Where as a cube is the space filling geometric shape that defines a cubic voxel space, the geometric shape of a Trapezo-rhombic dodecahedron is what can be used to fill a HCP matrix of points in 3D space, and can be used as a space filling voxel.

Documentation not yet provided.

This project is not a fully functioning application, but is a working prototype and model of an application with the aim to be a basis of evolution to test ideas and concepts so as to create an application that utilises a HCP geometry for simulation, celluar automata, pocedural graphical generation, and any other ideas that may come to being.

As there was nothing that could be found that resembles this concept and idea, it is presented here to bring to attention, an alternative geometric system to use with a solution of example code on how this can be done. The intention here is to give an idea to those that have better skills, knowledge and understanding than this author to use and take this further and develop quicker or in other areas.

The intent is also to have this concept and idea completely open source and any derivation free of copywrite or any other retriction that could be imposed.

## Installation:

This project is a Visual Studio 2022 C++ project set up for windows.

This projects uses OpenFrameworks version 0.12.0

1 : Download the code and install it according to the instructions given on the OpenFramworks website,
    https://openframeworks.cc/

2 : Download the files within this repository to a desired directory location.

3 : Go to the installation directory of the OpenFramworks and one should see the directories apps and libs as given by this repository.

4 : Copy all the files witin the downloaded copy of this repository into the created OpenFrameworks project directory. 
    Beware the overwriting of some the original OpenFrameworks files and adding
    new files will occur, but should not cause any problems with OpenFrameworks examples etc as the modified files are 
    of additive code that does not alter the functionality of existing code.
    
5 : Open visual studio and ope the project sln file that is within the apps/myApps/Virtual_Worlds directory.

6 : Make any neccessary changes to the project to be able to compile the code.

7 : A compiled executable and example data are available in the directory apps/myApps/Virtual_Worlds/Build.Virtual_Worlds/x64/Release

Note : this installation is not tested as yet, but should work.

## Dependencies
    These sre a list of all the current third party dependacies for this project
    
    OpenFrameWorks version 0.12.0
    ImGui
    imgui-docking
    tinyFileDialog
    assimp - From initial devolpment : Not used but may be needed in future
    
Other than OpenFrameWOrks, all the dependancies are all provided in the directories

                apps/myApps/Virtual_Worlds/source/FramwWork/Kernals
and
                apps/myApps/Virtual_Worlds/source/FramwWork/Universal_FW

## Source Code

Because this is a working project, within the source code is a lot of debugging code that has largely been commented out or of redundant nature from migration from a previous Qt implementation ready to be reused or modifed.

Much of the code has been written for as easy reading as possible to understand what the code does. However some of the code that has been adopted or copied from 3rd parties may follow a different naming convention.
