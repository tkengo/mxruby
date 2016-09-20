========
MX.zeros
========

.. rb:module:: MX

.. rb:function:: zeros(shape, opt = nil)

  Create mxarray whose all elements are zero.

  :param shape: shape array or int value.
  :param opt: Options hash. The default value is nil.

  :return: mxarray

  :raise: ArgumentError

  Options
    * *dtype* -- data type. See also :doc:`/users_guide/data_type`.

Examples
--------

  With int shape, mxarray has 1-dim.

  .. code-block:: ruby

    MX.zeros(5)
    #=> [0.0, 0.0, 0.0, 0.0, 0.0]

  With shape array, mxarray has n-dim.

  .. code-block:: ruby

    MX.zeros([2, 2])
    #=> [[0.0, 0.0], [0.0, 0.0]]

  .. code-block:: ruby

    MX.zeros([2, 2, 3])
    #=> [[[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]
