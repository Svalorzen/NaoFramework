NaoFramework
============

A modular framework for working with NAO robots.

Introduction
============

This code was written as a proof-of-concept in order to decide within the Dutch
Nao Team whether to switch over to a completely new framework to handle Nao
robots. The proof-of-concept work originated from a lack of inner knowledge of
the then used framework, unmanageability and size.

Another advantage of adopting a new framework would have been the possibility of
adoption within a number of other Dutch robotics teams, including for example
the Maneki-Neko drone team.

In the end the new framework was discarded due to the complexity of porting the
already existing software to run inverse-kinematics, computer vision and other
algorithms which were already available from other RoboCup teams (the one we
settled on is B-Human). Nonetheless this framework, done over the course of
pretty much a single week, remains as proof that such a transition may be
possible.

Architecture
============

This framework takes into account already existing work (for example our
previous codebase), and tries to maintain a similar setup throughout. At the
same time, it tries to fix what were considered the worst flaws of the old
system.

At the core of the framework is the need of building different "modules", which
are required to perform specific tasks and contribute to the overall behavior of
the robot. Modules may implement any AI algorithm that a team decides is worth
looking at, and it may need to communicate with other modules through custom
data structures which are not known beforehand.

To do this, this framework provides a number of classes for module communication
and implementation. The idea is that each module will be implemented as a
separate project, and be compiled as a dynamic library. This framework will then
load at runtime all requested dynamic libraries and at the same time
cross-checking data dependencies between modules to ensure that the loaded
configuration is valid.

A Blackboard class enables modules to talk to each other through the sharing of
data structures, the types of which need to be agreed beforehand (possibly
through inclusion of shared header files). In addition, the framework allows
multiple threads of execution for separate lists of modules, and communication
between modules residing in different threads (although with some slight
restrictions).

Every class in this framework comes with documentation, although a more general
documentation as not been written, since this was only a proof of concept. A
general overview of all classes in the framework can be found below.

## Namespaces ##

The framework classes are divided into four groups (and namespaces):

- Comm: This namespace contains the classes that enable all modules to talk to
  each other. 
- Console: This namespace contains temporary code that was used to have a CLI
  interface with the framework, using the readline library. Since this code
  could have also been used elsewhere, it has been split into a separate library
  and is available and maintained [here](https://github.com/Svalorzen/cpp-readline/)
- Core: This namespace contains all code that resides at the heart of the framework. It
  contains the classes that run all the code of the framework.
- Log: This namespace contains code that wraps the current logging
  implementation (boost::log), in order to be more easily used.
- Modules: This namespace wraps classes that relate to module implementation and
  loading.

## Flow ##

The theoretical flow of the framework should be the following:

- The framework reads from configuration files/user input the specified threads
  to create, and what modules they should contain.
- The framework looks for the appropriate dynamic libraries, and loads them in.
  The modules will in the future run in the same order in which they were
  loaded.
- During loading, each module is allowed to register through the Blackboard
  class which data structures it would like access to during runtime.
- Blackboard checks at each new module that no nonexistent fields are accessed
  before they are actually supplied.
- At the end of the loading, the framework then needs to verify cross-thread
  data dependencies.
- If all is well, desired threads can be started and data gathering/processing
  begins.

Build
=====

The code can be currently built with cmake. From the command line, it simply
requires the following commands:

    mkdir build
    cd build
    cmake ..
    make

This will create an executable called `framework_test` in the project main dir.

In addition, two dummy modules can be compiled in the `moduleExamples` folder.
To do this, you can use the following commands:

    cd moduleExamples
    ./compileModule Reader
    ./compileModule Writer

This will create two dynamic libraries for the two modules, that are ready to be
used by the framework. This modules are dummies, and their code is not nice: the
idea was to keep every module in a separate repository.

Usage
=====

Currently the framework comes in with a restricted amount of commands. They are:

- help: lists all available commands.
- test: starts all threads in the background. If the modules' dependencies are
  not met, the threads will not start. Currently there is no way to remove
  loaded modules and try again, nor to stop execution and load more modules
  (though the latter is supported by the code).
- create: creates a new named wave, which is a thread of execution, which can contain
  multiple modules.
- add: adds a new module to an existing wave to be loaded.
- quit/exit: terminates the program.
- run: runs a script, containing for each line one of the mentioned commands.

Documentation
=============

Documentation can be built with the usage of doxygen. To do so you simply need
to run the following command in the project main folder:

    doxygen

This will create an html folder. In it you can load in your browser the
`index.html` file, which will allow you to read the code API documentation.

In addition the `doc` folder contains a brief pdf that was used to present ideas
behind the framework to the team.
