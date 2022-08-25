###########
Terminology
###########

The purpose of this page is to introduce the terminology used by PluginPlay .

***********
Basic Terms
***********

PluginPlay::Module
   a self-contained piece of code wrapped in an API that can be inspected and
   executed by PluginPlay. The documentation uses the term PluginPlay::Module
   to avoid confusion with modules in C++20 or Python.

submodule
   A PluginPlay::Module called from within another PluginPlay::Module.
   Throughout this documentation "submodule" will always refer to a
   PluginPlay::Module calling another PluginPlay::Module.


property type
   A property type is a domain-specific quantity of interest. For
   example if your code computes geometric properties of shapes then you may
   have property types for area, volume, and perimeter. Basically these are the
   quantities that users of your code will want to compute and they define the
   manners in which a PluginPlay::Module can be called.

plugins
   These are sets of PluginPlay::Module instances that are distributed
   together. In C++ plugins will be libraries, whereas in Python plugins will
   be Python modules.

memoization
   A technique where repeated calls to an expensive function, with
   the same input, are avoided by saving the input to the function and the
   result of calling the function.

fields
   How we generically refer to the arguments to/from a function, *i.e.*, both
   inputs and results are fields.

************************
PluginPlay::Module State
************************

bound input
   For a particular PluginPlay::Module, ``M``, an input ``k`` is said to be
   bound if the ``ModuleInput`` instance returned by ``M.inputs().at("k")`` has
   a value. When a PluginPlay::Module is called as a particular property
   type, all inputs not appearing in the definition of the property type must
   be bound.

bound submodule
   Same as a "bound input" except that it refers to one of the
   submodules that a PluginPlay::Module will call instead of one of the
   input options.

transparent
   An input to a PluginPlay::Module is "transparent" if the result of calling
   the PluginPlay::Module does not depend on the value of the input. Most
   inputs are "opaque" in that their values do change the return. Examples of
   transparent options include debug information and options that affect
   the speed at which an algorithm computes (block sizes, number of threads,
   *etc.*).

opaque
   The opposite of transparent. An option is "opaque" it changing its
   value changes the result of a PluginPlay::Module.

locked
   A PluginPlay::Module is said to be locked if the end-user accessible state
   can no longer be changed. This occurs when a PluginPlay::Module is run
   and prevents concurrent access from invalidating memoization.
