.. _e_and_m_101:

#######
E&M 101
#######

.. |e_field| replace:: :math:`\vec{E}\left(\vec{r}\right)`
.. |q_i| replace:: :math:`q_i`
.. |r_i| replace:: :math:`\vec{r}_i`
.. |r_i_hat| replace:: :math:`\hat{r}_i`
.. |N| replace:: :math:`N`
.. |r| replace:: :math:`\vec{r}`
.. |i| replace:: :math:`i`

Let |e_field| be the electric field at a point |r| which arises from a set
of |N| point charges. If the |i|-th point charge has a charge |q_i| and is
located at |r_i|, |e_field| is given by (up to some constants):

.. math::

   \vec{E}(\vec{r}) = \sum_{i=1}^N \frac{q_i \hat{r}_i}
                                   {||\vec{r} - \vec{r}_i||^2}

where  |r_i_hat| is a unit vector in the direction of |r_i|.

Assume we want to write a code which can compute |e_field|. In PluginPlay this
code has three parts:

#. the PluginPlay::Module which actually computes |e_field|,
#. the property type defining the API of our PluginPlay::Module, and
#. the plugin.

Right now we are concerned with the first point. So we ignore plugins entirely
and assume that there exists a header file ``electric_field.hpp`` which
defines a property type ``ElectricField`` with an API:

.. code:: c++

   Point ElectricField(Point, std::vector<PointCharge>);

The ``Point`` and ``PointCharge`` classes are defined in the
``point_charge.hpp`` header:

.. literalinclude:: ../../../tests/pluginplay/docs/point_charge.hpp
   :language: c++
   :lines: 1-13
