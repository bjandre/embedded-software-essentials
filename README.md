```

Ben Andre
ECEN 5013 - Embedded Software Essentials
University of Colorado at Boulder
Spring 2017

Project Repository

```

# Building

## Supported make rules

  * all - Build all libraries and executables.
  
  * compile-all - Compiles all source, but does not create libraries
  or executables.
  
  * build-lib - builds all sources necessary to create libraries
  or executables.
  
  * build - builds all sources, create libraries, necessary to link
    executables.
  
  * test - Build all, then run any tests.
  
  * clean - Removes all build artifacts and editor files.
  
## Supported options
  
  * platform - The target build platform. Valid value are `[ '',
    host, bbb, frdm ]`. platform defaults to host if left unset.
  
    **NOTE**: The build system currently uses in-source
    builds. Switching platforms, e.g. from host to bbb, requires
    running `make clean`.
    
        ```
        
        make clean
        make platform=bbb all
        
        ```
  
  * release - The default build is a debug build because that is the
    most common use case. Setting release to a non-zero value will
    produce an optimized release build.

        ```
        
        make clean
        make platform=bbb release=1 all
        
        ```
  
  * project - The controlls the project code build via the
    preprocessor.  The default is the current project. To build
    another project set project to an integer.

        ```
        
        make clean
        make project=1 all
        
        ```
  
  * debug_rules - Additional output for debugging make rules. Enabled
  by setting to a non-empty value.
  
  * Changing into the relevant subdirectory and running `make XYZ.i`
  will write the preprocessed version of XYZ.c.
  
# References

  * [Auto-Dependency Generation](http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/)

# TODO

## Build system

  * Out of source builds to allow building host and target platform
    without running `make clean` between builds.

  * Try to get rid of recursive make. See:
  
    * [Recursive Make Considered Harmful](http://aegis.sourceforge.net/auug97.pdf)

    * [Multi-Architecture Builds](http://make.mad-scientist.net/papers/multi-architecture-builds/)

    * Example of [non-recursive make](https://evbergen.home.xs4all.nl/nonrecursive-make.html)
        
