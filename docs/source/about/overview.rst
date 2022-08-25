########
Overview
########

PluginPlay is a software framework that has been designed to make it as easy
as possible to expose every aspect of scientific software to the user. In
particular PluginPlay ensures users can:

* change the algorithm used to compute a result,
* change algorithmic parameters,
* construct non-conventional workflows (*e.g.*, use a non-conventional quantity
  as an input for another algorithm)

Moreover all of this can be done dynamically as part of the user's input. At
the same time PluginPlay also strives to allow developers to easily extend the
software. In particular this includes:

* introducing a new algorithm,
* propagating the new algorithm throughout the code,
* leveraging external libraries and algorithms,
* porting algorithms to new platforms

From the get go PluginPlay is also concerned about ensuring that all of the
above can be implemented in a manner which is amenable to high-performance
computing.

PluginPlay's solution to these design points is to rely heavily on encapsulation
and standardization. On the encapsulation front, new algorithms are added by
writing self-contained, functor-like objects called "modules". Each module must
define a ``run`` member, which is used by PluginPlay to opaquely execute the
code inside the module. The overall program is then created by stitching modules
together into a call tree. As defined so far, the resulting program is likely
to experience strong inter-module coupling. This is because the API of a
given module is going to be unique to that module. Hence if the results of
module A sometimes go to module B and other times go to module C, there needs
to be dispatch code to: choose which module to call, prepare the appropriate
input, and to process the results of the call.

This is where standardization comes in. When a module is called, it is called
because it is capable of computing a result of interest. Within a typical
domain, there is a limited number of "interesting" results and a plethora of
means to compute them. In the language of the PluginPlay we term these
interesting results "properties" and require standardized APIs for each one. The
classes that implement the APIs for a property are called "property types".

The basic ideas covered here are summarized by the following UML diagram:

.. image:: ../uml/overview.*
